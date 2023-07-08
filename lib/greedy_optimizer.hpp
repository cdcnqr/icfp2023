#pragma once

#include "problem.hpp"

#include <iostream>
#include <queue>

class GreedyOptimizer {
  static constexpr int min_musician_separation = 10;
  const Problem &problem;
public:
  std::vector<Point> placements;
  inline GreedyOptimizer(const Problem &problem): problem(problem) {
    
  }
  void optimize();
  bool verify_placements();
  
};
