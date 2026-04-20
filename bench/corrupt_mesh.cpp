#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <set>

struct Vector3 {
    double x, y, z;
};

struct Face {
    int v1, v2, v3;
};

void read_off(const std::string& filename, std::vector<Vector3>& verts, std::vector<Face>& faces) {
    std::ifstream f(filename);
    std::string line;
    if (!std::getline(f, line) || line.substr(0, 3) != "OFF") {
        throw std::runtime_error("Not a valid OFF file");
    }

    int nv, nf, ne;
    if (line.size() > 3) {
        std::stringstream ss(line.substr(3));
        ss >> nv >> nf >> ne;
    } else {
        while (std::getline(f, line) && (line.empty() || line[0] == '#'));
        std::stringstream ss(line);
        ss >> nv >> nf >> ne;
    }

    verts.reserve(nv);
    for (int i = 0; i < nv; ++i) {
        double x, y, z;
        f >> x >> y >> z;
        verts.push_back({x, y, z});
    }

    faces.reserve(nf);
    for (int i = 0; i < nf; ++i) {
        int n, v1, v2, v3;
        f >> n >> v1 >> v2 >> v3;
        faces.push_back({v1, v2, v3});
    }
}

void save_off(const std::string& filename, const std::vector<Vector3>& verts, const std::vector<Face>& faces) {
    std::ofstream f(filename);
    f << "OFF\n";
    f << verts.size() << " " << faces.size() << " 0\n";
    for (const auto& v : verts) {
        f << v.x << " " << v.y << " " << v.z << "\n";
    }
    for (const auto& tri : faces) {
        f << "3 " << tri.v1 << " " << tri.v2 << " " << tri.v3 << "\n";
    }
}

void corrupt_mesh(std::vector<Vector3>& verts, std::vector<Face>& faces) {
    std::cout << "Applying all possible corruptions...\n";
    std::random_device rd;
    std::mt19937 gen(rd());

    // 1. Create Holes (Remove 5% of faces)
    int num_to_remove = faces.size() * 0.05;
    std::vector<int> indices(faces.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);
    
    std::set<int> to_remove(indices.begin(), indices.begin() + num_to_remove);
    std::vector<Face> new_faces;
    for (int i = 0; i < faces.size(); ++i) {
        if (to_remove.find(i) == to_remove.end()) {
            new_faces.push_back(faces[i]);
        }
    }
    faces = std::move(new_faces);
    std::cout << " - Created " << num_to_remove << " holes.\n";

    // 2. Inconsistent Normals (Flip 5% of faces)
    int num_to_flip = faces.size() * 0.05;
    std::vector<int> flip_indices(faces.size());
    std::iota(flip_indices.begin(), flip_indices.end(), 0);
    std::shuffle(flip_indices.begin(), flip_indices.end(), gen);
    for (int i = 0; i < num_to_flip; ++i) {
        std::swap(faces[flip_indices[i]].v2, faces[flip_indices[i]].v3);
    }
    std::cout << " - Flipped " << num_to_flip << " face normals.\n";

    // 3. Degeneracies (Collapse edges in 5% of faces)
    int num_to_degenerate = faces.size() * 0.05;
    std::vector<int> deg_indices(faces.size());
    std::iota(deg_indices.begin(), deg_indices.end(), 0);
    std::shuffle(deg_indices.begin(), deg_indices.end(), gen);
    for (int i = 0; i < num_to_degenerate; ++i) {
        int idx = deg_indices[i];
        verts[faces[idx].v2] = verts[faces[idx].v1];
    }
    std::cout << " - Created " << num_to_degenerate << " degenerate triangles.\n";

    // 4. Add Noise (Floating small components)
    for (int i = 0; i < 3; ++i) {
        int base_idx = verts.size();
        double offset = 5.0 + i;
        verts.push_back({offset, offset, offset});
        verts.push_back({offset+0.1, offset, offset});
        verts.push_back({offset, offset+0.1, offset});
        faces.push_back({base_idx, base_idx+1, base_idx+2});
    }
    std::cout << " - Added 3 small noise components.\n";

    // 5. Create Singularities (Bowtie vertex)
    if (!verts.empty()) {
        std::uniform_int_distribution<> dist(0, verts.size() - 1);
        int target_v = dist(gen);
        int base_idx = verts.size();
        verts.push_back({verts[target_v].x+1, verts[target_v].y, verts[target_v].z});
        verts.push_back({verts[target_v].x, verts[target_v].y+1, verts[target_v].z});
        faces.push_back({target_v, base_idx, base_idx+1});
        std::cout << " - Created singularity at vertex " << target_v << ".\n";
    }

    // 6. Self-Intersections (Overlap)
    if (!faces.empty()) {
        std::uniform_int_distribution<> dist(0, faces.size() - 1);
        int f_idx = dist(gen);
        Face f = faces[f_idx];
        int base_idx = verts.size();
        verts.push_back({verts[f.v1].x, verts[f.v1].y, verts[f.v1].z + 0.01});
        verts.push_back({verts[f.v2].x, verts[f.v2].y, verts[f.v2].z + 0.01});
        verts.push_back({verts[f.v3].x, verts[f.v3].y, verts[f.v3].z + 0.01});
        faces.push_back({base_idx, base_idx+1, base_idx+2});
        std::cout << " - Created overlapping geometry.\n";
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: ./corrupt_mesh <input.off> <output.off>\n";
        return 1;
    }
    std::string infile = argv[1];
    std::string outfile = argv[2];

    std::vector<Vector3> verts;
    std::vector<Face> faces;
    try {
        read_off(infile, verts, faces);
        corrupt_mesh(verts, faces);
        save_off(outfile, verts, faces);
        std::cout << "Fully corrupted mesh saved to " << outfile << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
