#include "problem.hpp"
#include <SFML/Graphics.hpp>

inline void draw_problem(Problem &problem) {
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
