#!/usr/bin/env python3

from bcc import BPF
from pathlib import Path

def process_event(cpu, data, size):
      event = bpf["events"].event(data)  # Define the event structure
      print(f"Process {event.comm.decode()} (PID: {event.pid}, PPID: {event.ppid}) called sys_clone")

bpf_source = Path('ebpf-probe.c').read_text()
bpf = BPF(text=bpf_source)

bpf["events"].open_perf_buffer(process_event)
print("Monitoryng sys_clone calls... Press Ctrl+C to exit.")

while True:
    try:
        bpf.perf_buffer_poll()  # Poll for events
    except KeyboardInterrupt:
        break