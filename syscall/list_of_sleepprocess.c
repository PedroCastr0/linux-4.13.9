#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include "list_of_sleepprocess.h"
  
asmlinkage long sys_list_of_sleepprocess(const char __user *buf, int size) {
	struct task_struct *proces;
	unsigned char kbuf[256];
	int bufsz;
	int ret;

	/* Find the process */
	for_each_process(proces) {
		if((long)proces->state == 0x0001){ // se processo esta dormindo
			snprintf(kbuf, sizeof(kbuf), "Process: %s\n PID_Number: %ld\n Process State: %ld\n Priority: %ld\n RT_Priority: %ld\n Static Priority: %ld\n Normal Priority: %ld\n", 
					proces->comm, 
					(long)task_pid_nr(proces), 
					(long)proces->state, 
					(long)proces->prio, 
					(long)proces->rt_priority, 
					(long)proces->static_prio, (long)proces->normal_prio);
			bufsz = strlen(kbuf)+1;

			/* User buffer is too small */
			if(bufsz > size){
				return -1;
			}

			/* success */
			ret = copy_to_user((void*)buf, (void*)kbuf, bufsz);

			return bufsz - ret;
		}
	}

	/* Process not found */
	return -2;	
}
