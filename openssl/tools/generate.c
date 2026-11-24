#define SP_IMPLEMENTATION
#include "sp.h"

static const c8* features [] = { "no-asm", "no-shared", "no-module", "no-tests", "no-winstore" };
static const c8* roots [] = { "include/", "crypto/", "providers/", "apps/" };
static const c8* extensions [] = { "h", "c", "inc" };
static const c8* configured [] = { "include/openssl/configuration.h", "include/crypto/dso_conf.h" };
static const c8* unset [] = { "CC", "CXX", "CFLAGS", "CPPFLAGS", "LDFLAGS" };
static const c8* settings [] = { "OPENSSL_NO_", "OPENSSL_THREADS", "OPENSSL_RAND_SEED", "OPENSSL_CONFIGURED_API" };

typedef struct {
  sp_mem_t mem;
  sp_env_t env;
  sp_str_t openssl;
  sp_str_t work;
  sp_str_t tree;
  sp_str_t gen;
  sp_str_t config;
} generate_t;

static sp_str_t join(generate_t* g, sp_str_t base, const c8* sub) {
  return sp_fs_join_path(g->mem, base, sp_cstr_as_str(sub));
}

static bool starts_with_any(sp_str_t str, const c8** prefixes, u32 count) {
  sp_for(it, count) {
    if (sp_str_starts_with(str, sp_cstr_as_str(prefixes[it]))) {
      return true;
    }
  }
  return false;
}

static bool equals_any(sp_str_t str, const c8** values, u32 count) {
  sp_for(it, count) {
    if (sp_str_equal_cstr(str, values[it])) {
      return true;
    }
  }
  return false;
}

static sp_err_t run(generate_t* g, sp_ps_config_t config, sp_str_t* out) {
  config.env.mode = SP_PS_ENV_EXISTING;
  config.env.env = g->env;
  config.io.out.mode = out ? SP_PS_IO_MODE_CREATE : SP_PS_IO_MODE_NULL;
  sp_ps_output_t result = sp_ps_run(g->mem, config);
  if (result.error) {
    sp_log_err("failed to start {}", sp_fmt_str(config.command));
    return result.error;
  }
  if (result.status.exit_code) {
    sp_log_err("{} exited with {}", sp_fmt_str(config.command), sp_fmt_uint((u32)result.status.exit_code));
    return SP_ERR;
  }
  if (out) {
    *out = result.out;
  }
  return SP_OK;
}

static sp_err_t prepare(generate_t* g) {
  sp_try(run(g, (sp_ps_config_t) {
    .command = sp_str_lit("spn"),
    .args = { sp_str_lit("path"), sp_str_lit("openssl") },
  }, &g->openssl));
  g->openssl = sp_str_trim_right(g->openssl);

  if (sp_fs_exists(g->work)) {
    sp_try(sp_fs_remove_dir(g->work));
  }
  sp_try(sp_fs_create_dir(g->work));
  return sp_fs_copy_tree_at(sp_path_at_cwd(g->openssl), sp_path_at_cwd(g->tree), SP_FS_ATOMIC_REPLACE);
}

static sp_err_t configure(generate_t* g) {
  sp_ps_config_t config = {
    .command = sp_str_lit("perl"),
    .args = { sp_str_lit("Configure") },
    .cwd = g->tree,
  };
  sp_carr_for(features, it) {
    sp_ps_config_add_arg(g->mem, &config, sp_cstr_as_str(features[it]));
  }
  sp_ps_config_add_arg(g->mem, &config, sp_str_lit("linux-x86_64"));
  return run(g, config, SP_NULLPTR);
}

static sp_err_t build(generate_t* g) {
  sp_str_t database = sp_zero;
  sp_try(run(g, (sp_ps_config_t) {
    .command = sp_str_lit("make"),
    .args = { sp_str_lit("-s"), sp_str_lit("-pn") },
    .cwd = g->tree,
  }, &database));

  sp_ps_config_t config = {
    .command = sp_str_lit("make"),
    .args = { sp_str_lit("-s"), sp_str_lit("-j8"), sp_str_lit("build_generated") },
    .cwd = g->tree,
  };
  sp_str_t prefix = sp_str_lit("GENERATED = ");
  sp_da(sp_str_t) lines = sp_str_split_c8(g->mem, database, '\n');
  sp_da_for(lines, it) {
    if (!sp_str_starts_with(lines[it], prefix)) {
      continue;
    }
    sp_da(sp_str_t) targets = sp_str_split_c8(g->mem, sp_str_sub(lines[it], (s32)prefix.len, (s32)(lines[it].len - prefix.len)), ' ');
    sp_da_for(targets, target) {
      if (starts_with_any(targets[target], roots, sp_carr_len(roots))) {
        sp_ps_config_add_arg(g->mem, &config, sp_str_trim(targets[target]));
      }
    }
    break;
  }
  sp_try(run(g, config, SP_NULLPTR));

  sp_str_t info = sp_zero;
  sp_try(run(g, (sp_ps_config_t) {
    .command = sp_str_lit("perl"),
    .args = { sp_str_lit("util/mkbuildinf.pl"), sp_str_lit("spn"), sp_str_lit("spn") },
    .cwd = g->tree,
    .env.extra = { { .key = sp_str_lit("SOURCE_DATE_EPOCH"), .value = sp_str_lit("0") } },
  }, &info));
  sp_str_t path = join(g, g->tree, "crypto/buildinf.h");
  if (sp_fs_exists(path)) {
    sp_try(sp_fs_remove_file(path));
  }
  return sp_fs_create_file_str(path, info);
}

static sp_err_t harvest(generate_t* g, u32* copied) {
  sp_da(sp_fs_entry_t) entries = sp_zero;
  sp_try(sp_fs_collect_recursive(g->mem, g->tree, &entries));

  if (sp_fs_exists(g->gen)) {
    sp_try(sp_fs_remove_dir(g->gen));
  }

  sp_da_for(entries, it) {
    sp_fs_entry_t* entry = &entries[it];
    if (entry->kind != SP_FS_KIND_FILE) {
      continue;
    }
    if (!starts_with_any(entry->rel, roots, sp_carr_len(roots))) {
      continue;
    }
    if (!equals_any(sp_fs_get_ext(entry->name), extensions, sp_carr_len(extensions))) {
      continue;
    }
    if (equals_any(entry->rel, configured, sp_carr_len(configured))) {
      continue;
    }
    if (sp_fs_exists(sp_fs_join_path(g->mem, g->openssl, entry->rel))) {
      continue;
    }
    sp_path_t to = sp_path_at_cwd(sp_fs_join_path(g->mem, g->gen, entry->rel));
    sp_try(sp_fs_create_parent_at(to));
    sp_try(sp_fs_copy_file_at(sp_path_at_cwd(entry->path), to, SP_FS_ATOMIC_REPLACE));
    (*copied)++;
  }
  return SP_OK;
}

static sp_err_t read_settings(generate_t* g, sp_str_t path, sp_da(sp_str_t)* found) {
  sp_str_t content = sp_zero;
  sp_try(sp_io_read_file(g->mem, path, &content));

  *found = sp_da_new(g->mem, sp_str_t);
  sp_str_t define = sp_str_lit("define ");
  sp_da(sp_str_t) lines = sp_str_split_c8(g->mem, content, '\n');
  sp_da_for(lines, it) {
    sp_str_t line = sp_str_trim_right(lines[it]);
    if (!sp_str_starts_with(line, sp_str_lit("#"))) {
      continue;
    }
    line = sp_str_trim_left(sp_str_sub(line, 1, (s32)(line.len - 1)));
    if (!sp_str_starts_with(line, define)) {
      continue;
    }
    sp_str_t name = sp_str_sub(line, (s32)define.len, (s32)(line.len - define.len));
    if (starts_with_any(name, settings, sp_carr_len(settings))) {
      sp_da_push(*found, line);
    }
  }
  sp_da_sort(*found, sp_str_sort_kernel_alphabetical);
  return SP_OK;
}

static sp_err_t check(generate_t* g) {
  sp_da(sp_str_t) generated = sp_zero;
  sp_da(sp_str_t) vendored = sp_zero;
  sp_try(read_settings(g, join(g, g->tree, "include/openssl/configuration.h"), &generated));
  sp_try(read_settings(g, join(g, g->config, "include/openssl/configuration.h"), &vendored));

  bool equal = sp_da_size(generated) == sp_da_size(vendored);
  if (equal) {
    sp_da_for(generated, it) {
      if (!sp_str_equal(generated[it], vendored[it])) {
        equal = false;
        break;
      }
    }
  }
  if (!equal) {
    sp_log_err("config/include/openssl/configuration.h disagrees with Configure");
    return SP_ERR;
  }
  return SP_OK;
}

static sp_err_t generate(generate_t* g, u32* copied) {
  sp_try(prepare(g));
  sp_try(configure(g));
  sp_try(build(g));
  sp_try(harvest(g, copied));
  return check(g);
}

static s32 generate_main(s32 num_args, const c8** args) {
  generate_t g = {
    .mem = sp_mem_heap_as_allocator(sp_mem_heap_new()),
    .work = sp_str_lit("build/generate"),
    .gen = sp_str_lit("gen"),
    .config = sp_str_lit("config"),
  };
  g.tree = join(&g, g.work, "openssl");
  g.env = sp_env_capture(g.mem);
  sp_carr_for(unset, it) {
    sp_env_erase_c(&g.env, unset[it]);
  }

  u32 copied = 0;
  if (generate(&g, &copied)) {
    return 1;
  }
  sp_log("regenerated {} files in {}", sp_fmt_uint(copied), sp_fmt_str(g.gen));
  return 0;
}
SP_MAIN(generate_main)
