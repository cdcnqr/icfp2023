#pragma once

#include "problem.hpp"

#include <queue>

class GreedyOptimizer {
  const Problem &problem;
  std::vector<Point> placements;
  
  inline GreedyOptimizer(const Problem &problem): problem(problem) {
    
  }
  
  void optimize() {
    double inset = 10;
    double grid_minx = problem.stage_bottom_left.x + inset;
    double grid_miny = problem.stage_bottom_left.y + inset;
    double grid_w = problem.stage.x - 2 * inset;
    double grid_h = problem.stage.y - 2 * inset;
    
    double grid_box = 10;
    int grid_nx = std::floor(grid_w / grid_box);
    int grid_ny = std::floor(grid_h / grid_box);

    // "circle" of points that need to be marked when a musician is placed at the center
    std::vector<std::pair<int, int>> within_10;
    int max_test = std::ceil(11 / grid_box);
    for (int i = 0; i <= max_test; i++) {
      for (int j = 0; j <= max_test; j++) {
        if (std::sqrt(std::pow(i * grid_box, 2) + std::pow(j * grid_box, 2)) < 10) {
          within_10.push_back({i, j});
          within_10.push_back({i, -j});
          within_10.push_back({-i, j});
          within_10.push_back({-i, -j});
        }
      }
    }

    // counts of each type
    std::vector<int> musician_types(problem.musicians.size(), 0);
    int num_types = 0;
    for (int i : problem.musicians) {
      musician_types[i]++;
      num_types = std::max(num_types, musician_types[i]);
    }
    
    std::vector<std::vector<std::vector<double>>> weight(num_types,
        std::vector<std::vector<double>>(grid_nx, std::vector<double>(grid_ny, 0)));
    
    // calculate weight of each square
    for (auto &attendee : problem.attendees) {
      for (int x=0; x<grid_nx; x++) {
        for (int y=0; y<grid_ny; y++) {
          double dx = (grid_minx + (x + 0.5) * grid_box) - attendee.location.x;
          double dy = (grid_miny + (y + 0.5) * grid_box) - attendee.location.y;
          double dist_sq = dx * dx + dy * dy;
          for (int t=0; t<num_types; t++) {
              weight[t][x][y] += attendee.tastes[t] / dist_sq;
          }
        }
      }
    }
    
    struct WeightSquare {
      double weight;
      int x;
      int y;
    };
    
    std::vector<std::vector<bool>> placed(grid_nx, std::vector<bool>(grid_ny, false));
    
    auto comp = [](WeightSquare a, WeightSquare b) {
      return a.weight < b.weight;
    };
    std::vector<std::priority_queue<WeightSquare, std::vector<WeightSquare>, decltype(comp)>> sorted;
    for (int x=0; x<grid_nx; x++) {
      for (int y=0; y<grid_ny; y++) {
        for (int t=0; t<num_types; t++) {
          sorted[t].push({weight[t][x][y], x, y});
        }
      }
    }
    
    for (int t : problem.musicians) {
      WeightSquare ws = sorted[t].top();
      sorted[t].pop();
      while (placed[ws.x][ws.y]) {
        ws = sorted[t].top();
        sorted[t].pop();
      }
      placements.push_back({grid_minx + (ws.x + 0.5) * grid_box, grid_miny + (ws.y + 0.5) * grid_box});
      for (auto &p : within_10) {
        placed[ws.x + p.first][ws.y + p.second] = true;
      }
    }
  }

};
