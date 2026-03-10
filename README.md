# Linux Device Drivers

This repository contains various Linux kernel module drivers.

## Repository Structure

- `drivers/`: Contains individual kernel modules, each in its own subdirectory.
  - `01-helloworld/`: A basic "Hello World" kernel module.
  - `02-pseudo-char-driver/`: A pseudo character driver with basic file operations and mutex protection.

## Learning Notes

### 1. UART (Universal Asynchronous Receiver-Transmitter)

UART is a **Serial** type of communication where data bits are sent one by one in series over a single wire.

**Key Characteristics:**
- **Asynchronous:** No shared clock signal between transmitter and receiver. They rely on a pre-configured speed called the **Baud Rate** (e.g., 9600, 115200) to decode the data.
- **Idle State:** The line is held **HIGH** (Logic 1) when no data is being transmitted.
- **Start Bit:** Always **LOW** (Logic 0). This transition from HIGH to LOW tells the receiver that a new data frame is starting.
- **Data Frame:** Usually 5 to 9 bits (standard is 8 bits).
- **Parity Bit (Optional):** Used for simple error checking (Even or Odd parity).
- **Stop Bit:** Always **HIGH** (Logic 1). It can be 1, 1.5, or 2 bits long, signaling the end of the frame and returning the line to the idle state.

**Related "Stuff" in UART:**
- **Full-Duplex:** Uses two separate wires (**TX** for Transmit and **RX** for Receive) so it can send and receive data at the same time.
- **Flow Control (Optional):** Uses extra wires like **RTS** (Request to Send) and **CTS** (Clear to Send) to prevent data loss if the receiver is busy.
- **Baud Rate Mismatch:** If the transmitter and receiver are set to different baud rates, the data will be received as "garbage" because the timing for sampling the bits will be wrong.

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
