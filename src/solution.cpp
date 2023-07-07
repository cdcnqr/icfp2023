#include <iostream>
#include "../lib/solution.hpp"

void unprase_solution(const Solution& solution, json& output) {
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

int main() {
    Solution sol; 
    sol.problem_id = 1; 
    sol.placements.resize(50); 
    for(int i = 0; i < 50; ++i) {
       sol.placements[i] = Point{ 5.0*i, 5.0*i };  
    }
    json output; 
    unprase_solution(sol, output); 
    std::cout << output.dump() << "\n"; 
}