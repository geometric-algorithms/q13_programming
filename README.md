# Dynamic Delaunay Triangulation and Voronoi Diagram Construction

## Contributors
- **Santhosh** – Roll No: `210050131`
- **Prahasith Pasnoor** – Roll No: `210050114`

---

## Project Overview

### A Part: Static Delaunay Triangulation and Voronoi Diagram Construction
- **Goal**: 
  - Compute the **Delaunay triangulation** of a static set of points using **Fortune's Sweep Line Algorithm**.
  - Construct the **Voronoi diagram** as the dual of the triangulation.
- **Time Complexity**:  
  \( O(n \log n) \) expected time.

---

### B Part: Dynamic Delaunay Triangulation (Insertions Only)
- **Goal**:
  - Maintain the Delaunay triangulation under **dynamic point insertions** using a **Delaunay Tree**.
- **Time Complexity**:  
  - **Insertion**: \( O(\log n) \) expected time.
  - (**Deletion is not implemented**.)

---

## How to Build and Run

Both parts have their own `test.sh` script to automate compilation and execution.

---

### Part A: Fortune's Algorithm + Voronoi Diagram

#### Steps to Run:
```bash
cd part_A
bash test.sh
```

#### What happens in the script:
- Compiles the C++ code (`main.cpp`, `fortune.h`, `arc.h`, `event.h`, `point.h`).
- Runs the executable `./main` on `input.txt`.
- Executes `plot_voronoi.py` to generate the Voronoi diagram plot.

---

### Part B: Delaunay Tree for Dynamic Delaunay Triangulation

#### Steps to Run:
```bash
cd part_B
bash test.sh
```

#### What happens in the script:
- Compiles the C++ code (`main.cpp`, `point.h`).
- Runs `./main`, taking input points from `input.txt` and saving output to `output.txt`.
- Executes `visualize.py` to display the Delaunay triangulation.

---

## Requirements

- **C++ Compiler**:
```bash
g++ --version
```

- **Python 3**:
```bash
pip install matplotlib numpy
```

---

## Important Notes
- Input points must be **non-degenerate** (no three points collinear, no four points cocircular).
- All computations use `long double` for better floating-point accuracy.
- Scripts (`test.sh`) will terminate immediately on any error (`|| exit 1`).

