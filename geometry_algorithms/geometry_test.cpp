#include "geometry.h"

void CheckFunctions(const geometry::AbstractShape* shape, const geometry::Point& A, const geometry::Point& B) {
    if (shape->ContainsPoint(A)) {
        std::cout << "Given shape contains point A" << std::endl;
    } else {
        std::cout << "Given shape does not contain point A" << std::endl;
    }

    geometry::Segment AB(A, B);
    if (shape->CrossSegment(AB)) {
        std::cout << "Given shape crosses segment AB" << std::endl;
    } else {
        std::cout << "Given shape does not cross segment AB" << std::endl;
    }

    geometry::Vector ab(A, B);
    geometry::AbstractShape* clonedShape = shape->Clone();
    clonedShape->Move(ab)->Print();
    delete clonedShape;
}

int main() {
    geometry::AbstractShape* shape;
    char command[10];
    std::cin >> command;
    if (!strcmp(command, "point")) {
        int x, y;
        std::cin >> x >> y;
        shape = new geometry::Point(x, y);
    } else if (!strcmp(command, "segment")) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        shape = new geometry::Segment(geometry::Point(x1, y1), geometry::Point(x2, y2));
    } else if (!strcmp(command, "ray")) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        shape = new geometry::Ray(geometry::Point(x1, y1), geometry::Point(x2, y2));
    } else if (!strcmp(command, "line")) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        shape = new geometry::Line(geometry::Point(x1, y1), geometry::Point(x2, y2));
    } else if (!strcmp(command, "polygon")) {
        size_t n_points;
        std::cin >> n_points;
        std::vector<geometry::Point> points(0);
        for (size_t i = 0; i < n_points; ++i) {
            int x, y;
            std::cin >> x >> y;
            points.emplace_back(x, y);
        }
        shape = new geometry::Polygon(points, n_points);
    } else if (!strcmp(command, "circle")) {
        int x, y, r;
        std::cin >> x >> y >> r;
        shape = new geometry::Circle(geometry::Point(x, y), static_cast<double>(r));
    } else {
        std::cerr << "Undefined command" << std::endl;
        return 1;
    }

    int ax, ay, bx, by;
    std::cin >> ax >> ay >> bx >> by;
    geometry::Point A(ax, ay), B(bx, by);

    CheckFunctions(shape, A, B);
    delete shape;
    return 0;
}