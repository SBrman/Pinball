#pragma once
#include "Flipper.h"
#include "Vec.hpp"
#include "Objects.hpp"
#include "CollisionDetection.hpp"
#include "BoxRigid.h"
#include <SFML/Graphics.hpp>


struct Ball : public Circle
{
	double vx, vy, m, ax, ay; 
	sf::CircleShape sfml_repr;

    int time_since_collision = 100;

	Ball() {}
	Ball(double, double, double);

	double mass();
	Vec direction();
	Vec position();
	Vec velocity();
	Vec acceleration();
	
	void setRadius(double);

	void setPosition(Vec);
	void setPosition(double, double);

	void setVelocity(Vec);
	void setVelocity(double, double);

	void setAcceleration(Vec);
	void setAcceleration(double, double);
	
	// Add to position
	void move(Vec);
	void move(double, double);

	sf::CircleShape sfmlObj();

	// Collision detection
	bool collides(Line&);
	bool collides(Circle&);
	bool collides(BoxRigid&);

	// Collision resolution
	Vec resolve_collision(Line&);
	Vec resolve_collision(Ball&);
	Vec resolve_collision(BoxRigid&, bool);

	Vec resolve_collision_obstacle(Ball&);
};

