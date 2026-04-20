#include "point.h"
#include "tin.h"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace T_MESH;

void test_point_basics() {
    std::cout << "Testing Point basics..." << std::endl;
    
    Point p1(1.0, 2.0, 3.0);
    Point p2(4.0, 5.0, 6.0);
    
    // Addition
    Point p3 = p1 + p2;
    assert(p3.x == 5.0);
    assert(p3.y == 7.0);
    assert(p3.z == 9.0);
    (void)p3;
    
    // Subtraction
    Point p4 = p2 - p1;
    assert(p4.x == 3.0);
    assert(p4.y == 3.0);
    assert(p4.z == 3.0);
    (void)p4;
    
    // Dot Product
    coord dot = p1 * p2;
    assert(dot == (1.0*4.0 + 2.0*5.0 + 3.0*6.0));
    (void)dot;
    
    // Cross Product
    Point cross = p1 & p2;
    // (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4) = (12-15, 12-6, 5-8) = (-3, 6, -3)
    assert(cross.x == -3.0);
    assert(cross.y == 6.0);
    assert(cross.z == -3.0);
    (void)cross;
    
    // Scalar multiplication
    Point p5 = p1 * 2.0;
    assert(p5.x == 2.0);
    assert(p5.y == 4.0);
    assert(p5.z == 6.0);
    (void)p5;

    std::cout << "Point basics passed!" << std::endl;
}

void test_point_distance() {
    std::cout << "Testing Point distance..." << std::endl;
    
    Point p1(0.0, 0.0, 0.0);
    Point p2(3.0, 4.0, 0.0);
    
    assert(p1.squaredDistance(&p2) == 25.0);
    assert(p1.distance(p2) == 5.0);
    
    std::cout << "Point distance passed!" << std::endl;
}

void test_repair_report() {
    std::cout << "Testing RepairReport..." << std::endl;

    RepairReport report;
    assert(!report.repairedSomething());

    report.patched_holes = 1;
    assert(report.repairedSomething());

    report = RepairReport{};
    report.degeneracies_before = 3;
    report.degeneracies_after = 1;
    assert(report.repairedSomething());

    report = RepairReport{};
    report.intersections_before = 5;
    report.intersections_after = 5;
    assert(!report.repairedSomething());

    report.intersections_after = 2;
    assert(report.repairedSomething());

    std::cout << "RepairReport passed!" << std::endl;
}

int main() {
    try {
        test_point_basics();
        test_point_distance();
        test_repair_report();
        std::cout << "\nAll unit tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
