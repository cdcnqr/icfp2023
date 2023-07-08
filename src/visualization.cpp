#include "visualization.hpp"

void draw_problem(Problem &problem, const std::vector<Point> &placements) {
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
    
    for (auto &p : placements) {
        sf::CircleShape musician(10.f * scale);
        musician.setFillColor(sf::Color(32, 240, 32));
        musician.setOrigin(musician.getRadius(), musician.getRadius());
        musician.setPosition(p.x * scale, p.y * scale);
        window.draw(musician);

        sf::CircleShape blocking(5.f * scale);
        blocking.setFillColor(sf::Color(32, 32, 240));
        blocking.setOrigin(blocking.getRadius(), blocking.getRadius()); // center of circle
        blocking.setPosition(p.x * scale, p.y * scale);
        window.draw(blocking);
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
