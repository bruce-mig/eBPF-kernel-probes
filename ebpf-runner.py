#!/usr/bin/env python3

from bcc import BPF
from pathlib import Path

def process_clone_event(cpu, data, size):
      event = bpf["clone_events"].event(data)  # Define the event structure
      print(f"Process {event.comm.decode()} (PID: {event.pid}, PPID: {event.ppid}) called sys_clone")

def process_open_event(cpu, data, size):
      event = bpf["open_events"].event(data)  # Define the event structure
      print(f"[{event.timestamp / 1e9:.6f}] Process {event.comm.decode()} (PID: {event.pid}) called sys_open with filename: {event.filename.decode()}")

bpf_source = Path('ebpf-probe.c').read_text()
bpf = BPF(text=bpf_source)

bpf["clone_events"].open_perf_buffer(process_clone_event)
bpf["open_events"].open_perf_buffer(process_open_event)
print("Monitoryng sys_clone calls... Press Ctrl+C to exit.")

while True:
    try:
        bpf.perf_buffer_poll()  # Poll for events  
    except KeyboardInterrupt:
        break