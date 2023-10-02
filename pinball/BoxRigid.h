#pragma once
#include "Objects.hpp"


class BoxRigid : public Box
{
public:
    Vec center;
    Vec momentum;
    Vec vel;
    Vec total_force;

    float angle_vel = 30;
    
    float w, h;
    double vx, vy, ax, ay;
    double mass;
    float restitution;
    double rotational_inertia;
    double angular_momentum;
    double total_torque;
    double angle;

    int time_since_collision = 100;

    Line lines[4];

    std::vector<Vec> points;
    int point_count;
    sf::ConvexShape sfml_repr;
    
    BoxRigid() {}
    BoxRigid(float, float, float, float);

    void reset();
    void sfml_update_points();

    Vec position();
    Vec velocity();
    double angular_velocity();
    Vec acceleration();

    void set_position(Vec);
    void set_position(double, double);
    void set_velocity(Vec);
    void set_velocity(double, double);

    void set_point_count(int);
    void set_rotation(float);
    void set_center_of_mass(float, float);

    void apply_force(Vec, Vec);

    void update_lines();
    void update_corners();
    void update_physics(double);
    void resolve_collision(Vec hit_point, Vec hit_normal, float dt);
};

