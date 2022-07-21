#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static char *filename = "/dev/chrdevbase";    /* 设备文件路径 */
static char *is_write = "-w";
static char *is_read  = "-r";

/**
 * @brief 打印用法
 * 
 */
void print_usage()
{
	printf("Usage:\n");
	printf("    wriet: chrdevbase_test -w <string>\n");
	printf("    read: chrdevbase_test -r\n");
}

int main(int argc, char *argv[])
{
	int fd;
	int len;
	int ret;
	char buf[1024];

	fd = open(filename, O_RDWR);
	if (fd == -1) {
		printf("can not open file: %s\n", filename);
		return -1;
	}
	printf("open file: %s\n", filename);

	/* 向设备写入数据 */
	if ((0 == strcmp(argv[1], is_write)) && (3 == argc)) {
		len = strlen(argv[2]) + 1;
		len = len < 1024 ? len : 1024;
		ret = write(fd, argv[2], len);
		printf("write driver: %d\n", ret);
	/* 从设备读取数据 */
	} else if ((0 == strcmp(argv[1], is_read)) && (2 == argc)){
		len = read(fd, buf, 1024);
		printf("read len: %d\n", len);
		buf[len - 1] = '\0';
		printf("read data: %s\n", buf);
	/* 参数错误，打印用法 */
	} else {
		print_usage();
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}
