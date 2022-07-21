#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define CHRDEVBASE_MAJOR 200    /* 主设备号 */
#define CHRDEVBASE_NAME "chrdevbase"    /* 设备名 */

#define BUF_SIZE 128    /* 缓存区大小 */

/**
 * @brief 设备结构体，用于存储设备缓存数据
 * 
 */
struct chrdevbase_data {
	char *buf;
	int len;
};

static struct chrdevbase_data kdata;

/**
 * @brief 打开设备
 * 
 * @param inode 设备 inode
 * @param filp 设备文件
 * @return int 返回 0 成功，返回其他失败
 */
static int chrdevbase_open(struct inode *inode, struct file *filp)
{
	pr_info("%s entry!\n", __FUNCTION__);

	return 0;
}

/**
 * @brief 从设备读取数据
 * 
 * @param filp 设备文件
 * @param buf 返回给用户空间的数据缓冲区
 * @param size 要读取的数据长度
 * @param offset 相对于文件首地址的偏移
 * @return ssize_t 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t chrdevbase_read(struct file *filp, char __user *buf,
						size_t size, loff_t *offset)
{
	int len = (kdata.len < size ? kdata.len : size);

	pr_info("%s entry!\n", __FUNCTION__);

	if (copy_to_user(buf, kdata.buf, len)) {
		pr_err("copy_to_user error!\n");
		return -ENOMEM;
	}

	return len;
}

/**
 * @brief 向设备写数据 
 * 
 * @param filp 设备文件
 * @param buf 要写给设备写入的数据
 * @param size 要写入的数据长度
 * @param offset 相对于文件首地址的偏移
 * @return ssize_t 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t chrdevbase_write(struct file *filp, const char __user *buf,
						size_t size, loff_t *offset)
{
	int len = (size < BUF_SIZE ? size : BUF_SIZE);

	pr_info("%s entry!\n", __FUNCTION__);

	if (copy_from_user(kdata.buf, buf, len)) {
		pr_err("copy_from_user error!\n");
		return -ENOMEM;
	}

	kdata.len = len;

	return len;

}

/**
 * @brief 关闭/释放设备
 * 
 * @param inode 设备 inode
 * @param filp 设备文件
 * @return int 返回 0 成功，返回其他失败
 */
static int chrdevbase_release(struct inode *inode, struct file *filp)
{
	pr_info("%s entry!\n", __FUNCTION__);

	return 0;
}

/**
 * @brief 设备操作函数结构体
 * 
 */
static struct file_operations chrdevbase_ops = {
	.owner = THIS_MODULE,
	.open = chrdevbase_open,
	.read = chrdevbase_read,
	.write = chrdevbase_write,
	.release = chrdevbase_release,
};

/**
 * @brief 驱动入口函数
 * __init 段属性。驱动初始化函数只用一次，用完就可以释放了。
 * 
 * @return int 返回 0 成功，返回其他失败
 */
int __init chrdevbase_init(void)
{
	int ret = 0;

	/* 分配内存 */
	kdata.buf = (char *)kmalloc(BUF_SIZE, GFP_KERNEL);
	if (NULL == kdata.buf) {
		pr_err("kmalloc buf failed!\n");
		return -ENOMEM;
	}
	kdata.len = 0;

	/* 注册驱动 */
	ret = register_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME, &chrdevbase_ops);
	if (ret < 0) {
		pr_err("%s register failed\n", CHRDEVBASE_NAME);
		return -1;
	}
	pr_info("%s major: %d\n", CHRDEVBASE_NAME, CHRDEVBASE_MAJOR);

	return 0;
}

/**
 * @brief 驱动出口函数
 * __exit 段属性。如果驱动 build_in，则无需释放，exit 函数自然也不用编译。
 */
void __exit chrdevbase_exit(void)
{
	unregister_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME);
	pr_info("hello_drv exit\n");

	if (NULL != kdata.buf) {
		kfree(kdata.buf);
		pr_info("free buff ok!\n");
	}
}

module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");
