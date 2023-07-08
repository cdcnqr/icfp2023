#include <fstream>
#include <iostream>
#include <string>
#include <problem.hpp>
#include <visualization.hpp>

#include <optimizer.hpp>

int main(int argc, char** argv) {
  std::ifstream f (argv[1]); 
  auto data_str = json::parse(f)["Success"].template get<std::string>(); 
  Problem problem = json::parse(data_str); 

  print_problem(problem);

  Optimizer opt(problem);
  opt.optimize();

//  std::cout << opt.current_score() << std::endl;

  draw_problem(problem, opt.get_placements());
}
