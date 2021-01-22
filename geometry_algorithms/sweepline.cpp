#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

struct Point {
  int64_t x_;
  int64_t y_;
};

bool operator<(const Point& lhs, const Point& rhs) {
  return lhs.x_ < rhs.x_;
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
  out << point.x_ << ' ' << point.y_ << '\n';
  return out;
}

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x_ >> point.y_;
  return in;
}

struct Segment {
  Point A_;
  Point B_;
  int id_;

  long double GetY(int64_t x) const {
    if (A_.x_ == B_.x_) {
      return A_.y_;
    }
    return static_cast<long double>(A_.y_) +
           static_cast<long double>((B_.y_ - A_.y_) * (x - A_.x_)) /
               static_cast<long double>(B_.x_ - A_.x_);
  }
};

std::ostream& operator<<(std::ostream& out, const Segment& seg) {
  out << seg.A_ << ' ' << seg.B_ << '\n';
  return out;
}

std::istream& operator>>(std::istream& in, Segment& seg) {
  in >> seg.A_ >> seg.B_;
  return in;
}

int64_t VectorProduct(const Segment& AB, const Segment& CD) {
  int64_t ABx = AB.B_.x_ - AB.A_.x_;
  int64_t ABy = AB.B_.y_ - AB.A_.y_;
  int64_t CDx = CD.B_.x_ - CD.A_.x_;
  int64_t CDy = CD.B_.y_ - CD.A_.y_;
  return ABx * CDy - ABy * CDx;
}

int64_t Direction(const Point& A, const Point& B, const Point& C) {
  return VectorProduct({A, B}, {A, C});
}

bool OnSegmentProjection(const Segment& AB, const Point& C) {
  return std::min(AB.A_.x_, AB.B_.x_) <= C.x_ &&
         std::max(AB.A_.x_, AB.B_.x_) >= C.x_ &&
         std::min(AB.A_.y_, AB.B_.y_) <= C.y_ &&
         std::max(AB.A_.y_, AB.B_.y_) >= C.y_;
}

bool CrossSegment(const Segment& AB, const Segment& CD) {
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

bool operator<(const Segment& lhs, const Segment& rhs) {
  auto x =
      std::max(std::min(lhs.A_.x_, lhs.B_.x_), std::min(rhs.A_.x_, rhs.B_.x_));
  return lhs.GetY(x) < rhs.GetY(x);
}

struct Event {
  int64_t x_;
  int id_;
  bool is_open_;
  Event() = default;
  Event(int64_t x, int id, bool is_open) : x_(x), id_(id), is_open_(is_open) {}
};

std::ostream& operator<<(std::ostream& out, const Event& e) {
  out << '(' << e.x_ << ' ' << e.id_ << ' ' << std::boolalpha << e.is_open_
      << ")\n";
  return out;
}

bool operator<(const Event& lhs, const Event& rhs) {
  if (lhs.x_ != rhs.x_) {
    return lhs.x_ < rhs.x_;
  }
  return lhs.is_open_ && !rhs.is_open_;
}

std::set<Segment>::iterator Prev(std::set<Segment>::iterator it,
                                 const std::set<Segment>& set) {
  return it == set.begin() ? set.end() : --it;
}

template <typename ContainerType>
void PrintContainer(const ContainerType& container) {
  std::copy(
      container.begin(), container.end(),
      std::ostream_iterator<typename ContainerType::value_type>(std::cout));
}

std::pair<int, int> FindIntersectingSegments(
    const std::vector<Segment>& segments) {
  std::set<Segment> seg_set{};
  std::vector<Event> events{};
  std::vector<std::set<Segment>::iterator> where{};
  for (size_t i = 0; i < segments.size(); ++i) {
    events.emplace_back(std::min(segments[i].A_.x_, segments[i].B_.x_), i,
                        true);
    events.emplace_back(std::max(segments[i].A_.x_, segments[i].B_.x_), i,
                        false);
  }
  std::sort(events.begin(), events.end());
  where.resize(segments.size());
  for (const auto& event : events) {
    int curr_id = event.id_;
    if (event.is_open_) {
      auto upper_neighbour = seg_set.lower_bound(segments[curr_id]);
      auto lower_neighbour = Prev(upper_neighbour, seg_set);
      if (upper_neighbour != seg_set.end() &&
          CrossSegment(*upper_neighbour, segments[curr_id])) {
        return std::make_pair(upper_neighbour->id_, curr_id);
      }
      if (lower_neighbour != seg_set.end() &&
          CrossSegment(*lower_neighbour, segments[curr_id])) {
        return std::make_pair(lower_neighbour->id_, curr_id);
      }
      where[curr_id] = seg_set.insert(upper_neighbour, segments[curr_id]);
    } else {
      auto upper_neighbour = std::next(where[curr_id]);
      auto lower_neighbour = Prev(where[curr_id], seg_set);
      if (upper_neighbour != seg_set.end() &&
          lower_neighbour != seg_set.end() &&
          CrossSegment(*upper_neighbour, *lower_neighbour)) {
        return std::make_pair(lower_neighbour->id_, upper_neighbour->id_);
      }
      seg_set.erase(where[curr_id]);
    }
  }
  return std::make_pair(-1, -1);
}

int main() {
  size_t N = 0;
  std::cin >> N;
  std::vector<Segment> segments(N);
  for (size_t i = 0; i < N; ++i) {
    std::cin >> segments[i];
    if (segments[i].B_ < segments[i].A_) {
      std::swap(segments[i].A_, segments[i].B_);
    }
    segments[i].id_ = i;
  }
  auto result = FindIntersectingSegments(segments);
  if (result.first == -1 && result.second == -1) {
    std::cout << "NO\n";
  } else {
    std::cout << "YES\n"
              << result.first + 1 << ' ' << result.second + 1 << '\n';
  }
  return 0;
}