# MeshFix V2.1

**MeshFix** is a lightweight, self-contained tool designed to repair digitized polygon meshes. It automatically corrects common defects in raw digitized models, such as singularities, self-intersections, and degenerate elements, producing a single watertight manifold triangle mesh.

Developed by **Marco Attene** at IMATI-GE / CNR.

---

## Features

- **Automated Repair:** Fixes holes, self-intersections, and degeneracies.
- **Watertight Output:** Ensures the resulting mesh represents a single closed solid object.
- **Component Joining:** Optional joining of close open components.
- **High Portability:** Standard ANSI C++ with no external dependencies.
- **Command-Line Interface:** Easy to integrate into automated scripts and pipelines.

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
