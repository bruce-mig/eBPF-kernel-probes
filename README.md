# eBPF Watcher Project -- Linux Observability

A  project using BPF Compiler Collection (BCC) to monitor and trace system events using eBPF.

This Python script and eBPF program work together to monitor the occurrences of sys_clone and sys_openat system calls. 

The script uses the Prometheus Python client to expose counters for these calls as metrics, and starts an HTTP server on port 3000 to serve these metrics. 

The eBPF program hooks into the kernel's sys_clone and sys_openat2 functions using kprobes, collecting information about the process making the calls and the files being opened. 

The script processes these events, printing the process information and incrementing the Prometheus counters. 

The script runs continuously, polling the eBPF program for new events until interrupted by the user with Ctrl+C.

## Prerequisites

- Python 3.7+
- `uv` package manager
- BCC system dependencies (see Installation section)

## Installation

### 1. Install UV

**Windows:**
```powershell
powershell -c "irm https://astral.sh/uv/install.ps1 | iex"
```

**macOS/Linux:**
```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
```

### 2. Install BCC System Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y bpfcc-tools libbpfcc python3-bpfcc
```

**Fedora/RHEL:**
```bash
sudo dnf install -y bcc-tools python3-bcc
```

**macOS:**
```bash
# BCC is not officially supported on macOS
# Consider using Docker or a Linux VM
```

## Setup

### Clone the Repository

```bash
git clone https://github.com/bruce-mig/eBPF-kernel-probes.git
cd eBPF-kernel-probes
```

### Create Virtual Environment

```bash
# Create virtual environment named 'ebpf-venv'
uv venv ebpf-venv
```

### Activate Virtual Environment

**Windows:**
```powershell
ebpf-venv\Scripts\activate
```

**macOS/Linux:**
```bash
source ebpf-venv/bin/activate
```

You should see `(ebpf-venv)` appear at the start of your terminal prompt.

### Install Dependencies

```bash
uv pip install -r requirements.txt
```

## Usage

```bash
# Make sure your virtual environment is activated
# You should see (ebpf-venv) in your prompt

# Run the eBPF watcher
sudo python main.py
```

**Note:** eBPF programs typically require root privileges to interact with the kernel.

## Development

### Installing New Packages

```bash
# Install a new package
uv pip install package-name

# Update requirements.txt
uv pip freeze > requirements.txt
```

### Checking Installed Packages

```bash
uv pip list
```

## Deactivating Virtual Environment

When you're done working on the project:

```bash
deactivate
```

This will return you to your system Python environment. The `(ebpf-venv)` prefix will disappear from your prompt.

## Project Structure

```
ebpf-project/
├── ebpf-venv/           # Virtual environment (not in git)
├── .gitignore           # Git ignore file
├── requirements.txt     # Python dependencies
├── ebpf-watcher.c      # eBPF C program
├── main.py             # Python script
└── README.md           # This file
```

## Troubleshooting

### Permission Denied
If you get permission errors when running the script:
```bash
sudo python main.py
```

### Module Not Found
Make sure your virtual environment is activated:
```bash
source ebpf-venv/bin/activate  # macOS/Linux
# or
ebpf-venv\Scripts\activate     # Windows
```

### BCC Installation Issues
Ensure kernel headers are installed:
```bash
# Ubuntu/Debian
sudo apt-get install linux-headers-$(uname -r)

# Fedora/RHEL
sudo dnf install kernel-devel-$(uname -r)
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

[MIT](LICENSE)

## Resources

- [BCC Documentation](https://github.com/iovisor/bcc)
- [eBPF Documentation](https://ebpf.io/)
- [UV Documentation](https://github.com/astral-sh/uv)