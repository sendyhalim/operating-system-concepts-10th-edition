#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/module.h>

int simple_init(void) {
  printk(KERN_INFO, "Loading simple kernel module\n");

  return 0;
}

void simple_exit(void) {
  printk(KERN_INFO, "Removing simple kernel module\n");
}

module_init(simple_init);
module_exit(simple_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple module");
MODULE_AUTHOR("Test author");
