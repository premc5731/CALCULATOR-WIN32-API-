# Calculator (Win32 API)

A **simple, modern-looking calculator** application built entirely with **C** and the **Win32 API**, without any frameworks like .NET, MFC, or external libraries. This project demonstrates how to create native Windows applications with custom styling using the **Graphics Device Interface (GDI)** and direct control creation.

---

## Features

- **Standard Arithmetic Operations**:
  - Addition (`+`)
  - Subtraction (`-`)
  - Multiplication (`*`)
  - Division (`/`)
  - Modulus (`%`)

- **Smart Input Handling**:
  - Prevents leading zeros
  - Handles operator chaining like `12 + 3`
  - Prevents divide/modulus by zero

- **Lightweight & Fast**:
  - Compiled directly to a small `.exe` file
  - No external dependencies

---

## Build Instructions

### Prerequisites

- Windows OS
- C Compiler like:
  - [MinGW-w64](https://www.mingw-w64.org/)
  - [MSVC (Microsoft C Compiler)](https://visualstudio.microsoft.com/)

### Compile using MinGW:

Open the command prompt and run:

```bash
gcc calculator.c -o calculator.exe -lgdi32
```

This will generate a `calculator.exe` in the same directory.

---

## Screenshot

<img src="Screenshot\Screenshot 2025-08-06 222705.png" alt="Calculator Screenshot" width="300" height="400">


---

---

## Win32 Concepts Used

- `CreateWindowW`, `WNDCLASSW`, `WinMain`
- Message loop: `GetMessage`, `DispatchMessage`
- Window procedure: `WindowProcedure`
- Control creation: `Edit`, `Button`
- Font customization: `CreateFontW`
- Brush and color styling with `WM_CTLCOLOR*`
- Arithmetic parsing with `swscanf` and `swprintf`


