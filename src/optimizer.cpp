#include <optimizer.hpp>

Optimizer::Optimizer(const Problem& problem)
  : state(OptimizerState::grid_init_tag{}, problem),
  rng(std::random_device()()) {}

void Optimizer::set_placements(std::vector<Point> placements)
{
  state.set_placements(placements);
}

const std::vector<Point>& Optimizer::get_placements() {
  return state.get_placements();
}

uint64_t Optimizer::current_score() {
  return state.calc_score();
}

void Optimizer::optimize() {
  for (int i = 0; i < 10000; ++i) {
    state.perturb(rng);
  }
}
