#pragma once
#include <SFML/Graphics.hpp>
#include <Vec.hpp>

int main2();

// Modified sf
namespace msf {
    class RectangleShape : public sf::RectangleShape {
    public:
        // Not using Vector libraries.
        RectangleShape() { }
        RectangleShape(const Vec& cv) { sf::RectangleShape::RectangleShape(sf::Vector2f(cv.x, cv.y)); }
        void setPosition(const Vec& cv) { sf::RectangleShape::RectangleShape::setPosition(sf::Vector2f(cv.x, cv.y)); }
        void setSize(const Vec& cv) { sf::RectangleShape::RectangleShape::setSize(sf::Vector2f(cv.x, cv.y)); }
    };
    
    // Does not use sf::Vector2f so nothing to do here.
    class CircleShape : public sf::CircleShape { };

    class ConvexShape : public sf::ConvexShape {
        void setPoint(std::size_t index, const Vec& point) {
            sf::ConvexShape::ConvexShape::setPoint(index, sf::Vector2f(point.x, point.y));
        };
    };
}
