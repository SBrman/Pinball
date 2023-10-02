#include "Flipper.h"
#include "Objects.hpp"
#include <iostream>
#include <cmath>
#include "Utils.h"
#include <vector>


const double PI = 3.1415926535;
const float FLIPPER_SPEED = 2.0f;
const float ANGULAR_VEL = 30.0f;

Flipper::Flipper(float x, float y, float w, float h, int sign) : Flipper::BoxRigid(x, y, w, h) {	
	this->sign = sign;
	tip = Vec{ x + w / 2, y + h / 2 };
	pos = Vec{ x - w / 2, y - h / 2 };
	angle_vel = sign * ANGULAR_VEL;
	// for (Line& line : lines) { line.restitution = 0.2f; }
}

void Flipper::reset() {
	points[0] = { center.x - w / 2, center.y - h / 2 };
	points[1] = { center.x - w / 2, center.y + h / 2 };
	points[2] = { center.x + w / 2, center.y + h / 2 };
	points[3] = { center.x + w / 2, center.y - h / 2 };
}

void Flipper::sfml_flipper_fix() {
	Vec pp1 = interpolate(points[0], points[3], 0.5);
	Vec pp2 = interpolate(points[1], points[2], 0.5);
	sfml_repr.setPoint(1, sf::Vector2f(pp1.x, pp1.y));
	sfml_repr.setPoint(2, sf::Vector2f(pp2.x, pp2.y));
}

void Flipper::set_point_count(int n) {
	point_count = 4;
}

void Flipper::set_rotation(float angle) {
 	update_corners();
 	// sfml_flipper_fix();
 	// sfml_repr.setFillColor(sf::Color::Red);
 }

// void Flipper::resolve_collision(Ball& ball) {
// 	Vec dir = tip - pos;
//  	Vec dir_norm = dir.normalized();
//  	float proj = dot(ball.position() - pos, dir_norm);
//  	
//  	Vec closest_point;
//  	if (proj < 0) { closest_point = pos; } 
//  	else if (proj > dir.length()) { closest_point = tip; }
//  	else { closest_point = pos + dir_norm * proj; }
//  
//  	Vec ndir = ball.position() - closest_point;
// }

void Flipper::set_angle(float angle) {
	this->angle = clamp(angle, -ANGULAR_VEL * PI / 180, ANGULAR_VEL * PI / 180);
}

void Flipper::flip(float dt) {
	float angle_delta = sign * (FLIPPER_SPEED * ANGULAR_VEL * PI / 180) * dt;
	angle_vel = sign * ANGULAR_VEL;
	set_angle(angle - angle_delta);
	set_rotation(angle);
}

void Flipper::unflip(float dt) {
	float angle_delta = sign * (FLIPPER_SPEED * ANGULAR_VEL * PI / 180) * dt;
	angle_vel = sign * ANGULAR_VEL;
	set_angle(angle + angle_delta);
	set_rotation(angle);
}


sf::ConvexShape Flipper::sfmlObj()
{
	return sfml_repr;
}
