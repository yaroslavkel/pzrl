#include "min_circle.h"
#include <optional>

static constexpr double EPS = 1e-8;

// ── Point2D ──────────────────────────────────────────────────────────────────

bool Point2D::operator==(const Point2D& other) const {
    return std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS;
}

// ── Circle ───────────────────────────────────────────────────────────────────

bool Circle::contains(const Point2D& point) const {
    return distance(center, point) <= radius + EPS;
}

bool Circle::contains(const LineSegment& segment) const {
    return contains(segment.start) && contains(segment.end);
}

// Convexity guarantees the interior, so checking endpoints + midpoint suffices.
bool Circle::containsEntireSegment(const LineSegment& segment) const {
    return contains(segment.start) && contains(segment.end) && contains(findMidPoint(segment));
}

// ── Geometry helpers ─────────────────────────────────────────────────────────

double distanceSquared(const Point2D& p1, const Point2D& p2) {
    double dx = p1.x - p2.x, dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

double distance(const Point2D& p1, const Point2D& p2) {
    return std::sqrt(distanceSquared(p1, p2));
}

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2) {
    return {{(p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0}, distance(p1, p2) / 2.0};
}

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    double ax = p1.x, ay = p1.y;
    double bx = p2.x, by = p2.y;
    double cx = p3.x, cy = p3.y;

    double D = 2.0 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));

    if (std::abs(D) < EPS) {
        // Collinear: build from the two most distant points.
        double d12 = distanceSquared(p1, p2);
        double d13 = distanceSquared(p1, p3);
        double d23 = distanceSquared(p2, p3);
        if (d12 >= d13 && d12 >= d23) return circleFromDiameter(p1, p2);
        if (d13 >= d12 && d13 >= d23) return circleFromDiameter(p1, p3);
        return circleFromDiameter(p2, p3);
    }

    double a2 = ax * ax + ay * ay;
    double b2 = bx * bx + by * by;
    double c2 = cx * cx + cy * cy;

    double ux = (a2 * (by - cy) + b2 * (cy - ay) + c2 * (ay - by)) / D;
    double uy = (a2 * (cx - bx) + b2 * (ax - cx) + c2 * (bx - ax)) / D;

    Point2D center = {ux, uy};
    return {center, distance(center, p1)};
}

// ── Welzl's algorithm ────────────────────────────────────────────────────────

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n) {
    if (n == 0 || boundary.size() == 3) {
        switch (boundary.size()) {
            case 0: return {{0.0, 0.0}, -1.0};
            case 1: return {boundary[0], 0.0};
            case 2: return circleFromDiameter(boundary[0], boundary[1]);
            default: return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
        }
    }

    Point2D p = points[n - 1];
    Circle d = minCircleHelper(points, boundary, n - 1);

    if (d.isValid() && d.contains(p))
        return d;

    boundary.push_back(p);
    return minCircleHelper(points, boundary, n - 1);
}

Circle minCircle(std::vector<Point2D> points) {
    if (points.empty()) return {{0.0, 0.0}, -1.0};
    std::shuffle(points.begin(), points.end(), std::mt19937{std::random_device{}()});
    return minCircleHelper(points, {}, points.size());
}

// ── Segment helpers ───────────────────────────────────────────────────────────

Point2D findMidPoint(const LineSegment& segment) {
    return {(segment.start.x + segment.end.x) / 2.0,
            (segment.start.y + segment.end.y) / 2.0};
}

static std::optional<Point2D> segmentIntersection(const LineSegment& s1, const LineSegment& s2) {
    double x1 = s1.start.x, y1 = s1.start.y, x2 = s1.end.x, y2 = s1.end.y;
    double x3 = s2.start.x, y3 = s2.start.y, x4 = s2.end.x, y4 = s2.end.y;

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (std::abs(denom) < EPS) return std::nullopt;

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

    if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS)
        return Point2D{x1 + t * (x2 - x1), y1 + t * (y2 - y1)};

    return std::nullopt;
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments) {
    std::vector<Point2D> points;

    for (const auto& seg : segments) {
        points.push_back(seg.start);
        points.push_back(seg.end);
        points.push_back(findMidPoint(seg));
    }

    for (size_t i = 0; i < segments.size(); ++i)
        for (size_t j = i + 1; j < segments.size(); ++j)
            if (auto pt = segmentIntersection(segments[i], segments[j]))
                points.push_back(*pt);

    return points;
}

// ── Main API ─────────────────────────────────────────────────────────────────

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments) {
    if (segments.empty())
        return {{0.0, 0.0}, -1.0};

    if (segments.size() == 1)
        return circleFromDiameter(segments[0].start, segments[0].end);

    return minCircle(extractPointsFromSegments(segments));
}
