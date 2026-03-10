# Linux Device Drivers

This repository contains various Linux kernel module drivers.

## Repository Structure

- `drivers/`: Contains individual kernel modules, each in its own subdirectory.
  - `01-helloworld/`: A basic "Hello World" kernel module.
  - `02-pseudo-char-driver/`: A pseudo character driver with basic file operations and mutex protection.

## Building the Drivers

### Prerequisites

- Linux Kernel Headers
- Build tools (`gcc`, `make`, etc.)
- For target-specific compilation, you'll need the appropriate cross-compiler (`arm-linux-gnueabihf-` for default target).

### Commands

To build for the **target architecture** (configured in each driver's Makefile):
```bash
make
```

To build for the **host machine**:
```bash
make host
```

To **clean** all build artifacts:
```bash
make clean
```
