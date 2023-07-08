#include <fstream>
#include <iostream>
#include <string>
#include <problem.hpp>
#include <solution.hpp>
#include <visualization.hpp>

#include "cgal_calc.hpp"
#include <optimizer.hpp>
#include <greedy_optimizer.hpp>


void solve(std::string in_file_name, std::string out_file_name, int problem_id) {
    std::ifstream f(in_file_name); 
    auto data_str = json::parse(f)["Success"].template get<std::string>(); 
    auto data = json::parse(data_str); 
    Problem problem; 
    parse_problem(data, problem); 
    //print_problem(problem); 

    GreedyOptimizer greedy_opt(problem);
    greedy_opt.optimize();
    if (greedy_opt.placements.empty() || !greedy_opt.verify_placements()) {
        std::cout << "Greedy placing failed in problem " << problem_id << std::endl;
        return;
    }

    Solution sol; 
    sol.problem_id = problem_id; 
    sol.placements = greedy_opt.placements;
    json output; 
    unparse_solution(sol, output); 
    
    std::ofstream out(out_file_name);
    out << output.dump() << "\n"; 
    std::cout << "Problem " << problem_id << " solved." << std::endl;
}

int main(int argc, char** argv) {
    if (argc == 2 && strcmp(argv[1], "-a") == 0) {
        std::vector<std::thread> threads;
        for (int i=1; i<=45; i++) {
            threads.emplace_back(solve, "input/in" + std::to_string(i) + ".json",
                    "out/out" + std::to_string(i) + ".json",
                    i);
        }
        for (int i=1; i<=45; i++) {
            threads[i-1].join();
        }
        return 0;
    }
    std::ifstream f (argv[1]); 
    auto data_str = json::parse(f)["Success"].template get<std::string>(); 
    auto data = json::parse(data_str); 
    Problem problem; 
    parse_problem(data, problem); 
    print_problem(problem); 

    GreedyOptimizer greedy_opt(problem);
    greedy_opt.optimize();
    draw_problem(problem, greedy_opt.placements);
    greedy_opt.verify_placements();

    /*
    Optimizer opt(problem);
    draw_problem(problem, opt.get_placements());
    std::cerr << opt.current_score() << std::endl;
    */

    Solution sol; 
    sol.problem_id = 1; 
    /*
    sol.placements.resize(50); 
    for(int i = 0; i < 50; ++i) {
       sol.placements[i] = Point{ 5.0*i, 5.0*i };
    }
    */
    sol.placements = greedy_opt.placements;
    json output; 
    unparse_solution(sol, output); 
    std::cout << output.dump() << "\n"; 
}
