#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include "nlohmann/json.hpp"
using json = nlohmann::json; 

struct Point {
    double x, y; 
}; 

struct Attendee {
    Point location; 
    std::vector<double> tastes; 
}; 

struct Problem {
    Point room, stage; 
    Point stage_bottom_left; 
    std::vector<int> musicians; 
    std::vector<Attendee> attendees; 
}; 

void parse_problem(const json& data, Problem& problem); 

void print_point(const Point& point); 
void print_problem(const Problem& problem); 

#endif 