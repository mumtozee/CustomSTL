#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

struct Point {
  int64_t x_;
  int64_t y_;
};

bool operator<(const Point& lhs, const Point& rhs) {
  return (lhs.x_ < rhs.x_) || (lhs.x_ == rhs.x_ && lhs.y_ < rhs.y_);
}

bool operator>(const Point& lhs, const Point& rhs) { return rhs < lhs; }

std::ostream& operator<<(std::ostream& out, const Point& point) {
  out << point.x_ << ' ' << point.y_ << '\n';
  return out;
}

long double TwoPointDistance(const Point& A, const Point& B) {
  auto delta_x = static_cast<long double>(A.x_ - B.x_);
  auto delta_y = static_cast<long double>(A.y_ - B.y_);
  return sqrtl(delta_x * delta_x + delta_y * delta_y);
}

long double ComputeConvexHullLength(const std::vector<Point>& convex_hull) {
  long double answer = 0.0;
  for (size_t i = 0; i < convex_hull.size(); ++i) {
    answer += TwoPointDistance(convex_hull[i],
                               convex_hull[(i + 1) % convex_hull.size()]);
  }
  return answer;
}

bool LeftOriented(const Point& A, const Point& B, const Point& C) {
  Point P{B.x_ - A.x_, B.y_ - A.y_};
  Point Q{C.x_ - B.x_, C.y_ - B.y_};
  return P.x_ * Q.y_ > Q.x_ * P.y_;
}

bool RightOriented(const Point& A, const Point& B, const Point& C) {
  Point P{B.x_ - A.x_, B.y_ - A.y_};
  Point Q{C.x_ - B.x_, C.y_ - B.y_};
  return P.x_ * Q.y_ < Q.x_ * P.y_;
}

template <typename ContainerType>
void PrintContainer(const ContainerType& container) {
  std::copy(
      container.begin(), container.end(),
      std::ostream_iterator<typename ContainerType::value_type>(std::cout));
}

void SplitPointSet(const std::set<Point>& points,
                   std::vector<Point>& upper_hull,
                   std::vector<Point>& lower_hull) {
  Point left_most = *points.begin();
  Point right_most = *points.rbegin();
  for (const auto& point : points) {
    if (LeftOriented(point, left_most, right_most)) {
      upper_hull.push_back(point);
    } else if (LeftOriented(point, right_most, left_most)) {
      lower_hull.push_back(point);
    }
  }
  upper_hull.push_back(left_most);
  upper_hull.push_back(right_most);
  lower_hull.push_back(left_most);
  lower_hull.push_back(right_most);
}

std::vector<Point> GrahamScan(std::vector<Point>& points) {
  if (points.size() <= 2) {
    return points;
  }
  std::vector<Point> convex_hull{};
  size_t current = 1;
  for (size_t i = 2; i <= points.size(); ++i) {
    while (current > 0 && !LeftOriented(points[current - 1], points[current],
                                        points[i % points.size()])) {
      --current;
    }
    points[(++current) % points.size()] = points[i % points.size()];
  }
  convex_hull.reserve(current);
  for (size_t i = current - 1; i < current; --i) {
    convex_hull.push_back(points[i % points.size()]);
  }
  return convex_hull;
}

std::vector<Point> FindConvexHull(const std::set<Point>& points) {
  std::vector<Point> convex_hull{};
  std::vector<Point> upper_hull{};
  std::vector<Point> lower_hull{};
  SplitPointSet(points, upper_hull, lower_hull);
  std::sort(upper_hull.rbegin(), upper_hull.rend());
  std::sort(lower_hull.begin(), lower_hull.end());
  upper_hull = GrahamScan(upper_hull);
  lower_hull = GrahamScan(lower_hull);
  convex_hull.reserve(upper_hull.size() + lower_hull.size() - 2);
  if (upper_hull.size() <= 2) {
    std::copy(lower_hull.begin(), lower_hull.end(),
              std::back_inserter(convex_hull));
  } else if (lower_hull.size() <= 2) {
    std::copy(upper_hull.begin(), upper_hull.end(),
              std::back_inserter(convex_hull));
  } else {
    std::copy(upper_hull.begin(), upper_hull.end(),
              std::back_inserter(convex_hull));
    std::copy(lower_hull.begin() + 1, lower_hull.end() - 1,
              std::back_inserter(convex_hull));
  }
  return convex_hull;
}

int main() {
  size_t N = 0;
  std::cin >> N;
  std::set<Point> point_set;
  for (size_t i = 0; i < N; ++i) {
    Point point{};
    std::cin >> point.x_ >> point.y_;
    point_set.insert(point);
  }
  auto result = FindConvexHull(point_set);
  std::cout << std::fixed << std::setprecision(10)
            << ComputeConvexHullLength(result) << std::endl;
  return 0;
}