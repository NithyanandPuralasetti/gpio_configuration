# GPIO Pin Configuration & Hardware Register Simulator in C

A modular, terminal-based simulation written in C that models microcontroller GPIO peripheral configuration, bit-level register synchronization, and firmware safety diagnostics.

---

## 📌 Project Overview

In embedded systems, software drives hardware through bit-mapped memory registers rather than basic high-level variables. 

This project simulates:
* How individual pin states map directly to an 8-bit hardware register (`PORT_REGISTER`).
* How bitwise operations modify specific pins without affecting adjacent bits.
* Practical embedded safety guardrails like floating input detection and illegal write prevention.

---

## ✨ Key Features

* **Hardware Register Synchronization:** Synchronizes pin states with a virtual 8-bit hardware `PORT` register using bitwise operations (`|`, `&`, `~`, `<<`).
* **Memory-Optimized Structures:** Implements C bit-fields (`unsigned int : 1`) inside `struct` definitions to minimize RAM footprint.
* **Firmware Safety Interlocks:**
  * Rejects attempts to write digital output states to pins set as `INPUT`.
  * Automatically pulls pin state to `LOW` before mode switches to avoid hardware faults.
* **Diagnostic Scanner:** Detects high-impedance **floating pins** (Input pins with internal pull-up resistors disabled).
* **Interrupt Simulation:** Emulates transition detection when an output pin flips between logic levels.

---

## 📁 Repository Structure

* `gpio.h` — Peripheral definitions, bit-field structures, enums, and prototypes
* `gpio.c` — Register synchronization logic, driver functions, and diagnostics
* `main.c` — Interactive CLI menu and input validation engine
* `README.md` — Project documentation

---

## 🛠️ Build and Execution

### Prerequisites
* GCC Compiler (`gcc`)
* Linux, macOS, or WSL on Windows

### Compilation
Compile the source files using:

```bash
gcc main.c gpio.c -o gpio_sim
./gpio_sim
```
or
```bash
gcc *.c
./a.out
```
## Sample output

```text
+===============================+
|       Configuration MENU      |
+===============================+
|1.Configure GPIO pin           |
|2.Display ALL pins             |
|3.Search GPIO pin              |
|4.Change Pin mode              |
|5.Set Pin value                |
|6.Enable/Disable Pullup        |
|7.Fault Detection              |
|8.Exit                         |
+===============================+
Enter your choice : 1
Enter the GPIO Pin number: 2
Enter the GPIO Pin Mode (0 = Input 1 = Output): 1
Enter the GPIO Pin Value (0 = Low 1 = High): 1
Enter the Pullup (0 = Disabled 1 = Enabled): 0

HARDWARE REGISTER (PORT): 0x04  [Binary: 00000100]
[SUCCESS] Pin 2 configured successfully!
```
