# MeshFix V2.1 (Modernized Port)

> **Disclaimer:** This project is a modernized port of the original **MeshFix** developed by **Marco Attene**. I take no credit for the original algorithms, research, or core logic. This version is intended to provide a cleaner, more modern interface for macOS and Linux users. The original source code and research remain the intellectual property of the original author and IMATI-GE / CNR.

**MeshFix** is a lightweight, self-contained tool designed to repair digitized polygon meshes. It automatically corrects common defects in raw digitized models, such as singularities, self-intersections, and degenerate elements, producing a single watertight manifold triangle mesh.

Original Author: **Marco Attene** at IMATI-GE / CNR.

---

## Modifications in this Port

This version introduces several changes to the original 2016 codebase to improve usability and maintainability on modern systems:

1.  **Modern C++20 Standards:**
    *   Updated the build system to require C++20.
    *   Implemented the **spaceship operator (`<=>`)** for the `Point` class to simplify and modernize comparisons.
    *   Replaced raw string comparisons with **`std::string_view`** for more efficient argument parsing.
    *   Introduced **`std::span`** for safer orientation predicate interfaces.
2.  **Enhanced Command-Line Interface:**
    *   Added a `-h` flag to display detailed usage and help information.
    *   Added an explicit `-o <filename>` flag to specify the output path (while maintaining backward compatibility for positional arguments).
3.  **Removed Windows Dependency:**
    *   Deleted all `vc12` Visual Studio project files and `.exe` binaries.
    *   Removed MSVC-specific macros and compiler guards to focus on a clean macOS/Linux experience.
4.  **Improved Build & Infrastructure:**
    *   Streamlined `CMakeLists.txt` for modern compilers.
    *   Added a `.gitignore` to keep the repository clean of build artifacts.
    *   Added a professional `README.md` (this file).
5.  **New Testing Suite:**
    *   Added a **Python integration test suite** (`tests/test_cli.py`) to verify CLI behavior.
    *   Added a **C++ unit test suite** (`tests/unit_tests.cpp`) to validate core geometry logic.

---

## Errors Fixed by MeshFix

MeshFix is optimized for **raw digitized meshes** (e.g., from 3D scans) and addresses several categories of geometric and topological flaws:

### 1. Degenerate Elements
*   **Needles:** Triangles with one extremely short edge (nearly coincident vertices).
*   **Caps:** "Flat" triangles where one vertex lies on or near the opposite edge.
*   **Coincident Vertices:** Different vertex entries sharing the exact same spatial coordinates.

### 2. Connectivity & Topological Errors
*   **Singularities:** Non-manifold links where surfaces meet at a single point or edge incorrectly.
*   **Inconsistent Normals:** Automatically re-orients triangles to ensure a consistent "inside" vs. "outside."
*   **Noise Removal:** Removes isolated small components (scanning artifacts), keeping only the largest connected object.

### 3. Geometric Flaws
*   **Self-Intersections:** Detects and resolves cases where the mesh surface passes through itself.
*   **Overlapping Triangles:** Identifies and eliminates redundant triangles occupying the same space.

### 4. Holes and Boundaries
*   **Hole Patching:** Identifies boundary loops and fills them to make the model **watertight**.
*   **Component Joining:** Bridges gaps between separate components when their boundaries are sufficiently close.

## Example Dataset

The `dataset/` directory contains several minimal examples of mesh errors used for testing and demonstration:

*   `hole.off`: A cube with one missing face (hole).
*   `degenerate_needle.off`: A triangle with nearly coincident vertices.
*   `degenerate_cap.off`: A flat triangle (vertex on opposite edge).
*   `normals.off`: Two triangles with inconsistent orientations.
*   `noise.off`: A cube with an isolated tiny component.
*   `intersect.off`: Two self-intersecting triangles.

To test MeshFix on these examples, you can run:
```bash
python3 tests/test_dataset.py
```

## Supported Formats

MeshFix supports a wide range of industry-standard 3D formats for both input and output:

| Format | Extension | Support |
| :--- | :--- | :--- |
| **Object File Format** | `.off` | Full |
| **Polygon File Format** | `.ply` | Full |
| **STereoLithography** | `.stl` | Full |
| **Wavefront OBJ** | `.obj` | Full |
| **VRML 1.0 / 2.0** | `.wrl` | Full |
| **Open Inventor** | `.iv` | Full |
| **Evolutionary File Format** | `.eff` | Full |
| **Ver-Tri** | `.tri` | Partial |

---

## Installation

### Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

The executable will be generated in the `bin64/` (or `bin/`) directory.

---

## Usage

```bash
MeshFix <inmeshfile> [outmeshfile] [options]
```

### Options

| Option | Description |
| :--- | :--- |
| `-h` | Display help and usage information. |
| `-o <file>` | Explicitly specify the output filename. |
| `-a` | Join multiple open components before starting the repair process. |
| `-j` | Force output to STL format (default is OFF). |
| `-x` | Exit immediately if the output file already exists (useful for batch scripts). |

### Examples

**Basic Repair:**
```bash
MeshFix model.obj
# Produces model_fixed.off
```

**Specify Output File:**
```bash
MeshFix model.ply -o cleaned_model.stl
```

**Join Components and Force STL Output:**
```bash
MeshFix scan.off -a -j
# Produces scan_fixed.stl
```

---

## Citation Policy

If you use MeshFix for research purposes, please cite the following paper:

> **M. Attene.**  
> *A lightweight approach to repairing digitized polygon meshes.*  
> **The Visual Computer**, 2010. (c) Springer.  
> DOI: [10.1007/s00371-010-0416-3](https://doi.org/10.1007/s00371-010-0416-3)

---

## License

MeshFix is **Copyright (C) 2010: IMATI-GE / CNR**.

It is dual-licensed:
1. **GPL v3.0+**: For free/open-source use. See `gpl-3.0.txt` for details.
2. **Commercial License**: For integration into commercial software. Please contact the authors at `attene@ge.imati.cnr.it` for licensing terms.

---

*Disclaimer: MeshFix is optimized for raw digitized meshes. It may produce coarse results or fail on other types of input, such as tessellated CAD models.*
