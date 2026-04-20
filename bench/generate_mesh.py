import sys
import math

def generate_sphere(subdivisions):
    # Start with an octahedron
    f = (1.0 + math.sqrt(5.0)) / 2.0
    verts = [
        (-1,  f,  0), ( 1,  f,  0), (-1, -f,  0), ( 1, -f,  0),
        ( 0, -1,  f), ( 0,  1,  f), ( 0, -1, -f), ( 0,  1, -f),
        ( f,  0, -1), ( f,  0,  1), (-f,  0, -1), (-f,  0,  1)
    ]
    # Normalize to unit sphere
    verts = [(v[0]/math.sqrt(v[0]**2+v[1]**2+v[2]**2), 
              v[1]/math.sqrt(v[0]**2+v[1]**2+v[2]**2), 
              v[2]/math.sqrt(v[0]**2+v[1]**2+v[2]**2)) for v in verts]
    
    faces = [
        (0, 11, 5), (0, 5, 1), (0, 1, 7), (0, 7, 10), (0, 10, 11),
        (1, 5, 9), (5, 11, 4), (11, 10, 2), (10, 7, 6), (7, 1, 8),
        (3, 9, 4), (3, 4, 2), (3, 2, 6), (3, 6, 8), (3, 8, 9),
        (4, 9, 5), (2, 4, 11), (6, 2, 10), (8, 6, 7), (9, 8, 1)
    ]

    def get_middle_point(p1_idx, p2_idx, verts, middle_point_cache):
        smaller_idx = min(p1_idx, p2_idx)
        greater_idx = max(p1_idx, p2_idx)
        key = (smaller_idx, greater_idx)

        if key in middle_point_cache:
            return middle_point_cache[key]

        p1 = verts[p1_idx]
        p2 = verts[p2_idx]
        middle = ((p1[0]+p2[0])/2, (p1[1]+p2[1])/2, (p1[2]+p2[2])/2)
        # normalize
        length = math.sqrt(middle[0]**2 + middle[1]**2 + middle[2]**2)
        middle = (middle[0]/length, middle[1]/length, middle[2]/length)
        
        verts.append(middle)
        index = len(verts) - 1
        middle_point_cache[key] = index
        return index

    for _ in range(subdivisions):
        faces_subdiv = []
        cache = {}
        for tri in faces:
            v1 = tri[0]
            v2 = tri[1]
            v3 = tri[2]
            a = get_middle_point(v1, v2, verts, cache)
            b = get_middle_point(v2, v3, verts, cache)
            c = get_middle_point(v3, v1, verts, cache)
            faces_subdiv.append((v1, a, c))
            faces_subdiv.append((v2, b, a))
            faces_subdiv.append((v3, c, b))
            faces_subdiv.append((a, b, c))
        faces = faces_subdiv

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
        print("Usage: python3 generate_mesh.py <subdivisions> <output.off>")
        sys.exit(1)
    
    subdiv = int(sys.argv[1])
    outfile = sys.argv[2]
    
    verts, faces = generate_sphere(subdiv)
    save_off(outfile, verts, faces)
    print(f"Generated sphere with {len(verts)} vertices and {len(faces)} triangles.")
