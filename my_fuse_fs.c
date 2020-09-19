/* Implementation of a file system in user space 
 * using the FUSE library */

#define _GNU_SOURCE
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <errno.h>

#define BLOCK_SIZE 	512
#define MAX_FILES 	4096
#define MAX_BLOCKS	256
#define DIRECT_BLOCKS	16

typedef struct superblock 
{

	char	s_datablock[BLOCK_SIZE * MAX_BLOCKS];
	int	s_inode[MAX_FILES];
	int	s_free_blocks;
	time_t	s_mount_time;
		
} superblock;

typedef struct inode {

	int 	i_mode;
	int 	i_nlink;
	int 	i_atime;
	int 	t_mtime;
	int 	i_ctime;
	int 	i_uid;
	int	i_gid;
	int 	i_size;
	int 	i_blocks;
	int 	a_addr[DIRECT_BLOCKS];

} inode;

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
static int fs_getattr(const char *path, struct stat *fs_stat);

static const struct fuse_operations fs_operations = {
	.init		= fs_init,
	.open		= fs_open,
	.read		= fs_read,
	getattr		= fs_getattr,
};

int main(int argc, char *argv[])
{
	int ret;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	ret = fuse_main(args.argc, args.argv, &fs_operations, NULL);
	fuse_opt_free_args(&args);

	return ret;
}
