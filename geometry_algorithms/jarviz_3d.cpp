#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

struct Point {
  long double x_;
  long double y_;
  long double z_;

  Point() : Point(0, 0, 0) {}
  Point(long double x, long double y, long double z) : x_(x), y_(y), z_(z) {}
};

bool operator<(const Point& lhs, const Point& rhs) {
  return (lhs.z_ < rhs.z_) || (lhs.z_ == rhs.z_ && lhs.y_ < rhs.y_) ||
         (lhs.z_ == rhs.z_ && lhs.y_ == rhs.y_ && lhs.x_ < rhs.x_);
}

bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.z_ == rhs.z_ && lhs.y_ == rhs.y_ && lhs.x_ == rhs.x_);
}

bool operator!=(const Point& lhs, const Point& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& out, const Point& point) {
  out << point.x_ << ' ' << point.y_ << ' ' << point.z_ << '\n';
  return out;
}

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_ >> point.z_;
  return in;
}

using PointArray = std::vector<Point>;

struct Segment {
  Point A_;
  Point B_;

  Segment() = default;
  Segment(Point A, Point B) : A_(A), B_(B) {}
};

bool operator<(const Segment& lhs, const Segment& rhs) {
  return lhs.A_ < rhs.A_ && lhs.B_ < rhs.B_;
}

struct Vector {
  long double x_;
  long double y_;
  long double z_;

  Vector() : Vector(0, 0, 0) {}
  Vector(long double x, long double y, long double z) : x_(x), y_(y), z_(z) {}
  Vector(const Point& A, const Point& B)
      : x_(B.x_ - A.x_), y_(B.y_ - A.y_), z_(B.z_ - A.z_) {}

  long double GetLength() const { return std::hypot(x_, y_, z_); }

  Vector& operator*=(long double num) {
    x_ *= num;
    y_ *= num;
    z_ *= num;
    return *this;
  }
};

Point operator+(const Point& lhs, const Vector& rhs) {
  return {lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_};
}

Point operator+(const Vector& lhs, const Point& rhs) {
  return {lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_};
}

Vector operator+(const Vector& lhs, const Vector& rhs) {
  return {lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_};
}

Vector operator-(const Vector& lhs, Vector rhs) { return lhs + (rhs *= -1.); }

Vector VectorProduct(const Vector& lhs, const Vector& rhs) {
  auto res_x = lhs.y_ * rhs.z_ - rhs.y_ * lhs.z_;
  auto res_y = lhs.z_ * rhs.x_ - rhs.z_ * lhs.x_;
  auto res_z = lhs.x_ * rhs.y_ - rhs.x_ * lhs.y_;
  return {res_x, res_y, res_z};
}

long double DotProduct(const Vector& lhs, const Vector& rhs) {
  return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
}

Vector Projection(const Vector& vec, Vector on) {
  auto coeff = DotProduct(vec, on) / DotProduct(on, on);
  return on *= coeff;
}

Vector Projection(const Vector& vec, const Vector& base_a,
                  const Vector& base_b) {
  return Projection(vec, base_a) + Projection(vec, base_b);
}

Vector Orthogonalize(const Vector& vec, const Vector& with) {
  return vec - Projection(vec, with);
}

template <typename T>
T triple_min(const T& a, const T& b, const T& c) {
  return std::min(std::min(a, b), c);
}

template <typename T>
T triple_max(const T& a, const T& b, const T& c) {
  return std::max(std::max(a, b), c);
}

template <typename T>
T triple_median(const T& a, const T& b, const T& c) {
  if (a > b) {
    if (b > c) {
      return b;
    } else if (a > c) {
      return c;
    } else {
      return a;
    }
  } else {
    if (a > c) {
      return a;
    } else if (b > c) {
      return c;
    } else {
      return b;
    }
  }
}

struct Verge {
  int64_t idx_a_;
  int64_t idx_b_;
  int64_t idx_c_;

  Verge() = default;
  Verge(int64_t idx_a, int64_t idx_b, int64_t idx_c)
      : idx_a_(triple_min(idx_a, idx_b, idx_c)),
        idx_b_(triple_median(idx_a, idx_b, idx_c)),
        idx_c_(triple_max(idx_a, idx_b, idx_c)) {}
};

struct SegmentIndices {
  int64_t idx_a_;
  int64_t idx_b_;

  SegmentIndices() = default;
  SegmentIndices(int64_t idx_a, int64_t idx_b)
      : idx_a_(std::min(idx_a, idx_b)), idx_b_(std::max(idx_a, idx_b)) {}
};

struct CompareSegmentIndices {
  bool operator()(const SegmentIndices& lhs, const SegmentIndices& rhs) const {
    return lhs.idx_a_ < rhs.idx_a_ ||
           (lhs.idx_a_ == rhs.idx_a_ && lhs.idx_b_ < rhs.idx_b_);
  }
};

struct Plane {
  Point A_;
  Point B_;
  Point C_;

  Plane() = default;
  Plane(const Point& A, const Point& B, const Point& C) : A_(A), B_(B), C_(C) {}
};

struct ComparePointsForVerge {
  ComparePointsForVerge() = default;
  ComparePointsForVerge(const Segment& edge, const Point& vertex)
      : edge_(edge), vertex_(vertex) {}

  bool operator()(const Point& lhs, const Point& rhs) const {
    if (lhs == edge_.A_ || lhs == edge_.B_) {
      return false;
    }
    if (rhs == edge_.A_ || rhs == edge_.B_) {
      return true;
    }
    Vector base_a = Orthogonalize({edge_.A_, vertex_}, {edge_.A_, edge_.B_});
    Vector base_b = VectorProduct(base_a, {edge_.A_, edge_.B_});
    auto lhs_proj = Projection({edge_.A_, lhs}, base_a, base_b);
    auto rhs_proj = Projection({edge_.A_, rhs}, base_a, base_b);
    auto cos_base_lhs = DotProduct(lhs_proj, base_a) / lhs_proj.GetLength();
    auto cos_base_rhs = DotProduct(rhs_proj, base_a) / rhs_proj.GetLength();
    return cos_base_lhs < cos_base_rhs;
  }

 private:
  Segment edge_;
  Point vertex_;
};

int64_t GetNextVertex(const Segment& edge, const Point& A,
                      const PointArray& points) {
  auto next_vertex = std::min_element(points.begin(), points.end(),
                                      ComparePointsForVerge{edge, A});
  return next_vertex - points.begin();
}

long double Determinant(const Vector& col_a, const Vector& col_b,
                        const Vector& col_c) {
  return col_a.x_ * (col_b.y_ * col_c.z_ - col_b.z_ * col_c.y_) -
         col_b.x_ * (col_a.y_ * col_c.z_ - col_a.z_ * col_c.y_) +
         col_c.x_ * (col_a.y_ * col_b.z_ - col_a.z_ * col_b.y_);
}

int64_t FindOuterVertex(const Verge& verge, const PointArray& points) {
  for (size_t i = 0; i < points.size(); ++i) {
    if (points[i] != points[verge.idx_a_] &&
        points[i] != points[verge.idx_b_] &&
        points[i] != points[verge.idx_c_]) {
      return i;
    }
  }
}

Verge ConstructInitialVerge(const PointArray& points) {
  auto initial_point_it = std::min_element(points.begin(), points.end());
  int64_t init_idx = initial_point_it - points.begin();
  Plane lowest_plane = {
      points[init_idx],
      {points[init_idx].x_, points[init_idx].y_ + 1, points[init_idx].z_},
      {points[init_idx].x_ + 1, points[init_idx].y_, points[init_idx].z_}};
  int64_t second_idx = init_idx;
  int64_t third_idx = init_idx;
  for (size_t i = 0; i < points.size(); ++i) {
    if (points[i].z_ == points[init_idx].z_) {
      if (second_idx == init_idx) {
        second_idx = i;
      } else {
        third_idx = i;
      }
    }
  }
  if (second_idx == init_idx && third_idx == init_idx) {
    second_idx = GetNextVertex({points[init_idx], lowest_plane.B_},
                               lowest_plane.C_, points);
    third_idx = GetNextVertex({points[init_idx], points[second_idx]},
                              lowest_plane.B_, points);

  } else if (second_idx != init_idx && third_idx == init_idx) {
    Point pivot = (lowest_plane.B_ == points[second_idx]) ? lowest_plane.C_
                                                          : lowest_plane.B_;
    third_idx =
        GetNextVertex({points[init_idx], points[second_idx]}, pivot, points);
  }
  return {init_idx, second_idx, third_idx};
}

void NormalizeVerge(Verge& verge, const PointArray& points) {
  auto outer_idx = FindOuterVertex(verge, points);
  Vector norm = {points[verge.idx_a_], points[outer_idx]};
  auto det = Determinant(norm, {points[verge.idx_a_], points[verge.idx_b_]},
                         {points[verge.idx_b_], points[verge.idx_c_]});
  if (det > 0) {
    std::swap(verge.idx_b_, verge.idx_c_);
  }
}

std::vector<Verge> BuildConvexHull(const PointArray& points) {
  auto first_face = ConstructInitialVerge(points);
  NormalizeVerge(first_face, points);
  std::map<SegmentIndices, int64_t, CompareSegmentIndices> dict(
      CompareSegmentIndices{});
  std::vector<Verge> convex_hull{};
  convex_hull.push_back(first_face);
  dict[SegmentIndices{first_face.idx_a_, first_face.idx_b_}] =
      first_face.idx_c_;
  dict[SegmentIndices{first_face.idx_a_, first_face.idx_c_}] =
      first_face.idx_b_;
  dict[SegmentIndices{first_face.idx_b_, first_face.idx_c_}] =
      first_face.idx_a_;
  while (!dict.empty()) {
    auto curr = *dict.begin();
    auto next_vertex =
        GetNextVertex({points[curr.first.idx_a_], points[curr.first.idx_b_]},
                      points[curr.second], points);
    convex_hull.emplace_back(curr.first.idx_a_, curr.first.idx_b_, next_vertex);
    NormalizeVerge(convex_hull[convex_hull.size() - 1], points);
    if (dict.find(SegmentIndices{curr.first.idx_a_, next_vertex}) ==
        dict.end()) {
      dict[SegmentIndices{curr.first.idx_a_, next_vertex}] = curr.first.idx_b_;
    } else {
      dict.erase(SegmentIndices{curr.first.idx_a_, next_vertex});
    }
    if (dict.find(SegmentIndices{curr.first.idx_b_, next_vertex}) ==
        dict.end()) {
      dict[SegmentIndices{curr.first.idx_b_, next_vertex}] = curr.first.idx_a_;
    } else {
      dict.erase(SegmentIndices{curr.first.idx_b_, next_vertex});
    }
    dict.erase(SegmentIndices{curr.first.idx_a_, curr.first.idx_b_});
  }
  return convex_hull;
}

struct CompareVerges {
  bool operator()(const Verge& lhs, const Verge& rhs) const {
    return (lhs.idx_a_ < rhs.idx_a_) ||
           (lhs.idx_a_ == rhs.idx_a_ && lhs.idx_b_ < rhs.idx_b_) ||
           (lhs.idx_a_ == rhs.idx_a_ && lhs.idx_b_ == rhs.idx_b_ &&
            lhs.idx_c_ < rhs.idx_c_);
  }
};

int main() {
  size_t M = 0;
  std::cin >> M;
  for (size_t i = 0; i < M; ++i) {
    size_t N = 0;
    std::cin >> N;
    PointArray points(N);
    for (auto& point : points) {
      std::cin >> point;
    }
    auto result = BuildConvexHull(points);
    std::sort(result.begin(), result.end(), CompareVerges{});
    std::cout << result.size() << '\n';
    for (const auto& verge : result) {
      std::cout << 3 << ' ' << verge.idx_a_ << ' ' << verge.idx_b_ << ' '
                << verge.idx_c_ << '\n';
    }
  }
  return 0;
}
