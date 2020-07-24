#include "geometry.h"

using namespace geometry;

Point pivot;

long long DistSqr(const Point& A, const Point& B) {
    long long dx = A.GetX() - B.GetX();
    long long dy = A.GetY() - B.GetY();
    return dx * dx + dy * dy;
}

bool PolarOrder(const Point& A, const Point& B) {
    Vector p(A, pivot);
    Vector q(B, pivot);

    if (AngleSign(p, q) == 0) {
        return DistSqr(pivot, A) < DistSqr(pivot, B);
    }

    return AngleSign(p, q) > 0;
}

bool ToLeft(const Point& A, const Point& B, const Point& C) {
    Point p = {B.GetX() - A.GetX(), B.GetY() - A.GetY()};
    Point q = {C.GetX() - B.GetX(), C.GetY() - B.GetY()};

    return p.GetX() * q.GetY() > q.GetX() * p.GetY();
}

std::vector<Point> FindConvexHull(std::vector<Point>& points) {
    std::vector<Point> convex_hull;

    size_t min_index = 0;
    size_t N = points.size();

    for (size_t i = 1; i < N; ++i) {
        if (points[i] < points[min_index]) {
            min_index = i;
        }
    }

    std::swap(points[0], points[min_index]);
    pivot = points[0];
    std::sort(points.begin() + 1, points.end(), PolarOrder);

    size_t current = 1;

    for (size_t i = 2; i <= N; ++i) {
        while (current > 0 && !ToLeft(points[current - 1], points[current], points[i % N])) {
            --current;
        }

        points[(++current) % N] = points[i % N];
    }

    convex_hull.reserve(current);
    for (size_t j = current; j >= 1; --j) {
        convex_hull.push_back(points[j % N]);
    }

    return convex_hull;
}

struct Sight {
    Point point;
    int zone;

    Sight(const Point& a, size_t i) : point(a), zone(i) {
    }
};

bool HullContainsPoint(const std::vector<Point>& hull, const Point& A) {
    const size_t size = hull.size();

    for (size_t i = 0; i < size; ++i) {
        Segment tmp_seg = {hull[i], hull[(i + 1) % size]};
        if (tmp_seg.ContainsPoint(A)) {
            return true;
        }
    }

    return false;
}

int main() {
    size_t N;
    std::cin >> N;
    std::vector<Point> points;
    std::vector<int> answer(N, -1);
    points.reserve(N);

    for (size_t i = 0; i < N; ++i) {
        long long x, y;
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }

    std::vector<Point> rest = points;
    size_t i = 0;
    while (!rest.empty()) {
        std::vector<Point> vertices = FindConvexHull(rest);

        std::vector<Point> tmp;
        Polygon convex_pol(vertices, vertices.size());

        if (convex_pol.GetArea() < kEpsilon) {
            --i;
        }

        for (size_t j = 0; j < points.size(); ++j) {
            if (answer[j] == -1 && HullContainsPoint(vertices, points[j])) {
                answer[j] = i;
            } else if (answer[j] == -1) {
                tmp.emplace_back(points[j]);
            }
        }

        rest = tmp;
        ++i;
    }

    for (int k : answer) {
        std::cout << k << '\n';
    }
    return 0;
}