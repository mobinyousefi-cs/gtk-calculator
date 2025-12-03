# Graphical Calculator using C and GTK+

A fully functional **Graphical Calculator** implemented in **C** using the **GTK+ (GTK3)** framework. This project is part of *C Projects for Experienced*, designed to help developers strengthen their understanding of GUI programming, modular design, event-driven architectures, memory management, and algorithmic logic.

This calculator supports standard arithmetic expressions with **operator precedence**, **parentheses**, and **power operator (^)**, powered by a clean implementation of the **Shunting-Yard Algorithm**.

---

## Features

### Graphical User Interface (GTK+)

* Clean and intuitive GUI built using GTK+ widgets.
* Numeric and operator buttons laid out in a responsive 5×4 grid.
* Read‑only display field aligned to the right.
* Status bar for feedback (errors, execution success, cleared state, etc.).

### Functional Highlights

* Supports `+`, `-`, `*`, `/`, `^`, parentheses `()`, floating‑point numbers.
* Backspace, Clear Entry (CE), and Clear (C) functionality.
* Robust expression evaluation using a modular calculator engine.
* Detailed error messaging (syntax issues, division by zero, etc.).

### Engineering Aspects

* Clean modular code separation: `main`, `ui`, `calculator` modules.
* Headers included for full API visibility.
* No global state: uses a well‑structured `AppWidgets` context.
* Shunting‑yard algorithm for correct operator precedence.
* Makefile with `pkg-config` support for Linux GTK builds.

---

## Project Structure

```
gtk-calculator/
├─ Makefile
└─ src/
   ├─ main.c
   ├─ ui.h
   ├─ ui.c
   ├─ calculator.h
   └─ calculator.c
```

### File Responsibilities

| File           | Description                                                                  |
| -------------- | ---------------------------------------------------------------------------- |
| `main.c`       | Entry point, builds `GtkApplication` and window activation callback.         |
| `ui.h`         | Defines the `AppWidgets` structure and UI creation interface.                |
| `ui.c`         | Builds buttons, display, status bar, grid layout, and handles button events. |
| `calculator.h` | Declares public API for expression evaluation.                               |
| `calculator.c` | Implements the Shunting‑Yard algorithm and evaluation logic.                 |
| `Makefile`     | Build instructions using GCC and GTK+/pkg-config.                            |

---

## Build Instructions (Linux)

### 1. Install Dependencies

Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev
```

Fedora:

```bash
sudo dnf install gcc make gtk3-devel
```

Arch Linux:

```bash
sudo pacman -S base-devel gtk3
```

### 2. Build the Project

```bash
make
```

This produces the executable:

```
./gtk_calculator
```

### 3. Clean Build Files

```bash
make clean
```

---

## Running the Application

After building:

```bash
./gtk_calculator
```

The graphical calculator window will appear with:

* Interactive button grid
* Display for numbers & expressions
* Status bar for real-time feedback

---

## Core Algorithm: Shunting-Yard

This calculator uses **Dijkstra's Shunting‑Yard Algorithm** to parse expressions. It supports:

* Operator precedence
* Right‑associativity for `^`
* Floating-point parsing using `strtod`
* Parentheses grouping

Error handling includes:

* Invalid characters
* Mismatched parentheses
* Division by zero
* General syntax errors

The evaluator produces a result or fills a `CalcError` struct.

---

## Screenshots (Optional)

If you want, I can generate example GTK+ layout mockups or ASCII previews.

---

## License

This project is released under the **MIT License**.

---

## Author

**Mobin Yousefi**
GitHub: [github.com/mobinyousefi-cs](https://github.com/mobinyousefi-cs)

---

## Future Improvements

Possible expansions:

* Scientific functions: sin, cos, tan, log, ln
* GTK CSS theming (dark/light mode)
* Expression history panel
* Keyboard input support
* Plugin system for extendable operations

---

If you want, I can generate:

* A LICENSE file
* A `.gitignore`
* A `screenshots/` folder with mockups
* A Windows build guide (MSYS2 + GTK)
* A Flatpak or AppImage packaging script

Just tell me and I will add them. Ready for the next project whenever you are.
