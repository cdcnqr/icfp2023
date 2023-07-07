#ifndef INCLUDE_OPTIMIZER_STATE_HPP
#define INCLUDE_OPTIMIZER_STATE_HPP

#include <cstdint>
#include <vector>


class OptimizerState {
  constexpr double EPS = 1e-4;

  const std::vector<Attendee>& attendees;
  const std::vector<int64_t>& musicians;
  std::vector<Point> placements;
public:
  OptimizerState(double stage_minx, double stage_miny,
      double stage_width, double stage_height, int n_musicians);
  double calc_cost();
};

OptimizerState::OptimizerState(double stage_minx, double stage_miny,
    double stage_width, double stage_height, int n_musicians)
{
  double inset = 10 * EPS;
  double grid_minx = stage_minx + inset;
  double grid_miny = stage_miny + inset;
  double grid_w = stage_width - 2 * inset;
  double grid_h = stage_width - 2 * inset;
  double grid_a = grid_w * grid_h;
  int grid_nx = std::ceil(n_musicians * grid_w / grid_a);
  int grid_ny = std::ceil(n_musicians * grid_h / grid_a);
  double grid_dx = grid_nx == 1 ? 0 : grid_w / (grid_nx - 1);
  double grid_dy = grid_ny == 1 ? 0 : grid_h / (grid_ny - 1);

  int grid_i = 0;
  int grid_j = 0;
  for (int musician_id = 0; musician_id < n_musicians; ++musician_id) {
    placements.emplace_back(grid_minx + grid_i * grid_dx, grid_miny + grid_j * grid_dy);
    if (++grid_j >= grid_ny) {
      grid_j = 0;
      ++grid_i;
    }
  }
}



#endif // INCLUDE_OPTIMIZER_STATE_HPP
