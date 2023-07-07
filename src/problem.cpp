#include <fstream>
#include <iostream>
#include <string>
#include "../lib/problem.hpp"
#include "solution.hpp"
#include "visualization.hpp"

#include "greedy_optimizer.hpp"

void parse_problem(const json& data, Problem& problem) {
    data.at("room_width").get_to(problem.room.x); 
    data.at("room_height").get_to(problem.room.y); 
    data.at("stage_width").get_to(problem.stage.x); 
    data.at("stage_height").get_to(problem.stage.y); 
    data.at("stage_bottom_left")[0].get_to(problem.stage_bottom_left.x); 
    data.at("stage_bottom_left")[1].get_to(problem.stage_bottom_left.y); 
    problem.musicians.clear(); 
    for(auto musician : data.at("musicians")) {
        problem.musicians.push_back(musician); 
    }
    problem.attendees.clear();  
    for(auto attendee : data.at("attendees")) {
        Attendee tmp; 
        attendee.at("x").get_to(tmp.location.x); 
        attendee.at("y").get_to(tmp.location.y); 
        for(auto taste : attendee.at("tastes")) {
            tmp.tastes.push_back(taste); 
        }
        problem.attendees.push_back(tmp); 
    }
}

static void print_point(const Point& point) {
    std::cout << "(" << point.x << ", " << point.y << ")\n"; 
}

void print_problem(const Problem& problem) {
    std::cout << "room="; 
    print_point(problem.room); 
    std::cout << "stage="; 
    print_point(problem.stage); 
    std::cout << "stage_bottom_left="; 
    print_point(problem.stage_bottom_left); 
    std::cout << "musicians no.=" << problem.musicians.size() << "\n"; 
    std::cout << "attendees no.=" << problem.attendees.size() << "\n";   
}

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