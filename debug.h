//
// Created by user on 10/11/2020.
//

#ifndef _DEBUG_H_
#define _DEBUG_H_

/// @brief Debug printing

#include <asm/current.h>	// struct task_struct *current
#include <linux/printk.h>
#include <linux/sched.h>	// struct task_struct
#include <linux/string.h>	// strrchr()


#define xPRINTF(PRINTK_TAG, prefix, format, args...) do { \
        const char *f = __FILE__; \
        const char *n = strrchr(f, '/'); \
        printk(PRINTK_TAG "| %5u:%5u:%s:%u:%s|process is \"%s\" |" prefix format "\n", \
		(unsigned)current->tgid, \
		(unsigned)current->pid, \
		(n) ? n+1 : f, __LINE__, __FUNCTION__, current->comm, ##args); \
    } while(0)

// in descending order of importance
#define FPRINTF(format, args...) xPRINTF(KERN_EMERG, "EMERGENCY: ", format, ##args)
#define APRINTF(format, args...) xPRINTF(KERN_ALERT,     "ALERT: ", format, ##args)
#define CPRINTF(format, args...) xPRINTF(KERN_CRIT,   "CRITICAL: ", format, ##args)
#define EPRINTF(format, args...) xPRINTF(KERN_ERR,       "ERROR: ", format, ##args)
#define WPRINTF(format, args...) xPRINTF(KERN_WARNING, "WARNING: ", format, ##args)
#define NPRINTF(format, args...) xPRINTF(KERN_NOTICE,   "NOTICE: ", format, ##args)
#define IPRINTF(format, args...) xPRINTF(KERN_INFO,       "INFO: ", format, ##args)
#ifdef __DEBUG__
#define DPRINTF(format, args...) xPRINTF(KERN_DEBUG,      "DEBUG ", format, ##args)
#else
#define DPRINTF(format, args...)
#endif

#define HEX_DUMP(PRINTK_TAG, dump_prefix, addr, size) \
	print_hex_dump(PRINTK_TAG "| ", dump_prefix, DUMP_PREFIX_OFFSET, \
		 16, 1, addr, size, true)

#endif //_DEBUG_H_
