#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int scratch_board_init(void){
    printk("Boot from Scratch Board!\n");

    return 0;
}

SYS_INIT(scratch_board_init, PRE_KERNEL_1, 50);