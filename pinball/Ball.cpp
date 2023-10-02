#pragma once
#include "Ball.hpp"
#include "BoxRigid.h"
#include "Flipper.h"
#include <SFML/Graphics.hpp>
#include "CollisionDetection.hpp"
#include <cmath>
#include <iostream>

const double PI = 3.14159f;
double g = 9.81f;
const double COR = 0.95f;
const double MAX_VELOCITY = 400.0;


Ball::Ball(double x, double y, double r) : Ball::Circle(0, x, y, r) {
	m = PI * 10 * r * r;
	vx = 0;
	vy = 0;
	ax = 0;
	ay = g;

	sfml_repr.setPosition(x - r, y - r);
	//sfml_repr.setOrigin(r, r);
	sfml_repr.setRadius(r);
}

double Ball::mass() {
	return m;
}


Vec Ball::direction() {
	return Vec();
}

Vec Ball::position() {
	return Vec(x, y);
}

Vec Ball::velocity() {
	return Vec(vx, vy);
}

Vec Ball::acceleration() {
	return Vec(ax, ay);
}

void Ball::setPosition(Vec position) {
	x = position.x;
	y = position.y;
	sfml_repr.setPosition(x-radius, y-radius);
}

void Ball::setPosition(double px, double py) {
	x = px;
	y = py;
	sfml_repr.setPosition(x-radius, y-radius);
}

void Ball::setVelocity(Vec velocity) {
	vx = std::min(velocity.x, MAX_VELOCITY);
	vy = std::min(velocity.y, MAX_VELOCITY);
}

void Ball::setVelocity(double vx, double vy) {
	this->vx = std::min(vx, MAX_VELOCITY);
	this->vy = std::min(vy, MAX_VELOCITY);
}

void Ball::setAcceleration(Vec acceleration) {
	ax = acceleration.x;
	ay = acceleration.y;
}

void Ball::setAcceleration(double ax, double ay) {
	this->ax = ax;
	this->ay = ay;
}

void Ball::setRadius(double r) {
	radius = r;
	sfml_repr.setRadius(r);
}

// Add vec to position
void Ball::move(Vec pos) {
	x += pos.x;
	y += pos.y;
	sfml_repr.setPosition(x - radius, y - radius);
}

// Add vec to position
void Ball::move(double x, double y) {
	this->x += x;
	this->y += y;
	sfml_repr.setPosition(this->x - radius, this->y - radius);
}

sf::CircleShape Ball::sfmlObj() {
	return sfml_repr;
}

bool Ball::collides(Line& line) {
	return line_circle_intersect(line, *this);
}

bool Ball::collides(Circle& circle) {
	return circle_circle_intersect(*this, circle);
}

bool Ball::collides(BoxRigid& box) {
	return circle_box_intersect(*this, box);
}

Vec Ball::resolve_collision(Line& other) {
	Vec closest = closest_point(other, *this);
	
	Vec direction = position() - closest;
	if (direction.length() < radius) {
		setPosition(closest + direction.normalized() * radius);

		float cor = std::min(restitution, other.restitution);
		setVelocity(reflection(velocity(), direction) * cor);

		return closest;
	}
}

Vec Ball::resolve_collision(Ball& other) {

	Vec delta = position() - Vec{ other.x, other.y };	// Add a center vector to Circle?


	if (delta.length() < radius + other.radius) {
		double overlap = 0.5 * (delta.length() - radius - other.radius);

		Vec dir = delta.normalized();
		Vec correction = dir * overlap;

		setPosition(position() - correction);
		other.setPosition(other.position() + correction);

		double v1 = dot(velocity(), dir);
		double v2 = dot(other.velocity(), dir);

		float cor = std::min(restitution, other.restitution);

		double new_v1 = (mass() * v1 + other.mass() * v2 - other.mass() * (v1 - v2) * cor) / (mass() + other.mass());
		double new_v2 = (mass() * v1 + other.mass() * v2 - other.mass() * (v2 - v1) * cor) / (mass() + other.mass());

		setVelocity(velocity() + dir * (new_v1 - v1));
		other.setVelocity(other.velocity() + dir * (new_v2 - v2));
		

		Line delta_line{ 0, position().x, position().y, other.x, other.y };
		return closest_point(delta_line, *this);
	}

}

Vec Ball::resolve_collision_obstacle(Ball& other) {
	Vec delta = position() - Vec{ other.x, other.y };
	float dist = delta.length();
	float overlap = dist - radius - other.radius;

	if (overlap < 0) {
		Vec dir = delta.normalized();
		Vec correction = dir * overlap;
		setPosition(position() - correction);

		float v1 = dot(velocity(), dir);
		float v2 = 0.01; //TODO: This is wrong!
		double m_obs = 10000000000000.0;
		//TODO: Compute the correct new_v1 assuming infinite mass obstacle
		float cor = std::min(restitution, other.restitution);
		float new_v1 = (mass() * v1 + m_obs * v2 - m_obs * (v1 - v2) * cor) / (mass() + m_obs); 

		setVelocity(velocity() + dir * (new_v1 - v1));

		Line delta_line{ 0, position().x, position().y, other.x, other.y };
		return closest_point(delta_line, *this);
	}
}


Vec Ball::resolve_collision(BoxRigid& other, bool is_flipper)
{

	Line closest_line;
	Vec closestPoint;
	float max_distance_so_far = 100000000.0;

	for (Line& line : other.lines) {
		Vec cp = closest_point(line, *this);
		Vec vector_closest_circle = position() - cp;

		if (vector_closest_circle.length() < max_distance_so_far) {
			closestPoint = cp;
			closest_line = line;
			max_distance_so_far = vector_closest_circle.length();
		}
	}

	// Storing to use later
	Vec old_position = position();
	Vec old_velocity = velocity();

	resolve_collision(closest_line);

	if (is_flipper) {
		Vec collision_distance = closestPoint - other.position();
		Vec surfaceVelocity = Vec{ -collision_distance.y, collision_distance.x } * other.angular_velocity();

		Vec dir = position() - closestPoint;
		
		// Not using velocity() because it might have already updated in collision resolution with lines.
		float m1 = mass();
		float m2 = other.mass;
		float v_ball = dot(old_velocity, dir.normalized());		
		float v_flip = dot(surfaceVelocity, dir.normalized());
		
		float cor = std::min(restitution, other.restitution);
		float new_v;
		if (v_ball > v_flip) {
			new_v = (m1 * v_ball + m2 * v_flip - m2 * (v_ball - v_flip) * cor) / (m1 + m2);
		}
		else {
			m2 = 100;
			new_v = (m1 * v_ball + m2 * v_flip - m2 * (v_ball - v_flip) * cor) / (m1 + m2);
		}
		
		setVelocity(velocity() + dir.normalized() * (new_v - v_ball));
	}

	return closestPoint;
}
 