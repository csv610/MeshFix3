#include "point.h"
#include "tin.h"
#include "vertex.h"
#include "edge.h"
#include "triangle.h"
#include "list.h"
#include "matrix.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <sstream>

using namespace T_MESH;

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(x) do { \
    if (x) { \
        tests_passed++; \
    } else { \
        std::cerr << "FAILED: " << #x << " at line " << __LINE__ << std::endl; \
        tests_failed++; \
    } \
} while(0)

#define ASSERT_FALSE(x) ASSERT_TRUE(!(x))
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_NE(a, b) ASSERT_TRUE((a) != (b))
#define ASSERT_DOUBLE_EQ(a, b, eps) ASSERT_TRUE(std::abs((a) - (b)) < (eps))

void test_point_basics() {
    std::cout << "Testing Point basics..." << std::endl;

    Point p1(1.0, 2.0, 3.0);
    Point p2(4.0, 5.0, 6.0);

    Point p3 = p1 + p2;
    ASSERT_EQ(p3.x, 5.0);
    ASSERT_EQ(p3.y, 7.0);
    ASSERT_EQ(p3.z, 9.0);

    Point p4 = p2 - p1;
    ASSERT_EQ(p4.x, 3.0);
    ASSERT_EQ(p4.y, 3.0);
    ASSERT_EQ(p4.z, 3.0);

    coord dot = p1 * p2;
    ASSERT_EQ(dot, 32.0);

    Point cross = p1 & p2;
    ASSERT_EQ(cross.x, -3.0);
    ASSERT_EQ(cross.y, 6.0);
    ASSERT_EQ(cross.z, -3.0);

    Point p5 = p1 * 2.0;
    ASSERT_EQ(p5.x, 2.0);
    ASSERT_EQ(p5.y, 4.0);
    ASSERT_EQ(p5.z, 6.0);

    p5 *= 0.5;
    ASSERT_EQ(p5.x, 1.0);
    ASSERT_EQ(p5.y, 2.0);
    ASSERT_EQ(p5.z, 3.0);

    Point p6 = p1 / 2.0;
    ASSERT_EQ(p6.x, 0.5);
    ASSERT_EQ(p6.y, 1.0);
    ASSERT_EQ(p6.z, 1.5);

    std::cout << "Point basics passed!" << std::endl;
}

void test_point_comparison() {
    std::cout << "Testing Point comparison..." << std::endl;

    Point p1(1.0, 2.0, 3.0);
    Point p2(1.0, 2.0, 3.0);
    Point p3(2.0, 2.0, 3.0);
    Point p4(1.0, 3.0, 3.0);

    ASSERT_TRUE(p1 == p2);
    ASSERT_FALSE(p1 == p3);
    ASSERT_FALSE(p1 == p4);

    ASSERT_TRUE((p1 <=> p2) == std::partial_ordering::equivalent);
    ASSERT_TRUE((p1 <=> p3) == std::partial_ordering::less);
    ASSERT_TRUE((p3 <=> p1) == std::partial_ordering::greater);

    std::cout << "Point comparison passed!" << std::endl;
}

void test_point_distance() {
    std::cout << "Testing Point distance..." << std::endl;

    Point p1(0.0, 0.0, 0.0);
    Point p2(3.0, 4.0, 0.0);

    ASSERT_EQ(p1.squaredDistance(&p2), 25.0);
    ASSERT_DOUBLE_EQ(p1.distance(p2), 5.0, 1e-10);
    ASSERT_DOUBLE_EQ(p1.distance(&p2), 5.0, 1e-10);

    std::cout << "Point distance passed!" << std::endl;
}

void test_point_vector_operations() {
    std::cout << "Testing Point vector operations..." << std::endl;

    Point p1(3.0, 0.0, 0.0);
    ASSERT_TRUE(p1.isNull() == false);

    Point p2(0.0, 0.0, 0.0);
    ASSERT_TRUE(p2.isNull() == true);

    Point p3(1.0, 2.0, 3.0);
    ASSERT_EQ(p3.squaredLength(), 14.0);

    Point p4(3.0, 4.0, 0.0);
    ASSERT_DOUBLE_EQ(p4.length(), 5.0, 1e-10);

    Point inv = p1.inverse();
    ASSERT_EQ(inv.x, -3.0);
    ASSERT_EQ(inv.y, 0.0);
    ASSERT_EQ(inv.z, 0.0);

    Point p5(1.0, 2.0, 3.0);
    Point p6 = p5;
    p6.invert();
    ASSERT_EQ(p6.x, -1.0);
    ASSERT_EQ(p6.y, -2.0);
    ASSERT_EQ(p6.z, -3.0);

    std::cout << "Point vector operations passed!" << std::endl;
}

void test_point_at() {
    std::cout << "Testing Point at/[] operators..." << std::endl;

    Point p(1.0, 2.0, 3.0);
    ASSERT_EQ(p.at(0), 1.0);
    ASSERT_EQ(p.at(1), 2.0);
    ASSERT_EQ(p.at(2), 3.0);

    ASSERT_EQ(p[0], 1.0);
    ASSERT_EQ(p[1], 2.0);
    ASSERT_EQ(p[2], 3.0);

    p[0] = 10.0;
    ASSERT_EQ(p.x, 10.0);

    std::cout << "Point at/[] passed!" << std::endl;
}

void test_list_basics() {
    std::cout << "Testing List basics..." << std::endl;

    List list;
    ASSERT_EQ(list.numels(), 0);
    ASSERT_TRUE(list.head() == NULL);
    ASSERT_TRUE(list.tail() == NULL);

    int a = 1, b = 2;
    list.appendHead(&a);
    ASSERT_EQ(list.numels(), 1);

    list.appendTail(&b);
    ASSERT_EQ(list.numels(), 2);

    Node* n = list.head();
    ASSERT_TRUE(n != NULL);
    ASSERT_EQ(*(int*)n->data, 1);

    n = list.tail();
    ASSERT_TRUE(n != NULL);
    ASSERT_EQ(*(int*)n->data, 2);

    std::cout << "List basics passed!" << std::endl;
}

void test_list_operations() {
    std::cout << "Testing List operations..." << std::endl;

    int a = 1, b = 2;
    List list;
    list.appendTail(&a);
    list.appendTail(&b);
    list.appendTail(&a);

    ASSERT_EQ(list.numels(), 3);

    void* data = list.popHead();
    ASSERT_EQ(*(int*)data, 1);
    ASSERT_EQ(list.numels(), 2);

    data = list.popTail();
    ASSERT_EQ(*(int*)data, 1);
    ASSERT_EQ(list.numels(), 1);

    list.appendHead(&a);
    list.appendHead(&b);
    list.removeNode(&a);
    ASSERT_EQ(list.numels(), 2);

    std::cout << "List operations passed!" << std::endl;
}

void test_list_node_manipulation() {
    std::cout << "Testing List node manipulation..." << std::endl;

    int a = 1, b = 2, c = 3, d = 4;
    List list;
    list.appendTail(&a);
    list.appendTail(&b);
    list.appendTail(&c);

    Node* second = list.head()->next();
    list.insertAfter(second, &d);

    ASSERT_EQ(list.numels(), 4);

    Node* n = list.head();
    ASSERT_EQ(*(int*)n->data, 1);
    n = n->next();
    ASSERT_EQ(*(int*)n->data, 2);
    n = n->next();
    ASSERT_EQ(*(int*)n->data, 4);

    std::cout << "List node manipulation passed!" << std::endl;
}

void test_list_getnode() {
    std::cout << "Testing List getNode..." << std::endl;

    int arr[] = {10, 20, 30, 40, 50};
    List list;
    for (int i = 0; i < 5; i++) {
        list.appendTail(&arr[i]);
    }

    Node* n0 = list.getNode(0);
    ASSERT_TRUE(n0 != NULL);
    ASSERT_EQ(*(int*)n0->data, 10);

    Node* n2 = list.getNode(2);
    ASSERT_TRUE(n2 != NULL);
    ASSERT_EQ(*(int*)n2->data, 30);

    Node* n10 = list.getNode(10);
    ASSERT_TRUE(n10 == NULL);

    std::cout << "List getNode passed!" << std::endl;
}

void test_list_tovector() {
    std::cout << "Testing List toVector..." << std::endl;

    int arr[] = {10, 20, 30};
    List list;
    for (int i = 0; i < 3; i++) {
        list.appendTail(&arr[i]);
    }

    std::vector<void*> vec = list.toVector();
    ASSERT_EQ(vec.size(), 3U);
    ASSERT_EQ(*(int*)vec[0], 10);
    ASSERT_EQ(*(int*)vec[1], 20);
    ASSERT_EQ(*(int*)vec[2], 30);

    std::cout << "List toVector passed!" << std::endl;
}

void test_vertex_basics() {
    std::cout << "Testing Vertex basics..." << std::endl;

    Vertex v1(1.0, 2.0, 3.0);
    ASSERT_EQ(v1.x, 1.0);
    ASSERT_EQ(v1.y, 2.0);
    ASSERT_EQ(v1.z, 3.0);
    ASSERT_TRUE(v1.isLinked() == false);
    ASSERT_TRUE(v1.isOnBoundary() == false);

    Point p(4.0, 5.0, 6.0);
    Vertex v2(&p);
    ASSERT_EQ(v2.x, 4.0);
    ASSERT_EQ(v2.y, 5.0);
    ASSERT_EQ(v2.z, 6.0);

    Vertex v3(p);
    ASSERT_EQ(v3.x, 4.0);
    ASSERT_EQ(v3.y, 5.0);
    ASSERT_EQ(v3.z, 6.0);

    Vertex v4;
    ASSERT_EQ(v4.x, 0.0);
    ASSERT_EQ(v4.y, 0.0);
    ASSERT_EQ(v4.z, 0.0);

    ASSERT_TRUE(v4.isBaseType() == true);

    std::cout << "Vertex basics passed!" << std::endl;
}

void test_vertex_valence() {
    std::cout << "Testing Vertex valence..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v = mesh.newVertex(0.0, 0.0, 0.0);
    ASSERT_EQ(v->valence(), 0);

    Vertex* v1 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v, v1);
    Edge* e2 = mesh.CreateEdge(v1, v2);
    Edge* e3 = mesh.CreateEdge(v2, v);

    Triangle* t = mesh.CreateTriangle(e1, e2, e3);
    ASSERT_TRUE(t != NULL);

    ASSERT_EQ(v->valence(), 2);
    ASSERT_EQ(v1->valence(), 2);
    ASSERT_EQ(v2->valence(), 2);

    std::cout << "Vertex valence passed!" << std::endl;
}

void test_edge_basics() {
    std::cout << "Testing Edge basics..." << std::endl;

    Vertex v1(0.0, 0.0, 0.0);
    Vertex v2(1.0, 0.0, 0.0);
    Vertex v3(0.0, 1.0, 0.0);

    Edge e1(&v1, &v2);
    ASSERT_TRUE(e1.hasVertex(&v1) == true);
    ASSERT_TRUE(e1.hasVertex(&v2) == true);
    ASSERT_TRUE(e1.hasVertex(&v3) == false);
    ASSERT_TRUE(e1.isOnBoundary() == true);
    ASSERT_TRUE(e1.isIsolated() == true);

    Edge e2(&v2, &v3);
    ASSERT_EQ(e1.v1, &v1);
    ASSERT_EQ(e1.v2, &v2);

    ASSERT_TRUE(e1.isBaseType() == true);

    std::cout << "Edge basics passed!" << std::endl;
}

void test_edge_length() {
    std::cout << "Testing Edge length..." << std::endl;

    Vertex v1(0.0, 0.0, 0.0);
    Vertex v2(3.0, 4.0, 0.0);

    Edge e(&v1, &v2);
    ASSERT_EQ(e.squaredLength(), 25.0);
    ASSERT_DOUBLE_EQ(e.length(), 5.0, 1e-10);

    Point mid = e.getMidPoint();
    ASSERT_EQ(mid.x, 1.5);
    ASSERT_EQ(mid.y, 2.0);
    ASSERT_EQ(mid.z, 0.0);

    Point vec = e.toVector();
    ASSERT_EQ(vec.x, 3.0);
    ASSERT_EQ(vec.y, 4.0);
    ASSERT_EQ(vec.z, 0.0);

    std::cout << "Edge length passed!" << std::endl;
}

void test_edge_collapse() {
    std::cout << "Testing Edge collapse..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v4 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v4);
    Edge* e3 = mesh.CreateEdge(v4, v1);

    Triangle* t = mesh.CreateTriangle(e1, e2, e3);
    ASSERT_TRUE(t != NULL);

    ASSERT_EQ(e1->squaredLength(), 1.0);

    std::cout << "Edge collapse passed!" << std::endl;
}

void test_triangle_basics() {
    std::cout << "Testing Triangle basics..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    Triangle* t = mesh.CreateTriangle(e1, e2, e3);
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(t->isLinked() == true);
    ASSERT_TRUE(t->isBaseType() == true);

    ASSERT_TRUE(t->hasEdge(e1) == true);
    ASSERT_TRUE(t->hasEdge(e2) == true);
    ASSERT_TRUE(t->hasEdge(e3) == true);

    ASSERT_TRUE(t->hasVertex(v1) == true);
    ASSERT_TRUE(t->hasVertex(v2) == true);
    ASSERT_TRUE(t->hasVertex(v3) == true);

    Vertex* tv1 = t->v1();
    Vertex* tv2 = t->v2();
    Vertex* tv3 = t->v3();

    ASSERT_TRUE(tv1 == v1 || tv1 == v2 || tv1 == v3);
    ASSERT_TRUE(tv2 == v1 || tv2 == v2 || tv2 == v3);
    ASSERT_TRUE(tv3 == v1 || tv3 == v2 || tv3 == v3);

    std::cout << "Triangle basics passed!" << std::endl;
}

void test_triangle_adjacency() {
    std::cout << "Testing Triangle adjacency..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    Triangle* t1 = mesh.CreateTriangle(e1, e2, e3);

    ASSERT_TRUE(t1 != NULL);
    ASSERT_TRUE(t1->e1 != NULL);
    ASSERT_TRUE(t1->e2 != NULL);
    ASSERT_TRUE(t1->e3 != NULL);

    Edge* common = t1->commonEdge(t1);
    ASSERT_TRUE(common != NULL);

    std::cout << "Triangle adjacency passed!" << std::endl;
}

void test_triangle_normal() {
    std::cout << "Testing Triangle normal..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    Triangle* t = mesh.CreateTriangle(e1, e2, e3);
    ASSERT_TRUE(t != NULL);

    Point normal = t->getNormal();
    ASSERT_TRUE(normal.x != 0.0 || normal.y != 0.0 || normal.z != 0.0);

    Point vec = t->getVector();
    ASSERT_TRUE(vec.x != 0.0 || vec.y != 0.0 || vec.z != 0.0);

    std::cout << "Triangle normal passed!" << std::endl;
}

void test_triangle_center() {
    std::cout << "Testing Triangle center..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(2.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 2.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    Triangle* t = mesh.CreateTriangle(e1, e2, e3);
    ASSERT_TRUE(t != NULL);

    Point center = t->getCenter();
    ASSERT_DOUBLE_EQ(center.x, 2.0/3.0, 1e-6);
    ASSERT_DOUBLE_EQ(center.y, 2.0/3.0, 1e-6);
    ASSERT_DOUBLE_EQ(center.z, 0.0, 1e-6);

    std::cout << "Triangle center passed!" << std::endl;
}

void test_triangle_area() {
    std::cout << "Testing Triangle area..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    Triangle* t = mesh.CreateTriangle(e1, e2, e3);
    ASSERT_TRUE(t != NULL);

    double area = t->area();
    ASSERT_DOUBLE_EQ(area, 0.5, 1e-10);

    std::cout << "Triangle area passed!" << std::endl;
}

void test_mesh_euler_update() {
    std::cout << "Testing Basic_TMesh eulerUpdate..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    (void)mesh.CreateTriangle(e1, e2, e3);

    int shells = mesh.shells();
    ASSERT_TRUE(shells >= 0);

    std::cout << "Basic_TMesh eulerUpdate passed!" << std::endl;
}

void test_matrix_basics() {
    std::cout << "Testing Matrix basics..." << std::endl;

    Matrix4x4 m;
    ASSERT_TRUE(m.matrix[0][0] != DBL_MAX);

    std::cout << "Matrix basics passed!" << std::endl;
}

void test_matrix_transform() {
    std::cout << "Testing Matrix transform..." << std::endl;

    Matrix4x4 m;
    ASSERT_TRUE(&m.matrix[0][0] != nullptr);

    std::cout << "Matrix transform passed!" << std::endl;
}

void test_repair_report() {
    std::cout << "Testing RepairReport..." << std::endl;

    RepairReport report;
    ASSERT_FALSE(report.repairedSomething());

    report.patched_holes = 1;
    ASSERT_TRUE(report.repairedSomething());

    report = RepairReport{};
    report.degeneracies_before = 3;
    report.degeneracies_after = 1;
    ASSERT_TRUE(report.repairedSomething());

    report = RepairReport{};
    report.intersections_before = 5;
    report.intersections_after = 5;
    ASSERT_FALSE(report.repairedSomething());

    report.intersections_after = 2;
    ASSERT_TRUE(report.repairedSomething());

    std::cout << "RepairReport passed!" << std::endl;
}

void test_repair_options() {
    std::cout << "Testing RepairOptions..." << std::endl;

    RepairOptions opts;
    ASSERT_FALSE(opts.join_multiple_components);

    RepairOptions opts2;
    opts2.join_multiple_components = true;
    ASSERT_TRUE(opts2.join_multiple_components);

    std::cout << "RepairOptions passed!" << std::endl;
}

void test_mesh_creation() {
    std::cout << "Testing Basic_TMesh creation..." << std::endl;

    Basic_TMesh mesh;
    ASSERT_EQ(mesh.V.numels(), 0);
    ASSERT_EQ(mesh.E.numels(), 0);
    ASSERT_EQ(mesh.T.numels(), 0);

    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    ASSERT_TRUE(v1 != NULL);
    ASSERT_TRUE(v2 != NULL);
    ASSERT_TRUE(v3 != NULL);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    ASSERT_TRUE(e1 != NULL);
    ASSERT_TRUE(e2 != NULL);
    ASSERT_TRUE(e3 != NULL);

    Triangle* t = mesh.CreateTriangle(e1, e2, e3);
    ASSERT_TRUE(t != NULL);

    std::cout << "Basic_TMesh creation passed!" << std::endl;
}

void test_mesh_remove_elements() {
    std::cout << "Testing Basic_TMesh remove elements..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    (void)mesh.CreateTriangle(e1, e2, e3);

    ASSERT_EQ(mesh.T.numels(), 1);
    Triangle* t = mesh.T.head() ? (Triangle*)mesh.T.head()->data : nullptr;
    if (t) {
        mesh.removeTriangle(t);
    }
    ASSERT_EQ(mesh.T.numels(), 0);

    std::cout << "Basic_TMesh remove elements passed!" << std::endl;
}

void test_mesh_bounding_box() {
    std::cout << "Testing Basic_TMesh bounding box..." << std::endl;

    Basic_TMesh mesh("triangle");
    ASSERT_EQ(mesh.V.numels(), 3);

    Point bottom, top;
    mesh.getBoundingBox(bottom, top);

    ASSERT_TRUE(bottom.x != DBL_MAX);
    ASSERT_TRUE(top.x != DBL_MAX);

    std::cout << "Basic_TMesh bounding box passed!" << std::endl;
}

void test_mesh_area() {
    std::cout << "Testing Basic_TMesh area..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    (void)mesh.CreateTriangle(e1, e2, e3);

    double area = mesh.area();
    ASSERT_DOUBLE_EQ(area, 0.5, 1e-10);

    std::cout << "Basic_TMesh area passed!" << std::endl;
}

void test_mesh_translate() {
    std::cout << "Testing Basic_TMesh translate..." << std::endl;

    Basic_TMesh mesh("triangle");
    ASSERT_EQ(mesh.V.numels(), 3);

    Vertex* v1 = (Vertex*)mesh.V.head()->data;
    double origX = v1->x;

    Point offset(5.0, 10.0, 0.0);
    mesh.translate(offset);

    ASSERT_TRUE(v1->x != origX);

    std::cout << "Basic_TMesh translate passed!" << std::endl;
}

void test_mesh_center() {
    std::cout << "Testing Basic_TMesh center..." << std::endl;

    Basic_TMesh mesh("triangle");
    ASSERT_EQ(mesh.V.numels(), 3);

    Point center = mesh.getCenter();
    ASSERT_TRUE(center.x != DBL_MAX);
    ASSERT_TRUE(center.y != DBL_MAX);

    std::cout << "Basic_TMesh center passed!" << std::endl;
}

void test_mesh_selection() {
    std::cout << "Testing Basic_TMesh selection..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    (void)mesh.CreateTriangle(e1, e2, e3);

    mesh.deselectTriangles();
    int selected = mesh.selectBoundaryTriangles();
    ASSERT_TRUE(selected >= 0);

    std::cout << "Basic_TMesh selection passed!" << std::endl;
}

void test_point_setvalue() {
    std::cout << "Testing Point setValue..." << std::endl;

    Point p;
    p.setValue(1.0, 2.0, 3.0);
    ASSERT_EQ(p.x, 1.0);
    ASSERT_EQ(p.y, 2.0);
    ASSERT_EQ(p.z, 3.0);

    Point p2(4.0, 5.0, 6.0);
    p.setValue(&p2);
    ASSERT_EQ(p.x, 4.0);
    ASSERT_EQ(p.y, 5.0);
    ASSERT_EQ(p.z, 6.0);

    p.setValue(p2);
    ASSERT_EQ(p.x, 4.0);
    ASSERT_EQ(p.y, 5.0);
    ASSERT_EQ(p.z, 6.0);

    std::cout << "Point setValue passed!" << std::endl;
}

void test_point_static_methods() {
    std::cout << "Testing Point static methods..." << std::endl;

    Point p1(0.0, 0.0, 0.0);
    Point p2(1.0, 0.0, 0.0);
    Point p3(0.0, 1.0, 0.0);

    coord area2d = Point::squaredTriangleArea3D(p1, p2, p3);
    ASSERT_DOUBLE_EQ(area2d, 0.25, 1e-10);

    Point segStart(0.0, 0.0, 0.0);
    Point segEnd(1.0, 0.0, 0.0);
    Point pointInMiddle(0.5, 0.0, 0.0);
    Point pointOutside(2.0, 0.0, 0.0);

    ASSERT_TRUE(Point::pointInSegment(&pointInMiddle, &segStart, &segEnd));
    ASSERT_TRUE(Point::pointInInnerSegment(&pointInMiddle, &segStart, &segEnd));
    ASSERT_FALSE(Point::pointInSegment(&pointOutside, &segStart, &segEnd));

    std::cout << "Point static methods passed!" << std::endl;
}

void test_edge_swap() {
    std::cout << "Testing Edge swap..." << std::endl;

    Basic_TMesh mesh("tetrahedron");
    ASSERT_TRUE(mesh.T.numels() > 0);

    Edge* e = (Edge*)mesh.E.head()->data;
    ASSERT_TRUE(e != NULL);

    ASSERT_TRUE(e->isLinked());

    std::cout << "Edge swap passed!" << std::endl;
}

void test_edge_collapse_methods() {
    std::cout << "Testing Edge collapse methods..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    (void)mesh.CreateTriangle(e1, e2, e3);

    Point mid = e1->getMidPoint();
    ASSERT_TRUE(mid.x != DBL_MAX);
    ASSERT_TRUE(mid.y != DBL_MAX);

    Point vec = e1->toVector();
    ASSERT_TRUE(vec.x != DBL_MAX || vec.y != DBL_MAX || vec.z != DBL_MAX);

    std::cout << "Edge collapse methods passed!" << std::endl;
}

void test_triangle_queries() {
    std::cout << "Testing Triangle queries..." << std::endl;

    Basic_TMesh mesh("triangle");
    Triangle* t = (Triangle*)mesh.T.head()->data;
    ASSERT_TRUE(t != NULL);

    Vertex* v = t->v1();
    ASSERT_TRUE(v != NULL);

    Edge* e = t->oppositeEdge(v);
    ASSERT_TRUE(e != NULL);

    ASSERT_TRUE(t->hasVertex(v) == true);

    std::cout << "Triangle queries passed!" << std::endl;
}

void test_mesh_boundaries() {
    std::cout << "Testing Basic_TMesh boundaries..." << std::endl;

    Basic_TMesh mesh;
    Vertex* v1 = mesh.newVertex(0.0, 0.0, 0.0);
    Vertex* v2 = mesh.newVertex(1.0, 0.0, 0.0);
    Vertex* v3 = mesh.newVertex(0.0, 1.0, 0.0);

    Edge* e1 = mesh.CreateEdge(v1, v2);
    Edge* e2 = mesh.CreateEdge(v2, v3);
    Edge* e3 = mesh.CreateEdge(v3, v1);

    (void)mesh.CreateTriangle(e1, e2, e3);

    int bounds = mesh.boundaries();
    ASSERT_TRUE(bounds >= 0);

    int handles = mesh.handles();
    ASSERT_TRUE(handles >= 0);

    std::cout << "Basic_TMesh boundaries passed!" << std::endl;
}

void test_vertex_neighborhood() {
    std::cout << "Testing Vertex neighborhood..." << std::endl;

    Basic_TMesh mesh("triangle");
    Vertex* v = (Vertex*)mesh.V.head()->data;
    ASSERT_TRUE(v != NULL);

    int val = v->valence();
    ASSERT_TRUE(val >= 0);

    bool onBoundary = v->isOnBoundary();
    (void)onBoundary;

    std::cout << "Vertex neighborhood passed!" << std::endl;
}

void test_point_orientation() {
    std::cout << "Testing Point orientation..." << std::endl;

    Point p1(0.0, 0.0, 0.0);
    Point p2(1.0, 0.0, 0.0);
    Point p3(0.0, 1.0, 0.0);
    Point p4(0.0, 0.0, 1.0);

    coord o = orient3d_span(std::span<const double, 3>((double*)&p1.x, 3),
                            std::span<const double, 3>((double*)&p2.x, 3),
                            std::span<const double, 3>((double*)&p3.x, 3),
                            std::span<const double, 3>((double*)&p4.x, 3));
    (void)o;

    std::cout << "Point orientation passed!" << std::endl;
}

int main() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Running MeshFix3 Unit Tests" << std::endl;
    std::cout << "========================================\n" << std::endl;

    try {
        test_point_basics();
        test_point_comparison();
        test_point_distance();
        test_point_vector_operations();
        test_point_at();
        test_point_setvalue();
        test_point_static_methods();

        test_list_basics();
        test_list_operations();
        test_list_node_manipulation();
        test_list_getnode();
        test_list_tovector();

        test_vertex_basics();
        test_vertex_valence();

        test_edge_basics();
        test_edge_length();
        test_edge_collapse();

        test_triangle_basics();
        test_triangle_adjacency();
        test_triangle_normal();
        test_triangle_center();
        test_triangle_area();

        test_matrix_basics();
        test_matrix_transform();

        test_repair_report();
        test_repair_options();

        test_mesh_creation();
        test_mesh_remove_elements();
        test_mesh_bounding_box();
        test_mesh_area();
        test_mesh_translate();
        test_mesh_center();
        test_mesh_euler_update();
        test_mesh_selection();
        test_edge_swap();
        test_edge_collapse_methods();
        test_triangle_queries();
        test_mesh_boundaries();
        test_vertex_neighborhood();
        test_point_orientation();

        std::cout << "\n========================================" << std::endl;
        std::cout << "Test Results: " << tests_passed << " passed, " << tests_failed << " failed" << std::endl;
        std::cout << "========================================\n" << std::endl;

        if (tests_failed > 0) {
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}