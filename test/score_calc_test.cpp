#include <iostream>

#include <optimizer.hpp>

int main() {
  Problem problem{
    .room = {2000, 5000},
    .stage = {1000, 200},
    .stage_bottom_left = {500, 0},
    .musicians = {0, 1, 0},
    .attendees = {{{100, 500}, {1000, -1000}}, {{200, 1000}, {200, 200}}, {{1100, 800}, {800, 1500}}}
  };
  Optimizer opt(problem);
}
