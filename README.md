# Terminal Fractal Viewer

A very simple terminal based fractal viewer. 

<img width="304" height="170" alt="fractal-viewer-preview" src="https://github.com/user-attachments/assets/62e82417-4514-429b-b375-400b5f223eff" />

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

This project would likely work on Windows by using the PDCurses library instead of `ncurses`, however this is untested as of now.

PDCurses: https://pdcurses.org/

You will probably also need a compatible compiler such as:

- MinGW GCC
- MSYS2
- Visual Studio (with appropriate curses setup)

---

## Controls

| Key | Action |
|---|---|
| `w` | Move up |
| `a` | Move left |
| `s` | Move down |
| `d` | Move right |
| `b` | Zoom in |
| `v` | Zoom out |
| `x` | Quit |

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
