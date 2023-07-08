#include <problem.hpp>

#include <iostream>

void to_json(json& j, const Attendee& p) {
  j = json{
    {"x", p.location.x},
    {"y", p.location.y},
    {"tastes", p.tastes}
  };
}

void from_json(const json& j, Attendee& p) {
  j.at("x").get_to(p.location.x);
  j.at("y").get_to(p.location.y);
  j.at("tastes").get_to(p.tastes);
}

void to_json(json& j, const Problem& p) {
  j = json{
    {"room_width", p.room.x},
    {"room_height", p.room.y},
    {"stage_width", p.stage.x},
    {"stage_height", p.stage.y},
    {"stage_bottom_left", std::vector<double>{p.stage_bottom_left.x, p.stage_bottom_left.y}},
    {"musicians", p.musicians},
    {"attendees", p.attendees}
  };
}

void from_json(const json& j, Problem& p) {
  j.at("room_width").get_to(p.room.x);
  j.at("room_height").get_to(p.room.y);
  j.at("stage_width").get_to(p.stage.x);
  j.at("stage_height").get_to(p.stage.y);
  j.at("stage_bottom_left")[0].get_to(p.stage_bottom_left.x);
  j.at("stage_bottom_left")[1].get_to(p.stage_bottom_left.y);
  j.at("musicians").get_to(p.musicians);
  j.at("attendees").get_to(p.attendees);
}

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
