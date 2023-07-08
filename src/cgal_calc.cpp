#include "cgal_calc.hpp"

static Kernel::FT to_proper(SqrtExtT x) {
  return x.a0() + x.a1() * CGAL::sqrt(x.root());
}

uint64_t calc_score(Problem problem, std::vector<Point> placements) {
  uint64_t score = 0;
  int attendee_idx = -1;
  for (const auto& attendee : problem.attendees) {
    ++attendee_idx;
    std::vector<Curve_2> curves;
    const auto& raw_a = attendee.location;
    Point_2 a{raw_a.x, raw_a.y};
    for (int i = 0; i < problem.musicians.size(); ++i) {
      const auto& raw_b = placements[i];
      Point_2 b{raw_b.x, raw_b.y};
      curves.push_back({a, b});
      curves.emplace_back(b, 5);
    }
    std::vector<Res_Point_2> raw_pts;
    CGAL::compute_intersection_points(curves.begin(), curves.end(),
        std::back_inserter(raw_pts));

    std::vector<std::tuple<Direction_2, Kernel::FT, int>> vision_pts;

    Point_2 a_sqrt{raw_a.x, raw_a.y};

    for (int i = 0; i < problem.musicians.size(); ++i) {
      const auto& raw_b = placements[i];
      Point_2 b{raw_b.x, raw_b.y};
      Curve_2 vision_curves[] = {{a, b}, {b, 5}};
      std::array<Res_Point_2, 1> raw_pt;
      CGAL::compute_intersection_points(std::begin(vision_curves), std::end(vision_curves), raw_pt.begin());
      Point_2 pt{to_proper(raw_pt[0].x()), to_proper(raw_pt[0].y())};
      vision_pts.push_back({Direction_2{Segment_2{a_sqrt, pt}}, CGAL::squared_distance(a_sqrt, pt), i});
    }

    for (const auto& raw_pt : raw_pts) {
      Point_2 pt{to_proper(raw_pt.x()), to_proper(raw_pt.y())};
      if (pt == a_sqrt) continue;
      vision_pts.push_back({Direction_2{Segment_2{a_sqrt, pt}}, CGAL::squared_distance(a_sqrt, pt), problem.musicians.size()});
    }
    std::sort(vision_pts.begin(), vision_pts.end());

    bool visible = true;
    for (int i = 0; i < vision_pts.size(); ++i) {
      const auto& [dir, distsq, idx] = vision_pts[i];
      if (i > 0) {
        const auto& [pdir, pdistsq, pidx] = vision_pts[i - 1];
        if (dir > pdir) {
          visible = true;
        }
      }
      if (idx >= problem.musicians.size()) {
        visible = false;
      } else if (visible) {
        const auto& raw_b = placements[idx];
        Point_2 b{raw_b.x, raw_b.y};
        score += std::ceil(1000000 * attendee.tastes[problem.musicians[idx]] / CGAL::to_double(CGAL::squared_distance(a, b)));
      }
    }
  }
  return score;
}