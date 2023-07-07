#ifndef INCLUDE_OPTIMIZER_HPP
#define INCLUDE_OPTIMIZER_HPP

#include <optimizer_state.hpp>

class Optimizer {
  OptimizerState state;
public:
  Optimizer(const Problem& problem) :
    state(OptimizerState::grid_init_tag{}, problem)
  {
  }
};
#endif
