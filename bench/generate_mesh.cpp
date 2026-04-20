#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>

struct Vector3 {
    double x, y, z;
    Vector3(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
    Vector3 operator+(const Vector3& v) const { return Vector3(x+v.x, y+v.y, z+v.z); }
    Vector3 operator/(double d) const { return Vector3(x/d, y/d, z/d); }
    double length() const { return std::sqrt(x*x + y*y + z*z); }
    void normalize() {
        double l = length();
        if (l > 0) { x /= l; y /= l; z /= l; }
    }
};

struct Face {
    int v1, v2, v3;
};

int get_middle_point(int p1_idx, int p2_idx, std::vector<Vector3>& verts, std::map<std::pair<int, int>, int>& cache) {
    int smaller = std::min(p1_idx, p2_idx);
    int greater = std::max(p1_idx, p2_idx);
    auto key = std::make_pair(smaller, greater);

    if (cache.count(key)) return cache[key];

    Vector3 v1 = verts[p1_idx];
    Vector3 v2 = verts[p2_idx];
    Vector3 middle = (v1 + v2) / 2.0;
    middle.normalize();

    verts.push_back(middle);
    int index = verts.size() - 1;
    cache[key] = index;
    return index;
}

void generate_mesh(const std::string& type, int subdivisions, std::vector<Vector3>& verts, std::vector<Face>& faces) {
    if (type == "octahedron") {
        verts = {
            { 1, 0, 0}, {-1, 0, 0}, { 0, 1, 0}, { 0,-1, 0}, { 0, 0, 1}, { 0, 0,-1}
        };
        faces = {
            {4, 0, 2}, {4, 2, 1}, {4, 1, 3}, {4, 3, 0},
            {5, 2, 0}, {5, 1, 2}, {5, 3, 1}, {5, 0, 3}
        };
    } else { // icosahedron (sphere)
        double f = (1.0 + std::sqrt(5.0)) / 2.0;
        verts = {
            {-1,  f,  0}, { 1,  f,  0}, {-1, -f,  0}, { 1, -f,  0},
            { 0, -1,  f}, { 0,  1,  f}, { 0, -1, -f}, { 0,  1, -f},
            { f,  0, -1}, { f,  0,  1}, {-f,  0, -1}, {-f,  0,  1}
        };
        faces = {
            {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
            {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
            {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
            {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
        };
    }

    for (auto& v : verts) v.normalize();

    for (int i = 0; i < subdivisions; ++i) {
        std::vector<Face> faces_subdiv;
        std::map<std::pair<int, int>, int> cache;
        for (const auto& tri : faces) {
            int a = get_middle_point(tri.v1, tri.v2, verts, cache);
            int b = get_middle_point(tri.v2, tri.v3, verts, cache);
            int c = get_middle_point(tri.v3, tri.v1, verts, cache);
            faces_subdiv.push_back({tri.v1, a, c});
            faces_subdiv.push_back({tri.v2, b, a});
            faces_subdiv.push_back({tri.v3, c, b});
            faces_subdiv.push_back({a, b, c});
        }
        faces = std::move(faces_subdiv);
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

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: ./generate_mesh <octahedron|sphere> <subdivisions> <output.off>\n";
        return 1;
    }
    std::string type = argv[1];
    int subdiv = std::stoi(argv[2]);
    std::string outfile = argv[3];

    std::vector<Vector3> verts;
    std::vector<Face> faces;
    generate_mesh(type, subdiv, verts, faces);
    save_off(outfile, verts, faces);
    std::cout << "Generated " << type << " with " << verts.size() << " vertices and " << faces.size() << " triangles.\n";
    return 0;
}
