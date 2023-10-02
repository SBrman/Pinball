#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.hpp"
#include "Objects.hpp"

struct GameShapes {
    sf::RectangleShape leftFlipper;
    sf::RectangleShape rightFlipper;
    sf::CircleShape circle_obstacle[10];
    sf::RectangleShape box_obstacle[10];
    Ball pinball;
};

GameShapes createGameShapes(int W, int H) {
    GameShapes shapes;

    // Define the left flipper
    shapes.leftFlipper.setPosition(150, 800);
    shapes.leftFlipper.setSize(sf::Vector2f(200, 20));
    shapes.leftFlipper.setOrigin(sf::Vector2f(0, 10));

    // Define the right flipper
    shapes.rightFlipper.setPosition(650, 800);
    shapes.rightFlipper.setSize(sf::Vector2f(200, 20));
    shapes.rightFlipper.setOrigin(sf::Vector2f(200, 10));

    Ball pinball{500, 500, 40};
    pinball.sfml_repr.setFillColor(sf::Color(200, 200, 100, 200));
	sf::Texture balltex;
    balltex.loadFromFile("textures/ball.jpg");
	pinball.sfml_repr.setTexture(&balltex, true);
    
    return shapes;
}


