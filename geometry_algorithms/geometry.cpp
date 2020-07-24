#include "geometry.h"

using namespace geometry;

Vector::Vector() : Vector(0, 0) {
}

Vector::Vector(long long x, long long y) : X_(x), Y_(y) {
}

Vector::Vector(const Point& A, const Point& B) :
        Vector(B.GetX() - A.GetX(), B.GetY() - A.GetY()) {
}

Vector::Vector(const Vector& other) = default;

Vector& Vector::operator=(const Vector& other) = default;

Vector Vector::operator+() const {
    return *this;
}

Vector Vector::operator-() const {
    return Vector(-X_, -Y_);
}

Vector& Vector::operator+=(const Vector& other) {
    if (&other == this) {
        X_ *= 2;
        Y_ *= 2;
        return *this;
    }

    X_ += other.GetX();
    Y_ += other.GetY();
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    if (&other == this) {
        X_ = 0;
        Y_ = 0;
        return *this;
    }

    X_ -= other.GetX();
    Y_ -= other.GetY();
    return *this;
}

Vector& Vector::operator*=(long long number) {
    X_ *= number;
    Y_ *= number;
    return *this;
}

Vector& Vector::operator/=(long long number) {
    X_ /= number;
    Y_ /= number;
    return *this;
}

long long Vector::GetX() const {
    return X_;
}

long long Vector::GetY() const {
    return Y_;
}

long double Vector::GetLength() const {
    return sqrtl(powl(X_, 2) + powl(Y_, 2));
}

const Vector geometry::operator+(Vector lhs, const Vector& rhs) {
    return lhs += rhs;
}

const Vector geometry::operator-(Vector lhs, const Vector& rhs) {
    return lhs -= rhs;
}

const Vector geometry::operator*(long long number, Vector rhs) {
    return rhs *= number;
}

const Vector geometry::operator*(Vector lhs, long long number) {
    return lhs *= number;
}

long long geometry::VectorProduct(const Vector& lhs, const Vector& rhs) {
    const long long lhs_x = lhs.GetX();
    const long long lhs_y = lhs.GetY();
    const long long rhs_x = rhs.GetX();
    const long long rhs_y = rhs.GetY();

    return lhs_x * rhs_y - lhs_y * rhs_x;
}

long long geometry::DotProduct(const Vector& lhs, const Vector& rhs) {
    const long long lhs_x = lhs.GetX();
    const long long lhs_y = lhs.GetY();
    const long long rhs_x = rhs.GetX();
    const long long rhs_y = rhs.GetY();

    return lhs_x * rhs_x + lhs_y * rhs_y;
}

long long geometry::AngleSign(const Vector& lhs, const Vector& rhs) {
    const long long cross_prod = VectorProduct(lhs, rhs);

    if (cross_prod > 0) {
        return 1;
    } else if (cross_prod < 0) {
        return -1;
    } else {
        return 0;
    }
}

// Point Implementation
Point::Point() : Point(0, 0) {
}

Point::Point(const Point& other) = default;

Point& Point::operator=(const Point& other) = default;

Point::~Point() = default;

Point::Point(long long x, long long y) : X_(x), Y_(y) {
}

long long Point::GetX() const {
    return X_;
}

long long Point::GetY() const {
    return Y_;
}

bool Point::ContainsPoint(const Point& other) const {
    return (X_ == other.X_ && Y_ == other.Y_);
}

bool Point::CrossSegment(const Segment& other) const {
    return other.ContainsPoint(*this);
}

Point* Point::Move(const Vector& vec) const {
    return new Point(X_ + vec.GetX(), Y_ + vec.GetY());
}

Point* Point::Clone() const {
    return Move({0, 0});
}

void Point::Print() const {
    std::cout << X_ << ' ' << Y_;
}

bool geometry::operator<(const Point& A, const Point& B) {
    if (A.GetX() != B.GetX()) {
        return A.GetX() < B.GetX();
    }

    return A.GetY() < B.GetY();
}

bool geometry::operator>(const Point& A, const Point& B) {
    return B < A;
}

bool geometry::operator<=(const Point& A, const Point& B) {
    return !(A > B);
}

bool geometry::operator>=(const Point& A, const Point& B) {
    return !(A < B);
}

bool geometry::operator==(const Point& A, const Point& B) {
    return A.GetX() == B.GetX() && A.GetY() == B.GetY();
}

bool geometry::operator!=(const Point& A, const Point& B) {
    return !(A == B);
}

// Segment Implementation
Segment::Segment() = default;

Segment::Segment(const Point& A, const Point& B) : A_(A), B_(B) {
}

Segment::Segment(const Segment& other) = default;

Segment& Segment::operator=(const Segment& other) = default;

Segment::~Segment() = default;

const Point Segment::GetA() const {
    return A_;
}

const Point Segment::GetB() const {
    return B_;
}

long double Segment::GetLength() const {
    return Vector(A_, B_).GetLength();
}

bool Segment::ContainsPoint(const Point& A) const {
    if (A_ == A || B_ == A) {
        return true;
    }

    Vector tmp_a(A_, A);
    Vector tmp_b(B_, A);
    Vector vec_ab(A_, B_);

    return (VectorProduct(tmp_a, vec_ab) == 0) &&
           (DotProduct(tmp_a, vec_ab) > 0) &&
           (DotProduct(vec_ab, tmp_b) < 0);
}

bool OnSegmentProjection(const Segment& AB, const Point& C) {
    return std::min(AB.GetA().GetX(), AB.GetB().GetX()) <= C.GetX() &&
           std::max(AB.GetA().GetX(), AB.GetB().GetX()) >= C.GetX() &&
           std::min(AB.GetA().GetY(), AB.GetB().GetY()) <= C.GetY() &&
           std::max(AB.GetA().GetY(), AB.GetB().GetY()) >= C.GetY();
}

long long Direction(const Point& A, const Point& B, const Point& C) {
    return AngleSign({A, C}, {A, B});
}

bool Segment::CrossSegment(const Segment& AB) const {
    long long other_a = Direction(AB.GetA(), AB.GetB(), A_);
    long long other_b = Direction(AB.GetA(), AB.GetB(), B_);
    long long this_a = Direction(A_, B_, AB.GetA());
    long long this_b = Direction(A_, B_, AB.GetB());

    return (other_a != other_b && this_a != this_b) ||
           (other_a == 0 && OnSegmentProjection(AB, A_)) ||
           (other_a == 0 && OnSegmentProjection(AB, A_)) ||
           (other_b == 0 && OnSegmentProjection(AB, B_)) ||
           (this_a == 0 && OnSegmentProjection(*this, AB.GetA())) ||
           (this_b == 0 && OnSegmentProjection(*this, AB.GetB()));
}

Segment* Segment::Clone() const {
    return Move({0, 0});
}

Segment* Segment::Move(const Vector& vec) const {
    return new Segment({A_.GetX() + vec.GetX(), A_.GetY() + vec.GetY()},
                       {B_.GetX() + vec.GetX(), B_.GetY() + vec.GetY()});
}

void Segment::Print() const {
    A_.Print();
    std::cout << ' ';
    B_.Print();
}

// Line Implementation
Line::Line() = default;

Line::Line(const Point& A, const Point& B) : A_(A), B_(B) {
}

Line::Line(const Line& other) = default;

Line& Line::operator=(const Line& other) = default;

Line::~Line() = default;

const Point Line::GetA() const {
    return A_;
}

const Point Line::GetB() const {
    return B_;
}

Vector Line::GetDirection() const {
    return Vector(A_, B_);
}

bool Line::ContainsPoint(const Point& A) const {
    return (VectorProduct({A_, A}, GetDirection()) == 0);
}

bool Line::CrossSegment(const Segment& AB) const {
    if (ContainsPoint(AB.GetA()) || ContainsPoint(AB.GetB())) {
        return true;
    }

    return AngleSign(GetDirection(), {A_, AB.GetA()}) !=
           AngleSign(GetDirection(), {A_, AB.GetB()});
}

Line* Line::Clone() const {
    return Move({0, 0});
}

Line* Line::Move(const Vector& vec) const {
    return new Line({A_.GetX() + vec.GetX(), A_.GetY() + vec.GetY()},
                    {B_.GetX() + vec.GetX(), B_.GetY() + vec.GetY()});
}

void Line::Print() const {
    A_.Print();
    std::cout << ' ';
    B_.Print();
}

// Ray Implementation
Ray::Ray() = default;

Ray::Ray(const Point& A, const Point& B) : A_(A), B_(B) {
}

Ray::Ray(const Ray& other) = default;

Ray& Ray::operator=(const Ray& other) = default;

Ray::~Ray() = default;

const Point Ray::GetA() const {
    return A_;
}

const Point Ray::GetB() const {
    return B_;
}

Vector Ray::GetDirection() const {
    return Vector(A_, B_);
}

bool Ray::ContainsPoint(const Point& A) const {
    return VectorProduct(Vector(A_, A), GetDirection()) == 0 &&
           DotProduct(Vector(A_, A), Vector(A_, B_)) >= 0;
}

bool Ray::CrossSegment(const Segment& AB) const {
    if (ContainsPoint(AB.GetA()) || ContainsPoint(AB.GetB())) {
        return true;
    }

    if (!Segment(A_, B_).CrossSegment(AB)) {
        if (AngleSign(GetDirection(), {A_, AB.GetA()}) !=
            AngleSign(GetDirection(), {A_, AB.GetB()})) {
            return !(DotProduct(GetDirection(), {A_, AB.GetB()}) < 0 ||
                     DotProduct(GetDirection(), {A_, AB.GetA()}) < 0);
        } else {
            return false;
        }
    } else {
        return true;
    }
}

Ray* Ray::Clone() const {
    return Move({0, 0});
}

Ray* Ray::Move(const Vector& vec) const {
    return new Ray({A_.GetX() + vec.GetX(), A_.GetY() + vec.GetY()},
                   {B_.GetX() + vec.GetX(), B_.GetY() + vec.GetY()});
}

void Ray::Print() const {
    A_.Print();
    std::cout << ' ';
    B_.Print();
}

// Polygon Implementation
Polygon::Polygon() = default;

Polygon::Polygon(const Polygon& other) = default;

Polygon::Polygon(const std::vector<Point>& points, size_t vertex_num) : vertices_(points),
                                                                        vertex_count_(vertex_num) {
}

Polygon& Polygon::operator=(const Polygon& other) = default;

Polygon::~Polygon() = default;

size_t Polygon::GetSize() const {
    return vertex_count_;
}

const std::vector<Point>& Polygon::GetVertices() const {
    return vertices_;
}

bool Polygon::IsConvex() const {
    bool answer = true;
    long long previus_angle_sign = 0;

    for (size_t i = 0; i < vertex_count_; ++i) {
        Vector angle_side_A;

        if (i == 0) {
            angle_side_A = {vertices_[0], vertices_[vertex_count_ - 1]};
        } else {
            angle_side_A = {vertices_[i], vertices_[i - 1]};
        }

        Vector angle_side_B;

        if (i == vertex_count_ - 1) {
            angle_side_B = {vertices_[vertex_count_ - 1], vertices_[0]};
        } else {
            angle_side_B = {vertices_[i], vertices_[i + 1]};
        }

        long long current_angle_sign = AngleSign(angle_side_A, angle_side_B);

        if (current_angle_sign != 0) {
            if (previus_angle_sign != 0) {
                if (current_angle_sign != previus_angle_sign) {
                    answer = false;
                    break;
                }
            } else {
                previus_angle_sign = current_angle_sign;
            }
        }
    }

    if (previus_angle_sign == 0) {
        answer = false;
    }

    return answer;
}

bool Polygon::ContainsPoint(const Point& A) const {
    size_t cross_count = 0;
    Ray ray(A, {A.GetX() + 1, A.GetY()});
    for (size_t i = 0; i < vertex_count_; ++i) {
        Segment tmp_seg = {
                vertices_[i],
                vertices_[(i + 1) % vertex_count_]
        };

        if (tmp_seg.ContainsPoint(A)) {
            return true;
        }

        if (tmp_seg.GetA().GetY() == tmp_seg.GetB().GetY() ||
            A.GetY() == std::min(tmp_seg.GetA().GetY(), tmp_seg.GetB().GetY())) {
            continue;
        }

        if (A.GetY() == std::max(tmp_seg.GetA().GetY(), tmp_seg.GetB().GetY()) &&
            A.GetX() < std::min(tmp_seg.GetA().GetX(), tmp_seg.GetB().GetX())) {
            ++cross_count;
        }

        if (A.GetY() < std::max(tmp_seg.GetA().GetY(), tmp_seg.GetB().GetY()) &&
            A.GetY() > std::min(tmp_seg.GetA().GetY(), tmp_seg.GetB().GetY()) &&
            ray.CrossSegment(tmp_seg)) {
            ++cross_count;
        }
    }
    return cross_count % 2;
}

long double Polygon::GetArea() const {
    long double area = 0.0;

    size_t j = vertex_count_ - 1;
    for (size_t i = 0; i < vertex_count_; ++i) {
        area += static_cast<long double>((vertices_[j].GetX() + vertices_[i].GetX()) *
                                         (vertices_[j].GetY() - vertices_[i].GetY()));

        j = i;
    }

    return std::abs(area / 2.0);
}

bool Polygon::CrossSegment(const Segment& AB) const {
    for (size_t i = 0; i < vertex_count_; ++i) {
        Segment tmp_seg = {
                vertices_[i],
                vertices_[(i + 1) % vertex_count_]
        };

        if (tmp_seg.CrossSegment(AB)) {
            return true;
        }
    }
    return false;
}

Polygon* Polygon::Move(const Vector& vec) const {
    auto* new_poly = new Polygon;

    for (size_t i = 0; i < vertex_count_; ++i) {
        long long new_X = vertices_[i].GetX() + vec.GetX();
        long long new_Y = vertices_[i].GetY() + vec.GetY();

        new_poly->vertices_.emplace_back(new_X, new_Y);
    }

    new_poly->vertex_count_ = vertex_count_;
    return new_poly;
}

Polygon* Polygon::Clone() const {
    return Move(Vector(0, 0));
}

void Polygon::Print() const {
    std::cout << vertex_count_ << '\n';
    for (const auto& vertex : vertices_) {
        vertex.Print();
        std::cout << ' ';
    }
}

long double geometry::PointPointDist(const Point& A, const Point& B) {
    return sqrtl(static_cast<long double>(powl(A.GetX() - B.GetX(), 2)) +
                 static_cast<long double>(powl(A.GetY() - B.GetY(), 2)));
}

long double geometry::PointLineDist(const Line& m, const Point& A) {
    long double A_x = static_cast<long double>(m.GetA().GetX());
    long double A_y = static_cast<long double>(m.GetA().GetY());
    long double B_x = static_cast<long double>(m.GetB().GetX());
    long double B_y = static_cast<long double>(m.GetB().GetY());
    auto C_y = static_cast<long double>(A.GetY());
    auto C_x = static_cast<long double>(A.GetX());

    long double dist_1 = std::abs((B_y - A_y) * C_x + (A_x - B_x) * C_y + A_y * B_x - A_x * B_y);
    dist_1 /= sqrtl(powl(B_y - A_y, 2) + powl(B_x - A_x, 2));

    return dist_1;
}

long double geometry::PointRayDist(const Ray& ray, const Point& A) {
    long double dist;

    if (DotProduct(Vector(ray.GetA(), A),
                   ray.GetDirection()) >= 0) {
        dist = PointLineDist(Line(ray.GetA(), ray.GetB()), A);
    } else {
        dist = PointPointDist(ray.GetA(), A);
    }

    return dist;
}

long double geometry::PointSegmentDist(const Segment& seg, const Point& A) {
    long double dist;

    if (DotProduct(Vector(seg.GetA(), A),
                   Vector(seg.GetA(), seg.GetB())) >= 0 &&
        DotProduct(Vector(seg.GetB(), A),
                   Vector(seg.GetA(), seg.GetB())) <= 0) {
        dist = PointLineDist(Line(seg.GetA(), seg.GetB()), A);
    } else if (DotProduct(Vector(seg.GetA(), A),
                          Vector(seg.GetA(), seg.GetB())) < 0) {
        dist = PointPointDist(seg.GetA(), A);
    } else {
        dist = PointPointDist(seg.GetB(), A);
    }

    return dist;
}

// Circle Implementation
Circle::Circle() = default;

Circle::Circle(const Circle& other) = default;

Circle::Circle(const Point& center, size_t radius) : centre_(center), radius_(radius) {
}

Circle& Circle::operator=(const Circle& other) = default;

Circle::~Circle() = default;

size_t Circle::GetRadius() const {
    return radius_;
}

const Point& Circle::GetCentre() const {
    return centre_;
}

long double Circle::GetArea() const {
    return PI * static_cast<long double>(powl(radius_, 2));
}

long double Circle::GetLength() const {
    return 2.0 * PI * static_cast<long double>(radius_);
}

bool Circle::ContainsPoint(const Point& A) const {
    return std::abs(PointPointDist(A, centre_) - static_cast<long double>(radius_)) < kEpsilon ||
           PointPointDist(A, centre_) < static_cast<long double>(radius_);
}

bool Circle::CrossSegment(const Segment& AB) const {
    return (std::abs(PointSegmentDist(AB, centre_) - static_cast<long double>(radius_)) < kEpsilon ||
            PointSegmentDist(AB, centre_) < static_cast<long double>(radius_)) &&
           (PointPointDist(centre_, AB.GetA()) >= static_cast<long double>(radius_) ||
            PointPointDist(centre_, AB.GetB()) >= static_cast<long double>(radius_));
}

Circle* Circle::Move(const Vector& vec) const {
    return new Circle({centre_.GetX() + vec.GetX(),
                       centre_.GetY() + vec.GetY()}, radius_);
}

Circle* Circle::Clone() const {
    return Move({0, 0});
}

void Circle::Print() const {
    centre_.Print();
    std::cout << ' ' << radius_;
}