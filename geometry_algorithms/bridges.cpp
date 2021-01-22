#include <cstdint>
#include <iostream>
#include <vector>

struct Point {
  int64_t x_;
  int64_t y_;
};

struct Segment {
  Point A_;
  Point B_;
};

int64_t VectorProduct(const Segment &AB, const Segment &CD) {
  int64_t ABx = AB.B_.x_ - AB.A_.x_;
  int64_t ABy = AB.B_.y_ - AB.A_.y_;
  int64_t CDx = CD.B_.x_ - CD.A_.x_;
  int64_t CDy = CD.B_.y_ - CD.A_.y_;
  return ABx * CDy - ABy * CDx;
}

int64_t Direction(const Point &A, const Point &B, const Point &C) {
  return VectorProduct({A, B}, {A, C});
}

bool OnSegmentProjection(const Segment &AB, const Point &C) {
  return std::min(AB.A_.x_, AB.B_.x_) <= C.x_ &&
         std::max(AB.A_.x_, AB.B_.x_) >= C.x_ &&
         std::min(AB.A_.y_, AB.B_.y_) <= C.y_ &&
         std::max(AB.A_.y_, AB.B_.y_) >= C.y_;
}

bool CrossSegment(const Segment &AB, const Segment &CD) {
  auto dir_cda = Direction(CD.A_, CD.B_, AB.A_);
  auto dir_cdb = Direction(CD.A_, CD.B_, AB.B_);
  auto dir_abc = Direction(AB.A_, AB.B_, CD.A_);
  auto dir_abd = Direction(AB.A_, AB.B_, CD.B_);
  return (((dir_cda > 0 && dir_cdb < 0) || (dir_cda < 0 && dir_cdb > 0)) &&
          ((dir_abc > 0 && dir_abd < 0) || (dir_abc < 0 && dir_abd > 0))) ||
         (dir_cda == 0 && OnSegmentProjection(CD, AB.A_)) ||
         (dir_cdb == 0 && OnSegmentProjection(CD, AB.B_)) ||
         (dir_abc == 0 && OnSegmentProjection(AB, CD.A_)) ||
         (dir_abd == 0 && OnSegmentProjection(AB, CD.B_));
}

size_t CountCrosses(const Segment &road, const std::vector<Segment> &rivers) {
  size_t answer = 0;
  for (const auto &river : rivers) {
    if (CrossSegment(road, river)) {
      ++answer;
    }
  }
  return answer;
}

int main() {
  Segment road{};
  std::cin >> road.A_.x_ >> road.A_.y_ >> road.B_.x_ >> road.B_.y_;

  size_t N = 0;
  std::cin >> N;
  std::vector<Segment> rivers(N);
  for (size_t i = 0; i < N; ++i) {
    std::cin >> rivers[i].A_.x_ >> rivers[i].A_.y_ >> rivers[i].B_.x_ >>
        rivers[i].B_.y_;
  }
  std::cout << CountCrosses(road, rivers) << std::endl;
  return 0;
}