#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int my_init(void) {
    printk(KERN_ALERT "Initializing my module\n");
    return 0;
}

static void my_exit(void) {
    printk(KERN_ALERT "Exiting my module\n");
}

module_init(my_init);
module_exit(my_exit);