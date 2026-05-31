#include "min_circle.h"
#include <iostream>
#include <cmath>
#include <string>

static constexpr double EPS = 1e-6;

static int passed = 0, failed = 0;

static bool approxEq(double a, double b) { return std::abs(a - b) < EPS; }
static bool approxEq(Point2D a, Point2D b) { return approxEq(a.x, b.x) && approxEq(a.y, b.y); }

static void check(bool cond, const std::string& name) {
    if (cond) { std::cout << "[PASS] " << name << "\n"; ++passed; }
    else       { std::cout << "[FAIL] " << name << "\n"; ++failed; }
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {
    // ── Primitive helpers ────────────────────────────────────────────────────

    check(approxEq(distance({0,0}, {3,4}), 5.0),  "distance({0,0},{3,4}) = 5");
    check(approxEq(distanceSquared({0,0}, {3,4}), 25.0), "distanceSquared = 25");

    // circleFromDiameter
    {
        Circle c = circleFromDiameter({0,0}, {4,0});
        check(approxEq(c.center, {2,0}) && approxEq(c.radius, 2.0),
              "circleFromDiameter({0,0},{4,0})");
    }

    // circleFrom3Points — right triangle, hypotenuse is diameter
    {
        Circle c = circleFrom3Points({0,0}, {4,0}, {0,3});
        check(approxEq(c.center, {2, 1.5}) && approxEq(c.radius, 2.5),
              "circleFrom3Points right triangle");
    }

    // circleFrom3Points — collinear falls back to diameter of extreme pair
    {
        Circle c = circleFrom3Points({0,0}, {3,0}, {6,0});
        check(approxEq(c.center, {3,0}) && approxEq(c.radius, 3.0),
              "circleFrom3Points collinear");
    }

    // ── Point2D operators ────────────────────────────────────────────────────

    Point2D p1{1.0, 2.0}, p2{1.0, 2.0}, p3{1.0, 3.0};
    check(p1 == p2, "Point2D equality");
    check(p1 != p3, "Point2D inequality");

    // ── Circle::contains ────────────────────────────────────────────────────

    {
        Circle c{{0,0}, 5.0};
        check( c.contains(Point2D{3,4}),  "Circle contains boundary point");
        check( c.contains(Point2D{0,0}),  "Circle contains center");
        check(!c.contains(Point2D{3,5}),  "Circle excludes outside point");

        LineSegment inside{{-3,0},{3,0}};
        LineSegment outside{{-3,0},{0,7}};   // (0,7) is at distance 7 > 5
        check( c.contains(inside),  "Circle contains inner segment");
        check(!c.contains(outside), "Circle excludes segment crossing boundary");

        check( c.containsEntireSegment(inside),  "containsEntireSegment — inside");
        check(!c.containsEntireSegment(outside), "containsEntireSegment — outside");
    }

    // ── findMidPoint ────────────────────────────────────────────────────────

    {
        LineSegment seg{{0,0},{4,6}};
        Point2D mid = findMidPoint(seg);
        check(approxEq(mid, {2,3}), "findMidPoint");
    }

    // ── MinimumEnclosingCircleForSegments ────────────────────────────────────

    // Empty input → invalid circle
    {
        Circle c = MinimumEnclosingCircleForSegments({});
        check(!c.isValid(), "Empty → invalid circle");
    }

    // Single degenerate segment (point)
    {
        Circle c = MinimumEnclosingCircleForSegments({{{2,2},{2,2}}});
        check(c.isValid() && approxEq(c.center, {2,2}) && approxEq(c.radius, 0.0),
              "Single-point segment → radius 0");
    }

    // Single horizontal segment
    {
        Circle c = MinimumEnclosingCircleForSegments({{{0,0},{4,0}}});
        check(approxEq(c.center, {2,0}) && approxEq(c.radius, 2.0),
              "Single segment → diameter circle");
        check(c.contains({{0,0},{4,0}}), "Single segment circle contains its segment");
    }

    // Two segments — T-shape: (0,0)-(4,0) and (2,0)-(2,3)
    // MEC is circumcircle of {(0,0),(4,0),(2,3)}: center=(2, 5/6), r=13/6
    {
        LineSegment s1{{0,0},{4,0}}, s2{{2,0},{2,3}};
        Circle c = MinimumEnclosingCircleForSegments({s1, s2});
        double cx = 2.0, cy = 5.0/6.0, r = 13.0/6.0;
        check(c.isValid(), "T-shape → valid");
        check(approxEq(c.center.x, cx) && approxEq(c.center.y, cy) && approxEq(c.radius, r),
              "T-shape → exact circumcircle");
        check(c.contains(s1) && c.contains(s2), "T-shape circle contains both segments");
    }

    // Two intersecting segments — plus sign: (-1,0)-(1,0) and (0,-1)-(0,1)
    // All endpoints at distance 1 from origin → center (0,0), r=1
    {
        LineSegment s1{{-1,0},{1,0}}, s2{{0,-1},{0,1}};
        Circle c = MinimumEnclosingCircleForSegments({s1, s2});
        check(approxEq(c.center, {0,0}) && approxEq(c.radius, 1.0),
              "Plus sign → center (0,0) r=1");
        check(c.contains(s1) && c.contains(s2), "Plus sign circle contains both segments");
    }

    // Triangle of segments: vertices (0,0),(2,0),(1,2)
    // Circumcircle: center=(1, 0.75), r=1.25
    {
        LineSegment s1{{0,0},{2,0}}, s2{{2,0},{1,2}}, s3{{1,2},{0,0}};
        Circle c = MinimumEnclosingCircleForSegments({s1, s2, s3});
        check(approxEq(c.center.x, 1.0) && approxEq(c.center.y, 0.75) && approxEq(c.radius, 1.25),
              "Triangle → circumcircle center=(1,0.75) r=1.25");
        check(c.contains(s1) && c.contains(s2) && c.contains(s3),
              "Triangle circle contains all three sides");
    }

    // Collinear segments: (0,0)-(2,0) and (3,0)-(6,0)
    // MEC: diameter from (0,0) to (6,0) → center=(3,0), r=3
    {
        LineSegment s1{{0,0},{2,0}}, s2{{3,0},{6,0}};
        Circle c = MinimumEnclosingCircleForSegments({s1, s2});
        check(approxEq(c.center, {3,0}) && approxEq(c.radius, 3.0),
              "Collinear segments → center=(3,0) r=3");
        check(c.contains(s1) && c.contains(s2), "Collinear circle contains both segments");
    }

    // Segments in different quadrants: diagonal cross
    {
        LineSegment s1{{-3,-3},{3,3}}, s2{{-3,3},{3,-3}};
        Circle c = MinimumEnclosingCircleForSegments({s1, s2});
        double expected_r = std::sqrt(18.0); // dist from origin to (3,3)
        check(approxEq(c.center, {0,0}) && approxEq(c.radius, expected_r),
              "Diagonal cross → center=(0,0) r=3√2");
        check(c.contains(s1) && c.contains(s2), "Diagonal cross circle contains both segments");
    }

    // ── Summary ──────────────────────────────────────────────────────────────

    std::cout << "\n" << passed << "/" << (passed + failed) << " tests passed.\n";
    return (failed == 0) ? 0 : 1;
}
