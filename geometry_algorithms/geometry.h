#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <iomanip>
#include <stack>
#include <algorithm>

namespace geometry {
/* constant units */
const long double kEpsilon = 1e-9;
const long double PI = 3.14159265358979323846;

/* class decalarations */
class Vector;

class AbstractShape;

class Point;

class Segment;

class Ray;

class Line;

class Polygon;

class Vector {
public:
    Vector();

    Vector(long long x, long long y);

    Vector(const Point& A, const Point& B);

    Vector(const Vector& other);

    Vector& operator=(const Vector& other);

    ~Vector() = default;

    Vector operator+() const;

    Vector operator-() const;

    Vector& operator+=(const Vector& other);

    Vector& operator-=(const Vector& other);

    Vector& operator*=(long long number);

    Vector& operator/=(long long number);

    long long GetX() const;

    long long GetY() const;

    long double GetLength() const;

private:
    long long X_;
    long long Y_;
};

const Vector operator+(Vector lhs, const Vector& rhs);
const Vector operator-(Vector lhs, const Vector& rhs);
const Vector operator/(Vector lhs, const Vector& rhs);
const Vector operator*(long long number, Vector rhs);
const Vector operator*(Vector lhs, long long number);

class AbstractShape {
public:
    virtual bool ContainsPoint(const Point& A) const = 0;

    virtual bool CrossSegment(const Segment& AB) const = 0;

    virtual AbstractShape* Clone() const = 0;

    virtual AbstractShape* Move(const Vector& vec) const = 0;

    virtual void Print() const = 0;

    virtual ~AbstractShape() = default;
};

class Point : public AbstractShape {
public:
    Point();

    Point(long long x, long long y);

    Point(const Point& other);

    Point& operator=(const Point& other);

    ~Point() override;

    long long GetX() const;

    long long GetY() const;

    bool ContainsPoint(const Point& other) const override;

    bool CrossSegment(const Segment& other) const override;

    Point* Move(const Vector& vec) const override;

    Point* Clone() const override;

    void Print() const override;

private:
    long long X_;
    long long Y_;
};

bool operator<(const Point& A, const Point& B);
bool operator>(const Point& A, const Point& B);
bool operator<=(const Point& A, const Point& B);
bool operator>=(const Point& A, const Point& B);
bool operator==(const Point& A, const Point& B);
bool operator!=(const Point& A, const Point& B);

class Segment : public AbstractShape {
public:
    Segment();

    Segment(const Point& A, const Point& B);

    Segment(const Segment& other);

    Segment& operator=(const Segment& other);

    ~Segment() override;

    const Point GetA() const;

    const Point GetB() const;

    long double GetLength() const;

    bool ContainsPoint(const Point& A) const override;

    bool CrossSegment(const Segment& AB) const override;

    Segment* Clone() const override;

    Segment* Move(const Vector& vec) const override;

    void Print() const override;

private:
    Point A_;
    Point B_;
};

class Line : public AbstractShape {
public:
    Line();

    Line(const Point& A, const Point& B);

    Line(const Line& other);

    Line& operator=(const Line& other);

    ~Line() override;

    const Point GetA() const;

    const Point GetB() const;

    Vector GetDirection() const;

    bool ContainsPoint(const Point& A) const override;

    bool CrossSegment(const Segment& AB) const override;

    Line* Clone() const override;

    Line* Move(const Vector& vec) const override;

    void Print() const override;

private:
    Point A_;
    Point B_;
};

class Ray : public AbstractShape {
public:
    Ray();

    Ray(const Point& A, const Point& B);

    Ray(const Ray& other);

    Ray& operator=(const Ray& other);

    ~Ray() override;

    const Point GetA() const;

    const Point GetB() const;

    Vector GetDirection() const;

    bool ContainsPoint(const Point& A) const override;

    bool CrossSegment(const Segment& AB) const override;

    Ray* Clone() const override;

    Ray* Move(const Vector& vec) const override;

    void Print() const override;

private:
    Point A_;
    Point B_;
};

class Polygon : public AbstractShape {
public:
    Polygon();

    Polygon(const std::vector<Point>& points, size_t vertex_num);

    Polygon(const Polygon& other);

    Polygon& operator=(const Polygon& other);

    ~Polygon() override;

    size_t GetSize() const;

    long double GetArea() const;

    const std::vector<Point>& GetVertices() const;

    bool IsConvex() const;

    bool ContainsPoint(const Point& A) const override;

    bool CrossSegment(const Segment& AB) const override;

    Polygon* Clone() const override;

    Polygon* Move(const Vector& vec) const override;

    void Print() const override;

private:
    std::vector<Point> vertices_;
    size_t vertex_count_;
};

class Circle : public AbstractShape {
public:
    Circle();

    Circle(const Point& center, size_t radius);

    Circle(const Circle& other);

    Circle& operator=(const Circle& other);

    ~Circle() override;

    long double GetArea() const;

    long double GetLength() const;

    const Point& GetCentre() const;

    size_t GetRadius() const;

    bool ContainsPoint(const Point& A) const override;

    bool CrossSegment(const Segment& AB) const override;

    Circle* Clone() const override;

    Circle* Move(const Vector& vec) const override;

    void Print() const override;

private:
    Point centre_;
    size_t radius_;
};

/* additional functions' declarations */
long long VectorProduct(const Vector& lhs, const Vector& rhs);
long long DotProduct(const Vector& lhs, const Vector& rhs);
long long AngleSign(const Vector& lhs, const Vector& rhs);

long double PointPointDist(const Point& A, const Point& B);
long double PointLineDist(const Line& m, const Point& A);
long double PointRayDist(const Ray& ray, const Point& A);
long double PointSegmentDist(const Segment& seg, const Point& A);

} // namespace geometry

#endif //GEOMETRY_H
