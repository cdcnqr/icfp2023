#include <fstream>
#include <iostream>
#include <string>
#include <problem.hpp>
#include <solution.hpp>
#include <visualization.hpp>

#include <greedy_optimizer.hpp>

int main(int argc, char** argv) {
    std::ifstream f (argv[1]); 
    auto data_str = json::parse(f)["Success"].template get<std::string>(); 
    auto data = json::parse(data_str); 
    Problem problem; 
    parse_problem(data, problem); 
    print_problem(problem); 
    
    GreedyOptimizer opt(problem);
    opt.optimize();
    
    draw_problem(problem, opt.placements);

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
