#ifndef INCLUDE_OPTIMIZER_STATE_HPP
#define INCLUDE_OPTIMIZER_STATE_HPP

#include <cstdint>
#include <vector>

#include <CGAL/Cartesian.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>

#include <problem.hpp>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_2 = Kernel::Point_2;
using Segment_2 = Kernel::Segment_2;
using Direction_2 = Kernel::Direction_2;

using Traits_2 = CGAL::Arr_circle_segment_traits_2<Kernel>;
using Curve_2 = Traits_2::Curve_2;

using Res_Point_2 = Traits_2::Point_2;
using SqrtKernel = CGAL::Filtered_kernel<CGAL::Cartesian<CGAL::Sqrt_extension<Kernel::FT, Kernel::FT, CGAL::Tag_true, CGAL::Tag_true>>>;
using SqrtPoint_2 = SqrtKernel::Point_2;
using SqrtSegment_2 = SqrtKernel::Point_2;
using SqrtDirection_2 = SqrtKernel::Point_2;

class OptimizerState {
  static constexpr double EPS = 1e-4;

  const Problem& problem;
  std::vector<Point> placements;
public:
  struct grid_init_tag {};
  OptimizerState(grid_init_tag, const Problem& problem);
  double calc_cost();
};

OptimizerState::OptimizerState(grid_init_tag, const Problem& problem)
  : problem(problem)
{
  double inset = 10 * EPS;
  double grid_minx = problem.stage_bottom_left.x + inset;
  double grid_miny = problem.stage_bottom_left.y + inset;
  double grid_w = problem.stage.x - 2 * inset;
  double grid_h = problem.stage.y - 2 * inset;
  double grid_a = grid_w * grid_h;
  int grid_nx = std::ceil(problem.musicians.size() * grid_w / grid_a);
  int grid_ny = std::ceil(problem.musicians.size() * grid_h / grid_a);
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

double OptimizerState::calc_cost() {
  std::vector<Curve_2> curves;
  for (const auto& attendee : problem.attendees) {
    const auto& a = attendee.location;
    curves.clear();
    for (int i = 0; i < problem.musicians.size(); ++i) {
      const auto& b = placements[i];
      curves.emplace_back(Point_2{a.x, a.y}, Point_2{b.x, b.y});
      curves.emplace_back(Point_2{b.x, b.y}, 5);
    }
    std::vector<Res_Point_2> pts;
    CGAL::compute_intersection_points(curves.begin(), curves.end(),
        std::back_inserter(pts));
    std::sort(pts.begin(), pts.end(), [&](const auto& rpt1, const auto& rpt2){
        SqrtPoint_2 pt1{rpt1.x(), rpt1.y()};
        SqrtPoint_2 pt2{rpt2.x(), rpt2.y()};
        return SqrtDirection_2{SqrtSegment_2{a, pt1}} < SqrtDirection_2{SqrtSegment_2{a, pt2}};
        });
    std::vector<int> placement_idx(problem.musicians.size());
    std::iota(placement_idx.begin(), placement_idx.end(), 0);
    std::sort(placement_idx.begin(), placement_idx.end(), [&](const auto& idx1, const auto& idx2){
        return Direction_2{Segment_2{a, placement[idx1]}} < Direction_2{Segment_2{a, placement[idx2]}};
        });
  }
}

#endif // INCLUDE_OPTIMIZER_STATE_HPP
