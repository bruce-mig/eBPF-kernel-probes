#include <bcc/proto.h>
#include <linux/sched.h>
#include <uapi/linux/ptrace.h>

struct data_t {
   u32 pid;                  // process id
   u32 ppid;                 // parent process id
   char comm[TASK_COMM_LEN]; // process name
};

BPF_PERF_OUTPUT(events); // define a perf event output buffer

int kprobe__sys_clone(void *ctx) {
   struct data_t data = {};
   struct task_struct *task;

   task = (struct task_struct *)bpf_get_current_task();
   data.pid = bpf_get_current_pid_tgid();
   data.ppid = task->real_parent->tgid;
   bpf_get_current_comm(&data.comm, sizeof(data.comm)); // get process name

   events.perf_submit(ctx, &data, sizeof(data)); // submit event to user space
   return 0;
}