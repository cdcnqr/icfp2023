#ifndef INCLUDE_OPTIMIZER_STATE_HPP
#define INCLUDE_OPTIMIZER_STATE_HPP

#include <cstdint>
#include <vector>

#include <problem.hpp>

class OptimizerState {
  static constexpr double EPS = 1e-4;

  const Problem& problem;
  std::vector<Point> placements;
public:
  struct grid_init_tag {};
  OptimizerState(grid_init_tag, const Problem& problem);
  uint64_t calc_score();
  void set_placements(std::vector<Point> placements);
};

#endif // INCLUDE_OPTIMIZER_STATE_HPP
