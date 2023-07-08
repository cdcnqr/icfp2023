#ifndef INCLUDE_OPTIMIZER_STATE_HPP
#define INCLUDE_OPTIMIZER_STATE_HPP

#include <cstdint>
#include <vector>
#include <random>
#include <iostream>

#include <problem.hpp>

class OptimizerState {
  static constexpr double EPS = 1e-4;
  static constexpr double PI = 4 * std::atan(1);

  int move_tries = 5;
  double move_vs_swap_p = 0.5;
  // Move amount pulled from triangle distribution
  double move_amount_max = 10;

  const Problem& problem;
  std::vector<Point> placements;
public:
  struct grid_init_tag {};
  OptimizerState(grid_init_tag, const Problem& problem);
  uint64_t calc_score();

  std::vector<Point> get_placements();
  void set_placements(std::vector<Point> placements);
  const std::vector<Point>& get_placements_ref();

  template <class RNG>
  void perturb(RNG& rng);
  template <class RNG>
  void move_random(RNG& rng);
  template <class RNG>
  void swap_random(RNG& rng);
};

template <class RNG>
void OptimizerState::perturb(RNG& rng){
  if (problem.musicians.size() == 1) {
    move_random(rng);
    return;
  }
  std::uniform_real_distribution<double> p_sample(0, 1);
  double move_vs_flip_sample = p_sample(rng);
  if (move_vs_flip_sample < move_vs_swap_p) {
    move_random(rng);
  } else {
    swap_random(rng);
  }
}

template <class RNG>
void OptimizerState::move_random(RNG& rng){
  std::uniform_int_distribution<int> idx_sample(0, problem.musicians.size() - 1);
  std::uniform_real_distribution<double> move_angle_sample(0, 2 * PI);
  std::uniform_real_distribution<double> move_amount_sample(0, move_amount_max / 2);
  for (int i = 0; i < move_tries; ++i) {
    int move_idx = idx_sample(rng);
    double move_angle = move_angle_sample(rng);
    double move_amount = move_amount_sample(rng) + move_amount_sample(rng);
    Point new_pos = {
      placements[move_idx].x + move_amount * std::cos(move_angle),
      placements[move_idx].y + move_amount * std::sin(move_angle)
    };
    if (new_pos.x - 5 - EPS < problem.stage_bottom_left.x ||
        problem.stage_bottom_left.x + problem.stage.x < new_pos.x + 5 + EPS ||
        new_pos.y - 5 - EPS < problem.stage_bottom_left.y ||
        problem.stage_bottom_left.y + problem.stage.y < new_pos.y + 5 + EPS) {
      continue;
    }
    bool fit = true;
    for (int j = 0; j < problem.musicians.size(); ++j) {
      if (j == move_idx) continue;
      double dx = new_pos.x - placements[j].x;
      double dy = new_pos.y - placements[j].y;
      if (dx * dx + dy * dy < 5 * 5 + EPS) {
        fit = false;
        break;
      }
    }
    if (fit) {
      placements[move_idx] = new_pos;
      break;
    }
  }
}

template <class RNG>
void OptimizerState::swap_random(RNG& rng){
  std::uniform_int_distribution<int> idx_sample(0, problem.musicians.size() - 1);
  std::uniform_int_distribution<int> second_idx_sample(0, problem.musicians.size() - 2);
  int swap_idx1 = idx_sample(rng);
  int swap_idx2 = second_idx_sample(rng);
  if (swap_idx2 >= swap_idx1) ++swap_idx2;
  std::swap(placements[swap_idx1], placements[swap_idx2]);
}

#endif // INCLUDE_OPTIMIZER_STATE_HPP
