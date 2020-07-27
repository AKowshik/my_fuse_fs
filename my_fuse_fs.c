/* Implementation of a file system in user space 
 * using the FUSE library */

#define _GNU_SOURCE
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <errno.h>

static void *fs_init(struct fuse_conn_info *conn,
                        struct fuse_config *cfg)
{
        (void) conn;
        cfg->kernel_cache = 1;
        return NULL;
}

static int fs_open(const char *path, struct fuse_file_info *fi);
static int fs_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi);

static const struct fuse_operations fs_operations = {
        .init           = fs_init,
        .open           = fs_open,
        .read           = fs_read,
};

int main(int argc, char *argv[])
{
        int ret;
        struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

        ret = fuse_main(args.argc, args.argv, &fs_operations, NULL);
        fuse_opt_free_args(&args);

        return ret;
}