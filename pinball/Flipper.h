#pragma once
#include "Ball.hpp"
#include "Vec.hpp"
#include "Objects.hpp"
#include "BoxRigid.h"
#include "CollisionDetection.hpp"
#include <SFML/Graphics.hpp>


class Flipper : public BoxRigid {
public:

	int sign;
	Vec tip, pos;

	sf::ConvexShape sfml_repr_f;
	Vec p1, p2, p3, p4;

	Flipper() {}
	Flipper(float, float, float, float, int);

	void reset();
	void sfml_flipper_fix();

	void set_point_count(int);

	void set_rotation(float angle);
    void rotate(double);

	void set_angle(float angle);

	void flip(float dt);
	void unflip(float dt);

	sf::ConvexShape sfmlObj();

	// void resolve_collision(Ball& ball);
};



