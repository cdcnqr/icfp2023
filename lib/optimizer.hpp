#ifndef INCLUDE_OPTIMIZER_HPP
#define INCLUDE_OPTIMIZER_HPP

#include <optimizer_state.hpp>
#include <vector>

#include <problem.hpp>

class Optimizer {
  OptimizerState state;
public:
  Optimizer(const Problem& problem);

  std::vector<Point> get_placements();
  void set_placements(std::vector<Point> placements);

  uint64_t current_score();
};

#endif
