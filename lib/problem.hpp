#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include <nlohmann/json.hpp>
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

void to_json(json& j, const Attendee& p);
void from_json(const json& j, Attendee& p);

void to_json(json& j, const Problem& p);
void from_json(const json& j, Problem& p);

void parse_problem(const json& data, Problem& problem); 
void print_problem(const Problem& problem); 

#endif 
