import sys
import random
import math

def read_off(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    # Filter out empty lines and strip whitespace
    lines = [line.strip() for line in lines if line.strip()]
    
    if not lines[0].startswith("OFF"):
        raise ValueError("Not a valid OFF file")
    
    if len(lines[0]) > 3:
        # OFF nv nt ne format
        header_data = lines[0][3:].split()
        nv = int(header_data[0])
        nt = int(header_data[1])
        data_start = 1
    else:
        header_data = lines[1].split()
        nv = int(header_data[0])
        nt = int(header_data[1])
        data_start = 2

    verts = []
    for i in range(data_start, data_start + nv):
        verts.append(list(map(float, lines[i].split())))
    
    faces = []
    for i in range(data_start + nv, data_start + nv + nt):
        parts = list(map(int, lines[i].split()))
        faces.append(parts[1:]) # Skip the '3' prefix
    
    return verts, faces

def corrupt_mesh(verts, faces):
    print("Applying all possible corruptions...")
    
    # 1. Create Holes (Remove 5% of faces)
    num_to_remove = int(len(faces) * 0.05)
    indices = random.sample(range(len(faces)), num_to_remove)
    faces = [faces[i] for i in range(len(faces)) if i not in indices]
    print(f" - Created {num_to_remove} holes.")

    # 2. Inconsistent Normals (Flip 5% of faces)
    num_to_flip = int(len(faces) * 0.05)
    flip_indices = random.sample(range(len(faces)), num_to_flip)
    for idx in flip_indices:
        faces[idx][1], faces[idx][2] = faces[idx][2], faces[idx][1]
    print(f" - Flipped {num_to_flip} face normals.")

    # 3. Degeneracies (Collapse edges in 5% of faces)
    num_to_degenerate = int(len(faces) * 0.05)
    deg_indices = random.sample(range(len(faces)), num_to_degenerate)
    for idx in deg_indices:
        v1, v2 = faces[idx][0], faces[idx][1]
        verts[v2] = list(verts[v1]) # Make vertices coincident
    print(f" - Created {num_to_degenerate} degenerate triangles (needles/caps).")

    # 4. Add Noise (Floating small components)
    for i in range(3):
        base_idx = len(verts)
        offset = 5.0 + i
        verts.append([offset, offset, offset])
        verts.append([offset+0.1, offset, offset])
        verts.append([offset, offset+0.1, offset])
        faces.append([base_idx, base_idx+1, base_idx+2])
    print(" - Added 3 small noise components.")

    # 5. Create Singularities (Bowtie vertex)
    # Pick a random vertex and connect a new triangle to it only
    if len(verts) > 0:
        target_v = random.randint(0, len(verts)-1)
        base_idx = len(verts)
        verts.append([verts[target_v][0]+1, verts[target_v][1], verts[target_v][2]])
        verts.append([verts[target_v][0], verts[target_v][1]+1, verts[target_v][2]])
        faces.append([target_v, base_idx, base_idx+1])
        print(f" - Created singularity at vertex {target_v}.")

    # 6. Self-Intersections (Overlap)
    # Take a face and duplicate it with a slight offset so they intersect
    if len(faces) > 0:
        f_idx = random.randint(0, len(faces)-1)
        f = faces[f_idx]
        base_idx = len(verts)
        for v_idx in f:
            v = verts[v_idx]
            verts.append([v[0], v[1], v[2] + 0.01]) # Slight Z offset
        faces.append([base_idx, base_idx+1, base_idx+2])
        print(" - Created overlapping/self-intersecting geometry.")

    return verts, faces

def save_off(filename, verts, faces):
    with open(filename, 'w') as f:
        f.write("OFF\n")
        f.write(f"{len(verts)} {len(faces)} 0\n")
        for v in verts:
            f.write(f"{v[0]} {v[1]} {v[2]}\n")
        for tri in faces:
            f.write(f"3 {tri[0]} {tri[1]} {tri[2]}\n")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 corrupt_mesh.py <input.off> <output.off>")
        sys.exit(1)
    
    infile = sys.argv[1]
    outfile = sys.argv[2]
    
    try:
        verts, faces = read_off(infile)
        c_verts, c_faces = corrupt_mesh(verts, faces)
        save_off(outfile, c_verts, c_faces)
        print(f"Fully corrupted mesh saved to {outfile}")
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
