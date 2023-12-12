#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("gontardaniil <gontardaniil@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

// Оголосити структуру для елемента списку
struct hello_entry {
    struct list_head list;
    ktime_t time;
};

// Створити статичну змінну голови списку
static LIST_HEAD(hello_list);

// Оголосити параметр
static unsigned int amount = 1;
module_param(amount, uint, S_IRUGO);
MODULE_PARM_DESC(amount, "Number of times to print 'Hello, world!' (default=1)");

static int __init hello_init(void)
{
    if (amount == 0 || (amount > 5 && amount < 10)) {
        // Надрукувати попередження та продовжити роботу
        pr_warn("Print count is 0 or between 5 and 10. Defaulting to 1.\n");
    }
    BUG_ON(amount > 10);

    // Виділити пам'ять та додати елементи до списку відповідно до значень amount
    while (amount > 0) {
        struct hello_entry *entry;

        if (amount == 1) {
            entry = NULL;
        } else {
            entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        }


        // Зберегти поточний час ядра
        entry->time = ktime_get();

        // Додати елемент до списку
        list_add(&entry->list, &hello_list);

        // Надрукувати привітання
        pr_emerg("Hello, world!\n");
        
        amount--;
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct hello_entry *entry, *temp;

    // Пройти по списку
    list_for_each_entry_safe(entry, temp, &hello_list, list) {
        // Надрукувати час події в наносекундах
        pr_emerg("Time: %lld ns\n", ktime_to_ns(entry->time));

        // Вилучити елемент зі списку
        list_del(&entry->list);

        // Звільнити виділену пам'ять
        kfree(entry);
    }
}

module_init(hello_init);
module_exit(hello_exit);
