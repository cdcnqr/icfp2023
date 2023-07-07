#ifndef SOLUTION_H
#define SOLUTION_H

#include "problem.hpp"

struct Solution {
    uint32_t problem_id; 
    std::vector<Point> placements; 
}; 

void unprase_solution(const Solution& sol, json& output); 

#endif
