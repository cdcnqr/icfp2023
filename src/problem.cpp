#include <fstream>
#include <iostream>
#include <string>
#include "../lib/problem.hpp"

#include <SFML/Graphics.hpp>

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

static void print_point(const Point& point, bool newline) {
    std::cout << "(" << point.x << ", " << point.y << ")"; 
    if(newline) std::cout << "\n"; 
}

void print_problem(const Problem& problem) {
    std::cout << "room="; 
    print_point(problem.room, true); 
    std::cout << "stage="; 
    print_point(problem.stage, true); 
    std::cout << "stage_bottom_left="; 
    print_point(problem.stage_bottom_left, true); 
    std::cout << "\nmusicians=[\n"; 
    for(int i = 0; i < problem.musicians.size()-1; ++i) {
        std::cout << problem.musicians[i] << ", "; 
    }  
    std::cout << problem.musicians[problem.musicians.size()-1] << "\n]\n"; 
    std::cout << "\nattendees=[\n"; 
    for(int i = 0; i < problem.attendees.size()-1; ++i) {
        print_point(problem.attendees[i].location, false); 
        std::cout << ", ";  
    }
    print_point(problem.attendees[problem.attendees.size()-1].location, false); 
    std::cout << "\n]\n"; 
}

void draw_problem(Problem &problem) {
    constexpr double width = 1000, height = 1000;
    sf::RenderWindow window(sf::VideoMode(width, height), "Problem");
    
    double scale = fmin(width / problem.room.x, height / problem.room.y);

    sf::RectangleShape room(sf::Vector2f(problem.room.x * scale, problem.room.y * scale));
    room.setFillColor(sf::Color::White);
    window.draw(room);

    sf::RectangleShape stage(sf::Vector2f(problem.stage.x * scale, problem.stage.y * scale));
    stage.setPosition(problem.stage_bottom_left.x * scale, problem.stage_bottom_left.y * scale);
    stage.setFillColor(sf::Color(216, 64, 64));
    window.draw(stage);

    for (auto &attendee : problem.attendees) {
        sf::CircleShape shape(2.f);
        shape.setFillColor(sf::Color(128, 128, 128));
        shape.setOrigin(shape.getRadius(), shape.getRadius()); // center of circle
        shape.setPosition(attendee.location.x * scale, attendee.location.y * scale);
        window.draw(shape);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /*
        window.clear();
        window.draw(shape);
        */
        window.display();
    }
}

int main(int argc, char** argv) {
    std::ifstream f (argv[1]); 
    auto data_str = json::parse(f)["Success"].template get<std::string>(); 
    auto data = json::parse(data_str); 
    Problem problem; 
    parse_problem(data, problem); 
    print_problem(problem); 
    
    draw_problem(problem);
}