#include "optimizer.hpp"

Optimizer::Optimizer(const Problem& problem)
  : state(OptimizerState::grid_init_tag{}, problem) {}

void Optimizer::set_placements(std::vector<Point> placements)
{
  state.set_placements(placements);
}

std::vector<Point> Optimizer::get_placements() {
  return state.get_placements();
}

uint64_t Optimizer::current_score() {
  return state.calc_score();
}
