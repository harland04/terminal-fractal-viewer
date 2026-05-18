# Terminal Fractal Viewer

A very simple terminal based fractal viewer.

---

## Dependencies

- `ncurses`
- `gcc` (or another compatible C/C++ compiler)
- `make` (optional)

---

## Installation

### Debian / Ubuntu

Install required packages with:

```bash
sudo apt install libncurses-dev libncursesw6 build-essential
```

---

### Build

If using the Makefile:

```bash
make
```

Otherwise, you can compile manually:

```bash
g++ main.cpp -lncurses -o fractal-viewer
```

---

## Windows Support

This project would likely work on Windows by using the PDCurses library instead of `ncurses`.

PDCurses: https://pdcurses.org/

You will probably also need a compatible compiler such as:

- MinGW GCC
- MSYS2
- Visual Studio (with appropriate curses setup)

---

## Controls

| Key | Action |
|---|---|
| `W` | Move up |
| `A` | Move left |
| `S` | Move down |
| `D` | Move right |
| `B` | Zoom in |
| `V` | Zoom out |

---

## Notes

- Zooming in too far may cause performance issues.
- Some terminals may display characters/shapes differently.

---

## Future Plans

- Heightmap/DEM rendering with expanded ASCII repertoire for enhanced visuals
- Optimization overhaul
- Color support
- Mouse controls
- New fractals (Burning ship, Julia set, etc.)
- Multithreaded rendering
- Custom iteration counts
- Saving screenshots to text/image files

---
