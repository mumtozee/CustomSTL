#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Point {
  long double x_;
  long double y_;
};

bool operator<(const Point& lhs, const Point& rhs) {
  return (lhs.y_ < rhs.y_) || (lhs.y_ == rhs.y_ && lhs.x_ < rhs.x_);
}

bool operator>(const Point& lhs, const Point& rhs) { return rhs < lhs; }

bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.y_ == rhs.y_) && (lhs.x_ == rhs.x_);
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
  out << point.x_ << ' ' << point.y_ << '\n';
  return out;
}

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_;
  return in;
}

class Vector {
 public:
  Vector();
  Vector(long double x, long double y);
  Vector(const Point& A, const Point& B);
  Vector(const Vector& other);
  Vector& operator=(const Vector& other);
  ~Vector() = default;
  Vector operator+() const;
  Vector operator-() const;
  Vector& operator+=(const Vector& other);
  Vector& operator-=(const Vector& other);
  Vector& operator*=(long double number);
  Vector& operator/=(long double number);
  long double GetX() const;
  long double GetY() const;
  long double GetLength() const;

 private:
  long double x_;
  long double y_;
};

Vector::Vector() : Vector(0.0, 0.0) {}

Vector::Vector(long double x, long double y) : x_(x), y_(y) {}

Vector::Vector(const Point& A, const Point& B)
    : Vector(B.x_ - A.x_, B.y_ - A.y_) {}

Vector::Vector(const Vector& other) = default;

Vector& Vector::operator=(const Vector& other) = default;

Vector Vector::operator+() const { return *this; }

Vector Vector::operator-() const { return Vector(-x_, -y_); }

Vector& Vector::operator+=(const Vector& other) {
  if (&other == this) {
    x_ *= 2;
    y_ *= 2;
    return *this;
  }

  x_ += other.GetX();
  y_ += other.GetY();
  return *this;
}

Vector& Vector::operator-=(const Vector& other) {
  if (&other == this) {
    x_ = 0;
    y_ = 0;
    return *this;
  }

  x_ -= other.GetX();
  y_ -= other.GetY();
  return *this;
}

Vector& Vector::operator*=(long double number) {
  x_ *= number;
  y_ *= number;
  return *this;
}

Vector& Vector::operator/=(long double number) {
  x_ /= number;
  y_ /= number;
  return *this;
}

long double Vector::GetX() const { return x_; }

long double Vector::GetY() const { return y_; }

long double Vector::GetLength() const {
  return sqrtl(powl(x_, 2) + powl(y_, 2));
}

std::ostream& operator<<(std::ostream& out, const Vector& vec) {
  out << vec.GetX() << ' ' << vec.GetY() << '\n';
  return out;
}

Vector operator+(Vector lhs, const Vector& rhs) { return lhs += rhs; }

Vector operator-(Vector lhs, const Vector& rhs) { return lhs -= rhs; }

Vector operator*(long double number, Vector rhs) { return rhs *= number; }

Vector operator*(Vector lhs, long double number) { return lhs *= number; }

Point operator+(const Vector& lhs, const Point& rhs) {
  return {rhs.x_ + lhs.GetX(), rhs.y_ + lhs.GetY()};
}

Point operator+(const Point& lhs, const Vector& rhs) {
  return {lhs.x_ + rhs.GetX(), lhs.y_ + rhs.GetY()};
}

long double VectorProduct(const Vector& lhs, const Vector& rhs) {
  const long double lhs_x = lhs.GetX();
  const long double lhs_y = lhs.GetY();
  const long double rhs_x = rhs.GetX();
  const long double rhs_y = rhs.GetY();

  return lhs_x * rhs_y - lhs_y * rhs_x;
}

bool AngleSign(const Vector& lhs, const Vector& rhs) {
  const long double cross_prod = VectorProduct(lhs, rhs);
  return cross_prod >= 0;
}

struct PolarOrder {
  bool operator()(const Vector& lhs, const Vector& rhs) {
    if (lhs.GetY() > 0 && rhs.GetY() < 0) {
      return true;
    }
    if (lhs.GetY() < 0 && rhs.GetY() > 0) {
      return false;
    }
    if (rhs.GetY() == 0) {
      return (rhs.GetX() < 0 &&
              (lhs.GetY() > 0 || (lhs.GetY() == 0 && lhs.GetX() > 0)));
    }
    if (lhs.GetY() == 0) {
      return (lhs.GetX() > 0 || (lhs.GetX() < 0 && rhs.GetY() < 0));
    }
    return lhs.GetX() * rhs.GetY() > lhs.GetY() * rhs.GetX();
  }
};

template <typename ContainerType>
void PrintContainer(const ContainerType& container) {
  std::copy(
      container.begin(), container.end(),
      std::ostream_iterator<typename ContainerType::value_type>(std::cout));
}

using PointArray = std::vector<Point>;
using VectorArray = std::vector<Vector>;
using Polygon = std::pair<Point, std::vector<Vector>>;

Polygon MakePolygon(const PointArray& points) {
  VectorArray vectors(points.size());
  for (size_t i = 0; i < points.size(); ++i) {
    vectors[i] = {points[i], points[(i + 1) % points.size()]};
  }
  return std::make_pair(points[0], vectors);
}

Polygon MinkowskySum(const Polygon& poly_a, const Polygon& poly_b) {
  Point res_min_point = {poly_a.first.x_ + poly_b.first.x_,
                         poly_a.first.y_ + poly_b.first.y_};
  VectorArray res_vector_array{};
  std::merge(poly_a.second.begin(), poly_a.second.end(), poly_b.second.begin(),
             poly_b.second.end(), std::back_inserter(res_vector_array),
             PolarOrder{});
  return std::make_pair(res_min_point, res_vector_array);
}

bool IsPointInConvexPolygon(const Point& point, const Polygon& poly) {
  const auto psize = poly.second.size();
  auto prev_angle_sign =
      AngleSign({point, poly.first}, {point, poly.first + poly.second[0]});
  auto last_vertex = poly.first + poly.second[0];
  for (size_t i = 1; i <= psize; ++i) {
    auto current_angle_sign = AngleSign(
        {point, last_vertex}, {point, last_vertex + poly.second[i % psize]});
    if (prev_angle_sign ^ current_angle_sign) {
      return false;
    }
    last_vertex = last_vertex + poly.second[i % psize];
  }
  return true;
}

bool PolygonsIntersect(const PointArray& points_a, const PointArray& points_b) {
  auto poly_a = MakePolygon(points_a);
  auto poly_b = MakePolygon(points_b);
  auto mink_sum = MinkowskySum(poly_a, poly_b);
  return IsPointInConvexPolygon({0, 0}, mink_sum);
}

int main() {
  size_t N = 0;
  size_t M = 0;
  std::cin >> N;
  PointArray points_a(N);
  for (size_t i = 0; i < N; ++i) {
    std::cin >> points_a[i];
  }
  std::cin >> M;
  PointArray points_b(M);
  for (size_t i = 0; i < M; ++i) {
    std::cin >> points_b[i];
    points_b[i].x_ *= -1.0;
    points_b[i].y_ *= -1.0;
  }
  std::reverse(points_a.begin(), points_a.end());
  std::reverse(points_b.begin(), points_b.end());
  auto new_begin = std::min_element(points_a.begin(), points_a.end());
  std::rotate(points_a.begin(), new_begin, points_a.end());
  new_begin = std::min_element(points_b.begin(), points_b.end());
  std::rotate(points_b.begin(), new_begin, points_b.end());
  std::cout << (PolygonsIntersect(points_a, points_b) ? "YES" : "NO")
            << std::endl;
  return 0;
}