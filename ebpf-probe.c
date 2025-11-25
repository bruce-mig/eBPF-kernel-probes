#include <bcc/proto.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <uapi/linux/ptrace.h>

struct clone_data_t {
   u32 pid;                  // process id
   u32 ppid;                 // parent process id
   char comm[TASK_COMM_LEN]; // process name
};

struct open_data_t {
   u32 pid;
   u64 timestamp;
   char comm[TASK_COMM_LEN];
   char filename[NAME_MAX];
};

BPF_PERF_OUTPUT(clone_events); // define a perf output for clone events
BPF_PERF_OUTPUT(open_events);  // define a perf output for open events

int kprobe__sys_clone(void *ctx) {
   struct clone_data_t data = {};
   struct task_struct *task;

   task = (struct task_struct *)bpf_get_current_task();
   data.pid = bpf_get_current_pid_tgid();
   data.ppid = task->real_parent->tgid;
   bpf_get_current_comm(&data.comm, sizeof(data.comm)); // get process name

   clone_events.perf_submit(ctx, &data,
                            sizeof(data)); // submit event to user space
   return 0;
}

int kprobe__do_sys_openat2(struct pt_regs *ctx, int dfd,
                           const char __user *filename, int flags,
                           umode_t mode) {
   struct open_data_t data = {};

   data.pid = bpf_get_current_pid_tgid();
   data.timestamp = bpf_ktime_get_ns();
   bpf_get_current_comm(&data.comm, sizeof(data.comm));

   bpf_probe_read_user_str(&data.filename, sizeof(data.filename),
                           filename); // read filename from user space

   open_events.perf_submit(ctx, &data,
                           sizeof(data)); // submit event to user space
   return 0;
}