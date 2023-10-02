#include "BoxRigid.h"
#include <cmath>

BoxRigid::BoxRigid(float x, float y, float w, float h) : BoxRigid::Box(0, x, y, w, h)
{
    center = {x, y};
    momentum = {0.0, 0.0};
    total_force = {0.0, 0.0};

    this->w = w;
    this->h = h;
    restitution = 0.8;
    mass = 1000 * w * h;

    angle = 0.0;
    rotational_inertia = mass * (w * w + h * h) / 12;
    total_torque = 0.0;

    sfml_repr.setPointCount(4);
    reset();
    sfml_update_points();
}

void BoxRigid::reset() {
    while (points.size() < 4)
        points.push_back(Vec{ 0.0, 0.0 });

    points[0] = { center.x - w / 2, center.y - h / 2 };
    points[1] = { center.x - w / 2, center.y + h / 2 };
    points[2] = { center.x + w / 2, center.y + h / 2 };
    points[3] = { center.x + w / 2, center.y - h / 2 };

    update_lines();
}

void BoxRigid::update_lines() {
    for (int i = 0; i < 4; ++i) {
        int j = (i < 3) ? i + 1 : 0;
        lines[i] = {0, points[i].x, points[i].y, points[j].x, points[j].y};
    }
}

void BoxRigid::sfml_update_points() {
    for (int i = 0; i < points.size(); ++i) 
        sfml_repr.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
}

void BoxRigid::set_point_count(int n) {
    sfml_repr.setPointCount(n);
}

void BoxRigid::set_rotation(float angle) {
    this->angle = angle;
    update_corners();
}

Vec BoxRigid::velocity() {
    return momentum / mass;
}

double BoxRigid::angular_velocity() {
    return angular_momentum / rotational_inertia;
}

Vec BoxRigid::acceleration() {
    return total_force / mass;
}

void BoxRigid::set_position(Vec p)
{
    center.x = p.x;
    center.y = p.y;
}

void BoxRigid::set_position(double px, double py)
{
    center.x = px;
    center.y = py;
}

void BoxRigid::set_velocity(double vvx, double vvy)
{
    vx = vvx;
    vy = vvy;
}

Vec BoxRigid::position() {
    return center;
}

void BoxRigid::set_center_of_mass(float x, float y) {
    center = {x, y};
}

void BoxRigid::apply_force(Vec force, Vec applied_position) {
    total_force += force;
    Vec displacement = applied_position - center;
    total_torque = cross(displacement, force);
}

void BoxRigid::update_corners()
{
    Vec right {std::cos(angle), std::sin(angle)};
    right *= w / 2;

    Vec up {-std::sin(angle), std::cos(angle)};
    up *= (- h / 2);

    points[0] = center + up - right;
    points[1] = center + up + right;
    points[2] = center - up + right;
    points[3] = center - up - right;
    
    update_lines();
    sfml_update_points();
}

void BoxRigid::update_physics(double dt) {
    // Update linear and angular momentum
    momentum += total_force * dt;
    center += velocity() * dt;
    angular_momentum += total_torque * dt;
    angle += angular_velocity() * dt;

    update_corners();

    // Reset forces and torques after applying them
    total_force = {0.0, 0.0};
    total_torque = 0.0;
}

void BoxRigid::resolve_collision(Vec hit_point, Vec hit_normal, float dt)
{
    Vec r = hit_point - center;
    Vec r_perpendicular {-r.y, r.x};
    Vec point_velocity = velocity() + r_perpendicular * angular_velocity();

    float j = - (1+restitution) * dot(point_velocity, hit_normal);
    j /= (1/mass + std::pow(dot(r_perpendicular, hit_normal), 2) / rotational_inertia);

    Vec impulse = hit_normal * j;
    momentum += impulse;
    angular_momentum += dot(r_perpendicular, impulse);

    update_physics(1.01 * dt);
}
