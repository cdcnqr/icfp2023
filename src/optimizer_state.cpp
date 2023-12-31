#include <optimizer_state.hpp>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
using Point_2 = Kernel::Point_2;
using Segment_2 = Kernel::Segment_2;
using Direction_2 = Kernel::Direction_2;

using Traits_2 = CGAL::Arr_circle_segment_traits_2<Kernel>;
using Curve_2 = Traits_2::Curve_2;

using Res_Point_2 = Traits_2::Point_2;
using SqrtExtT = CGAL::Sqrt_extension<Kernel::FT, Kernel::FT, CGAL::Tag_true, CGAL::Tag_true>;

Kernel::FT to_proper(SqrtExtT x) {
  return x.a0() + x.a1() * CGAL::sqrt(x.root());
}

OptimizerState::OptimizerState(grid_init_tag, const Problem& problem)
  : problem(problem)
{
  double inset = 10 + EPS;
  double grid_minx = problem.stage_bottom_left.x + inset;
  double grid_miny = problem.stage_bottom_left.y + inset;
  double grid_w = problem.stage.x - 2 * inset;
  double grid_h = problem.stage.y - 2 * inset;
  int grid_nx = std::ceil(std::sqrt(problem.musicians.size() * grid_w / grid_h));
  int grid_ny = std::ceil(std::sqrt(problem.musicians.size() * grid_h / grid_w));
  double grid_dx = grid_nx == 1 ? 0 : grid_w / (grid_nx - 1);
  double grid_dy = grid_ny == 1 ? 0 : grid_h / (grid_ny - 1);

  int grid_i = 0;
  int grid_j = 0;
  for (int musician_id = 0; musician_id < problem.musicians.size(); ++musician_id) {
    placements.push_back({grid_minx + grid_i * grid_dx, grid_miny + grid_j * grid_dy});
    if (++grid_j >= grid_ny) {
      grid_j = 0;
      ++grid_i;
    }
  }
}

uint64_t OptimizerState::calc_score() {
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

    std::cout << "Computing intersections between " << curves.size() << " curves" << std::endl;

    CGAL::compute_intersection_points(curves.begin(), curves.end(),
        std::back_inserter(raw_pts));

    std::cout << "Got " << raw_pts.size() << " intersections" << std::endl;

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

    std::cout << "Sorting points" << std::endl;
    std::sort(vision_pts.begin(), vision_pts.end());
    std::cout << "Done sorting points" << std::endl;

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

std::vector<Point> OptimizerState::get_placements() {
  return placements;
}

void OptimizerState::set_placements(std::vector<Point> placements) {
  this->placements = placements;
}

const std::vector<Point>& OptimizerState::get_placements_ref() {
  return placements;
}
