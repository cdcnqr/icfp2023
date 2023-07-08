#ifndef INCLUDE_OPTIMIZER_HPP
#define INCLUDE_OPTIMIZER_HPP

#include <optimizer_state.hpp>
#include <random>
#include <vector>

#include <problem.hpp>

class Optimizer {
  OptimizerState state;
  std::mt19937_64 rng;
public:
  Optimizer(const Problem& problem);

  std::vector<Point> get_placements();
  void set_placements(std::vector<Point> placements);
  const std::vector<Point>& get_placements();

  uint64_t current_score();
  void optimize();
};

#endif
