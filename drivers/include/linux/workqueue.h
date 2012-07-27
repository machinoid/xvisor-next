#ifndef _LINUX_WORKQUEUE_H_
#define _LINUX_WORKQUEUE_H_

#include <vmm_workqueue.h>

#define work_struct		vmm_work
#define workqueue		vmm_workqueue

#define system_long_wq		NULL

#define queue_work(a, b)	vmm_workqueue_schedule_work(a, b)
#define cancel_work_sync(a)	vmm_workqueue_stop_work(a)

#endif /* _LINUX_WORKQUEUE_H_ */
