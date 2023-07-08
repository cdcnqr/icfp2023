#include <iostream>
#include "../lib/solution.hpp"

void unparse_solution(const Solution& solution, json& output) {
    json contents; 
    output["problem_id"] = solution.problem_id; 
    contents["placements"] = json::array(); 
    for(const auto& p : solution.placements) {
        contents["placements"].push_back(json::object({
            {"x", p.x},
            {"y", p.y}
        })); 
    } 
    output["contents"] = contents.dump(); 
}
