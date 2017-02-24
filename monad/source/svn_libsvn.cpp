#include <iostream>

#include <apr_pools.h>

#include <svn_types.h>
#include <svn_client.h>
#include <svn_cmdline.h>
#include <svn_config.h>
#include <svn_opt.h>
#include <svn_path.h>
#include <svn_pools.h>
#include <svn_version.h>

#include "util.h"
#include "svn.h"

namespace svn {
    namespace internal {
        bool enabled = false;

        apr_pool_t *pool;

        const svn_version_checklist_t lib_version[] = {
            {"svn_client", svn_client_version},
            {"svn_subr", svn_subr_version},
            {"svn_ra", svn_ra_version},
            {NULL, NULL}
        };

        SVN_VERSION_DEFINE(my_version);

        svn_client_ctx_t *ctx;

        void handle_error(svn_error_t *err) {
            if (err) {
                fputs("\n", stderr);
                svn_handle_error2(err, stderr, FALSE, "svn: ");
            }
            svn_error_clear(err);
        }

        struct notify_baton {
            bool had_print_error;
            const char *path_prefix;
        };


        const char *
        local_style_skip_ancestor(const char *parent_path,
                const char *path,
                apr_pool_t *subpool) {
            const char *relpath = NULL;

            if (parent_path)
            {
                relpath = svn_path_is_child(parent_path, path, pool);

                if (!relpath)
                {
                    if (strcmp(path, parent_path) == 0)
                        relpath = ".";
                    else
                        relpath = path;
                }
            }

            return svn_path_local_style(relpath ? relpath : path, subpool);
        }

        // notify is pretty much copied from subversion 1.8.10, with some
        // things removed, partly since they don't apply and partly to get it
        // to compile with SVN 1.6.
        void notify(void *baton, const svn_wc_notify_t *n, apr_pool_t *subpool) {
            notify_baton *nb = (notify_baton*)baton;
            char statchar_buf[5] = "    ";
            const char *path_local;
            svn_error_t *err;

            if (n->url)
                path_local = n->url;
            else
            {
                if (n->path_prefix)
                    path_local = local_style_skip_ancestor(n->path_prefix, n->path, subpool);
                else
                    path_local = local_style_skip_ancestor(nb->path_prefix, n->path, subpool);
            }

            switch (n->action) {
                case svn_wc_notify_skip:
                    if ((err = svn_cmdline_printf(subpool, "Skipped '%s'\n", path_local)))
                        goto print_error;
                    break;
                case svn_wc_notify_update_delete:
                    if ((err = svn_cmdline_printf(subpool, "D    %s\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_update_replace:
                    if ((err = svn_cmdline_printf(subpool, "R    %s\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_update_add:
                    if (n->content_state == svn_wc_notify_state_conflicted) {
                        if ((err = svn_cmdline_printf(subpool, "C    %s\n", path_local)))
                            goto print_error;
                    } else {
                        if ((err = svn_cmdline_printf(subpool, "A    %s\n", path_local)))
                            goto print_error;
                    }
                    break;

                case svn_wc_notify_exists:
                    if (n->content_state == svn_wc_notify_state_conflicted)
                        statchar_buf[0] = 'C';
                    else
                        statchar_buf[0] = 'E';

                    if (n->prop_state == svn_wc_notify_state_conflicted)
                        statchar_buf[1] = 'C';
                    else if (n->prop_state == svn_wc_notify_state_merged)
                        statchar_buf[1] = 'G';

                    if ((err = svn_cmdline_printf(subpool, "%s %s\n", statchar_buf, path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_restore:
                    if ((err = svn_cmdline_printf(subpool, "Restored '%s'\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_revert:
                    if ((err = svn_cmdline_printf(subpool, "Reverted '%s'\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_failed_revert:
                    if ((err = svn_cmdline_printf(subpool, "Failed to revert '%s' -- try updating instead.\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_resolved:
                    if ((err = svn_cmdline_printf(subpool, "Resolved conflicted state of '%s'\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_add:
                    /* We *should* only get the MIME_TYPE if PATH is a file.  If we
                       do get it, and the mime-type is not textual, note that this
                       is a binary addition. */
                    if (n->mime_type && (svn_mime_type_is_binary(n->mime_type))) {
                        if ((err = svn_cmdline_printf(subpool, "A  (bin)  %s\n", path_local)))
                            goto print_error;
                    } else {
                        if ((err = svn_cmdline_printf(subpool, "A         %s\n", path_local)))
                            goto print_error;
                    }
                    break;

                case svn_wc_notify_delete:
                    if ((err = svn_cmdline_printf(subpool, "D         %s\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_update_update:
                    {
                        if (n->content_state == svn_wc_notify_state_conflicted) {
                            statchar_buf[0] = 'C';
                        } else if (n->kind == svn_node_file) {
                            if (n->content_state == svn_wc_notify_state_merged)
                                statchar_buf[0] = 'G';
                            else if (n->content_state == svn_wc_notify_state_changed)
                                statchar_buf[0] = 'U';
                        }

                        if (n->prop_state == svn_wc_notify_state_conflicted)
                            statchar_buf[1] = 'C';
                        else if (n->prop_state == svn_wc_notify_state_merged)
                            statchar_buf[1] = 'G';
                        else if (n->prop_state == svn_wc_notify_state_changed)
                            statchar_buf[1] = 'U';

                        if (n->lock_state == svn_wc_notify_lock_state_unlocked)
                            statchar_buf[2] = 'B';

                        if (statchar_buf[0] != ' ' || statchar_buf[1] != ' ' || statchar_buf[2] != ' ') {
                            if ((err = svn_cmdline_printf(subpool, "%s %s\n", statchar_buf, path_local)))
                                goto print_error;
                        }
                    }
                    break;

                case svn_wc_notify_locked:
                    if ((err = svn_cmdline_printf(subpool, "'%s' locked by user '%s'.\n", path_local, n->lock->owner)))
                        goto print_error;
                    break;

                case svn_wc_notify_unlocked:
                    if ((err = svn_cmdline_printf(subpool, "'%s' unlocked.\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_failed_lock:
                case svn_wc_notify_failed_unlock:
                    svn_handle_warning2(stderr, n->err, "svn: ");
                    break;

                case svn_wc_notify_changelist_set:
                    if ((err = svn_cmdline_printf(subpool, "A [%s] %s\n", n->changelist_name, path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_changelist_clear:
                case svn_wc_notify_changelist_moved:
                    if ((err = svn_cmdline_printf(subpool, "D [%s] %s\n", n->changelist_name, path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_tree_conflict:
                    if ((err = svn_cmdline_printf(subpool, "   C %s\n", path_local)))
                        goto print_error;
                    break;

                case svn_wc_notify_property_modified:
                case svn_wc_notify_property_added:
                    err = svn_cmdline_printf(subpool, "property '%s' set on '%s'\n", n->prop_name, path_local);
                    if (err)
                        goto print_error;
                    break;

                case svn_wc_notify_property_deleted:
                    err = svn_cmdline_printf(subpool, "property '%s' deleted from '%s'.\n", n->prop_name, path_local);
                    if (err)
                        goto print_error;
                    break;

                case svn_wc_notify_property_deleted_nonexistent:
                    err = svn_cmdline_printf(subpool, "Attempting to delete nonexistent property '%s' on '%s'\n", n->prop_name, path_local);
                    if (err)
                        goto print_error;
                    break;

                case svn_wc_notify_revprop_set:
                    err = svn_cmdline_printf(subpool, "property '%s' set on repository revision %ld\n", n->prop_name, n->revision);
                    if (err)
                        goto print_error;
                    break;

                case svn_wc_notify_revprop_deleted:
                    err = svn_cmdline_printf(subpool, "property '%s' deleted from repository revision %ld\n", n->prop_name, n->revision);
                    if (err)
                        goto print_error;
                    break;

                default:
                    break;
            }

            if ((err = svn_cmdline_fflush(stdout)))
                goto print_error;

            return;

print_error:
            /* If we had no errors before, print this error to stderr. Else, don't print
               anything.  The user already knows there were some output errors,
               so there is no point in flooding her with an error per notification. */
            if (!nb->had_print_error) {
                nb->had_print_error = true;
                svn_handle_error2(err, stderr, FALSE, "svn: ");
            }
            svn_error_clear(err);
        }

    }
}


int svn::init(bool enabled) {
    internal::enabled = enabled;
    if (!internal::enabled) {
        return 0;
    }

    using internal::ctx;
    using internal::pool;

    svn_error_t *err;

    if (svn_cmdline_init("monad", stderr)) {
        return 1;
    }

    err = svn_ver_check_list(&internal::my_version, internal::lib_version);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

    pool = svn_pool_create_ex(NULL, NULL);

	err = svn_config_ensure(NULL, pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

	err = svn_client_create_context(&ctx, pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

    ctx->notify_func2 = internal::notify;
    ctx->notify_baton2 = new internal::notify_baton;

    err = svn_config_get_config(&ctx->config, NULL, pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

    svn_config_t *cfg_config = (svn_config_t*)apr_hash_get(ctx->config, SVN_CONFIG_CATEGORY_CONFIG, APR_HASH_KEY_STRING);

    err = svn_cmdline_create_auth_baton(&ctx->auth_baton,
            FALSE,     /* non-interactive */
            NULL,      /* username */
            NULL,      /* password */
            NULL,      /* config dir */
            FALSE,     /* no-auth-cache */
            FALSE,     /* trust-server-cert */
            cfg_config,
            NULL,      /* cancel-func */
            NULL,      /* cancel-baton */
            pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

    return 0;
}

int svn::checkout(std::string path) {
    if (!internal::enabled) {
        std::cerr << "svn disabled" << std::endl;
        return 1;
    }

    using internal::ctx;
    using internal::pool;

    svn_error_t *err;
    svn_opt_revision_t rev;
	svn_revnum_t svn_rev_output;
    const char *dest_dir, *cpath;

    cpath = svn_path_canonicalize(path.c_str(), pool);

    std::cout << "Checking out '" << cpath << "'..." << std::endl;

    err = svn_opt_parse_path(&rev, &dest_dir, cpath, pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

    dest_dir = svn_path_canonicalize(dest_dir, pool);
    dest_dir = svn_path_basename(dest_dir, pool);

    rev.kind = svn_opt_revision_head;

    err = svn_path_get_absolute(&((internal::notify_baton*)ctx->notify_baton2)->path_prefix, dest_dir, pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

	err = svn_client_checkout3(&svn_rev_output,
		cpath, dest_dir, &rev, &rev,
		svn_depth_infinity,
		TRUE, TRUE,
		ctx, pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

    return 0;
}

int svn::update(std::string path) {
    if (!internal::enabled) {
        std::cerr << "svn disabled" << std::endl;
        return 1;
    }

    using internal::ctx;
    using internal::pool;

    svn_error_t *err;
    svn_opt_revision_t rev;
    const char *cpath;
    apr_array_header_t *result_revs;
    apr_array_header_t *cpaths;

    cpath = svn_path_canonicalize(path.c_str(), pool);

    std::cout << "Updating '" << cpath << "'..." << std::endl;

    rev.kind = svn_opt_revision_head;

    ((internal::notify_baton*)ctx->notify_baton2)->path_prefix = NULL;

    cpaths = apr_array_make(pool, 1, sizeof(const char*));
    *(const char**)(apr_array_push(cpaths)) = cpath;

	err = svn_client_update3(&result_revs,
		cpaths, &rev,
		svn_depth_infinity,
		TRUE, TRUE, TRUE,
		ctx, pool);
    if (err) {
        internal::handle_error(err);
        return 1;
    }

    return 0;
}

