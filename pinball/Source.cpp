#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Vec.hpp"
#include "trials.hpp"
#include "GameShapes.hpp"
#include "Ball.hpp"
#include "Flipper.h"
#include "Source.h"
#include <random>

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<int> uni(-100, 100);


const int IMMOVABLE_OBJECT_ID = 1000;

const double COR = 0.95f;
double R = 20.0;
double PLUNGER_HEIGHT = 100.0;

const int HEIGHT = 1000;
const int WIDTH = 800;

Line BORDER_LEFT {0, 0, 0, 0, HEIGHT};
Line BORDER_RIGHT {0, WIDTH, 0, WIDTH, HEIGHT};
Line BORDER_TOP {0, 0, 0, WIDTH, 0};
Line BORDER_BOTTOM {0, WIDTH*0.9, HEIGHT, WIDTH, HEIGHT};




// Ball-Wall Collision (account for radius)
void handle_border_collision(Ball* ball, std::vector<Line>& borders) {
    for (Line border : borders) {
        if (ball->collides(border)) {
            ball->resolve_collision(border);
        }
    }
}


Vec handle_circle_ball_collision(int* score, std::vector<Ball*> bumpers, Ball* ball) {

    Vec col_point{-9999, -9999};

    for (int i = 0; i < bumpers.size(); i++) {
        if (circle_circle_intersect(*bumpers[i], *ball)) {
            bumpers[i]->sfml_repr.setFillColor(sf::Color::Green);
            col_point = ball->resolve_collision_obstacle(*bumpers[i]);
            ball->setVelocity(ball->velocity() * 1.2);
            bumpers[i]->time_since_collision = 0;
            if (i < 2) {
                *score += 100;
            }
            else { 
                *score += 10;
            }
        } else{
            bumpers[i]->time_since_collision += 1;
        }

        if (bumpers[i]->time_since_collision > 1000) {
            bumpers[i]->sfml_repr.setFillColor(sf::Color::White);
        }

    }

    return col_point;
}

// void handle_box_circle_collision(int* score, BoxRigid* leftF, BoxRigid* rightF, Ball* ball) {
Vec handle_box_circle_collision(int* score, Flipper* leftF, Flipper* rightF, Ball* ball) {
    
    Vec col_point{-9999, -9999};

    if (ball->collides(*leftF)) {
        leftF->sfml_repr.setFillColor(sf::Color::Red);
        col_point = ball->resolve_collision(*leftF, true);
        // ball->vy *= -ball->restitution;
    }

    if (ball->collides(*rightF)) {
        rightF->sfml_repr.setFillColor(sf::Color::Red);
        // ball->vy *= -ball->restitution;
        col_point = ball->resolve_collision(*rightF, true);
    }

    return col_point;
}


// void physics(int* score, Ball* ball, sf::RenderWindow* window, Ball* bumpers[], BoxRigid* leftF, BoxRigid* rightF, double dt){
Vec physics(int* score, Ball* ball, sf::RenderWindow* window, std::vector<Ball*> bumpers, Flipper* leftF, Flipper* rightF, std::vector<Line>& borders, double dt){
    int width = window->getSize().x;
    int height = window->getSize().y;

    ball->setVelocity(ball->velocity() + ball->acceleration() * dt);
    ball->setPosition(ball->position() + ball->velocity() * dt);

    // for (int i = 0; i < 3; i++) {
        // bumpers[i].setVelocity(circles[i].velocity() + circles[i].acceleration() * dt);
        // bumpers[i].setPosition(circles[i].position() + circles[i].velocity() * dt);
        // handle_border_collision(bumpers[i]);
    // }
    // ball->sfml_repr.rotate(0.01f);
    
    Vec collision_point_border {-9999, -9999};
    for (Line border : borders) {
        if (ball->collides(border)) {
            collision_point_border = ball->resolve_collision(border);
        }
    }

    // handle_border_collision(ball, borders);
    Vec circle_ball_collision_point = handle_circle_ball_collision(score, bumpers, ball);
    Vec flipper_ball_collision_point = handle_box_circle_collision(score, leftF, rightF, ball);
    
    for (Vec cp : {collision_point_border, circle_ball_collision_point, flipper_ball_collision_point}) {
        if (cp.x != -9999 && cp.y != -9999) {
            return cp;
        }
    }

    return Vec{-9999, -9999};
}

void controlFlippers(Flipper& leftF, Flipper& rightF, float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        leftF.sfml_repr.setFillColor(sf::Color::Green);
        leftF.flip(dt);
    } else {
        // leftF.sfml_repr.setFillColor(sf::Color::Blue);
        leftF.unflip(dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        rightF.sfml_repr.setFillColor(sf::Color::Green);
        rightF.flip(dt);
    } else {
        // rightF.sfml_repr.setFillColor(sf::Color::Blue);
        rightF.unflip(dt);
    }
}

void updateView(sf::RenderWindow& window, Ball& pinball, sf::View& view)
{
    sf::Vector2u windowSize = window.getSize();
    float px = pinball.x - pinball.radius;
    float py = clamp(pinball.y - pinball.radius, HEIGHT * 0.30, HEIGHT * 0.70);
    view.setCenter(px, py);
    view.setSize(sf::Vector2f(windowSize.x * 1.8, windowSize.y * 1.8));
    // window.setView(view);
}

int main() {

    double dt = 1.0 / 150.0;

    // Anti-aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Window
    sf::RenderWindow window{ sf::VideoMode(WIDTH, HEIGHT), "Pinball [CSCI 5611]", sf::Style::Default, settings};
    
    // View
    sf::View view{ sf::FloatRect{0, 0, WIDTH, HEIGHT} };

    // Borders
    BORDER_LEFT.sfml_repr.setSize(sf::Vector2f(-20, BORDER_LEFT.p2.y+20));
    BORDER_RIGHT.sfml_repr.setSize(sf::Vector2f(20, BORDER_RIGHT.p2.y+20));
    BORDER_TOP.sfml_repr.setSize(sf::Vector2f(BORDER_TOP.p2.x+20, -20));
    BORDER_BOTTOM.sfml_repr.setSize(sf::Vector2f(100, 20));

    std::vector<Line> borders = { BORDER_BOTTOM, BORDER_LEFT, BORDER_RIGHT, BORDER_TOP };

    Line BORDER_BOTTOM_L_INCLINED {0, 0, HEIGHT, WIDTH*0.45, HEIGHT*1.4};
    Line BORDER_BOTTOM_R_INCLINED {0, WIDTH*0.55, HEIGHT*1.4, WIDTH, HEIGHT};
    
    // Rectangular Obstacles
    std::vector<std::vector<float>> box_params = {
        {0.20, 1.1, 0.50, 0.02, 30},   // left
        {0.72, 1.1, 0.50, 0.02, -30},  // right
        {0.92, 0.1, 0.32, 0.008, 60},  // Plunger slanter

        {0.15, .7, 0.32, 0.008, 70},           
        {0.75, .7, 0.32, 0.008, -70},           

        {-0.012, -0.01, 0.025, 0.021, 0},  // box
        {0.45, 0.5, 0.12, 0.1, -45},  // box
        // {0.2, 0.3, 0.1, 0.1, 45},  // box
    };

    std::vector<BoxRigid> boxes;
    for (std::vector<float> params : box_params) {
        boxes.push_back(BoxRigid{WIDTH*params[0], HEIGHT*params[1], WIDTH*params[2], HEIGHT*params[3]});
        boxes.back().angle = params[4] * 3.14159/ 180.0;
        boxes.back().update_corners();
        boxes.back().sfml_repr.setFillColor(sf::Color::White);
    }



    Ball plunger (WIDTH*0.97, HEIGHT*0.95, std::min(WIDTH, HEIGHT) * 0.03);
    plunger.setVelocity(0, 0);
    sf::RectangleShape plunger_rect;
    plunger_rect.setPosition(plunger.x - plunger.radius, plunger.y - plunger.radius);
    plunger_rect.setSize(sf::Vector2f(plunger.radius*2, plunger.radius*2));
    plunger_rect.setFillColor(sf::Color::Red);


    BoxRigid plunger_blockers[2] = {
        BoxRigid {WIDTH*0.93, HEIGHT*0.7, WIDTH*0.01, HEIGHT*0.6},
        BoxRigid {WIDTH*0.97, HEIGHT*0.85, WIDTH*0.1, HEIGHT*0.01}
    };
    for (BoxRigid& blocker : plunger_blockers) {
        blocker.sfml_repr.setFillColor(sf::Color::White);
    }

    for (auto& border : borders) 
        border.sfml_repr.setFillColor(sf::Color::White);


    // Pinball
    std::vector<Ball*> pinballs;
    pinballs.push_back(new Ball {WIDTH-R, HEIGHT-R-PLUNGER_HEIGHT, R});
    pinballs.push_back(new Ball {WIDTH-6*R, HEIGHT-10*R-PLUNGER_HEIGHT, R});

    
    // For debugging
    pinballs[0]->setPosition(WIDTH*0.96, 400);
    pinballs[1]->setPosition(WIDTH*0.96, 500);

    // Flipper
    Flipper leftF{WIDTH*0.30, HEIGHT*0.8, WIDTH*0.25, HEIGHT*0.02, 1};
    Flipper rightF{WIDTH*0.60, HEIGHT*0.8, WIDTH*0.25, HEIGHT*0.02, -1};

    // Obstacles
	std::vector<Ball*> bumpers;
    bumpers.push_back(new Ball(WIDTH*0.25, HEIGHT*0.3, std::min(WIDTH, HEIGHT) * 0.07));
    bumpers.push_back(new Ball(WIDTH*0.65, HEIGHT*0.3, std::min(WIDTH, HEIGHT) * 0.07));

    // flipper attached to bumper
    bumpers.push_back(new Ball(WIDTH*0.2, HEIGHT*0.8, std::min(WIDTH, HEIGHT) * 0.075));
    bumpers.push_back(new Ball(WIDTH*0.7, HEIGHT*0.8, std::min(WIDTH, HEIGHT) * 0.075));

    for (auto bumper : bumpers) {
        bumper->sfml_repr.setOutlineThickness(0.5);
        bumper->sfml_repr.setOutlineColor(sf::Color::White);
    }


	sf::Texture balltex, flipper_tex, bumper_tex;
	balltex.loadFromFile("textures/ball.jpg");
	bumper_tex.loadFromFile("textures/bumper.jpg");

	sf::Texture bumper_tex0;
	bumper_tex0.loadFromFile("textures/b100.png");

    flipper_tex.loadFromFile("textures/flipper.jpg");
    leftF.sfml_repr.setTexture(&flipper_tex);
    rightF.sfml_repr.setTexture(&flipper_tex);
	leftF.sfml_repr.setOutlineThickness(0.5);
	leftF.sfml_repr.setOutlineColor(sf::Color::White);
	rightF.sfml_repr.setOutlineThickness(0.5);
	rightF.sfml_repr.setOutlineColor(sf::Color::White);


    for (Ball *pinball: pinballs) {
        pinball->sfml_repr.setTexture(&balltex);
    }

	for (int i = 0; i < bumpers.size(); i++) {
        bumpers[i]->setVelocity(0, 0);
        bumpers[i]->setAcceleration(0, 0);
        bumpers[i]->sfml_repr.setTexture(&bumper_tex);
        bumpers[i]->restitution = 0.7f;
        if (i < 2)
            bumpers[i]->m *= 1.5;
	}
    
    // Different bumper types
	bumpers[0]->sfml_repr.setTextureRect(sf::IntRect(0, 0, bumper_tex0.getSize().x, bumper_tex0.getSize().y));
    bumpers[0]->sfml_repr.setTexture(&bumper_tex0);
    bumpers[0]->restitution = 0.95f;
	bumpers[1]->sfml_repr.setTextureRect(sf::IntRect(0, 0, bumper_tex0.getSize().x, bumper_tex0.getSize().y));
    bumpers[1]->sfml_repr.setTexture(&bumper_tex0);
    bumpers[1]->restitution = 0.95f;

    
    // Load font
    sf::Font font;
    if (!font.loadFromFile("fonts/sb.ttf")) { std::cout << "Error loading font" << std::endl; }

    // Scores
    int score = 0;
    sf::Text text;
    text.setCharacterSize(48);
    text.setFont(font);
    // text.setString("Score: " + std::to_string(score));
    text.setFillColor(sf::Color::White);
    text.setPosition(WIDTH - 350, 0);

    sf::Text text2;
    text2.setCharacterSize(100);
    text2.setFont(font);
    text2.setString("Game Over");
    text2.setFillColor(sf::Color::White);
    text2.setPosition(WIDTH/2 - 200, HEIGHT/2 - 50);

    // Background
    sf::Texture background;
    background.loadFromFile("textures/bg1.jpg");
    sf::Sprite backgroundSprite;
    //backgroundSprite.setPosition(-WIDTH/2, -HEIGHT/2);
    backgroundSprite.setTexture(background);

    // Collision sound
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("sound/hit.wav")) { return 1; }
    sf::Sound collisionSound(buffer);
    sf::Sound collisionSound2(buffer);
    

    Vec collision_point;
    Vec collision_point2;
    ParticleSystem particles(500);
    ParticleSystem particles2(500);
    sf::Clock clock;

    while (window.isOpen()) {
        
        sf::Event evnt;

        // Close window?
        while (window.pollEvent(evnt)) {
            switch (evnt.type) { 
            case sf::Event::Closed: window.close(); break; 
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            plunger.ay -= 30;
            plunger_rect.setFillColor(sf::Color::Green);
        } else{
            plunger.ay = 9.81;
            plunger_rect.setFillColor(sf::Color::Red);
        }

        plunger_rect.setPosition(plunger.sfml_repr.getPosition());
        plunger.setVelocity(plunger.velocity() + plunger.acceleration() * dt);
        plunger.setPosition(plunger.position() + plunger.velocity() * dt);

        if (line_circle_intersect(BORDER_BOTTOM, plunger)) plunger.resolve_collision(BORDER_BOTTOM);
        if (line_circle_intersect(BORDER_RIGHT, plunger)) plunger.resolve_collision(BORDER_RIGHT);

        for (BoxRigid& blocker : plunger_blockers) {
            if (circle_box_intersect(plunger, blocker)) {
                plunger.resolve_collision(blocker, false);
            }
        }

        for (Ball* pinball: pinballs) {
            if (circle_circle_intersect(plunger, *pinball)) {
                plunger.resolve_collision(*pinball);
            }
            if (circle_box_intersect(*pinball, plunger_blockers[0])) {
                pinball->resolve_collision(plunger_blockers[0], false);
            }
        }


        leftF.sfml_repr.setFillColor(sf::Color::White);
        rightF.sfml_repr.setFillColor(sf::Color::White);
        controlFlippers(leftF, rightF, dt);

        // Calculate physics
        collision_point = physics(&score, pinballs[0], &window, bumpers, &leftF, &rightF, borders, dt);
        collision_point2 = physics(&score, pinballs[1], &window, bumpers, &leftF, &rightF, borders, dt);

        if (circle_circle_intersect(*pinballs[0], *pinballs[1])) {
            // pinballs[0]->sfml_repr.setFillColor(sf::Color::White);
            Vec col_point = pinballs[1]->resolve_collision(*pinballs[0]);
        }

        // Tesing box collision
        Vec col_new {-9999, -9999};
        Vec col_new1 {-9999, -9999};
        for (BoxRigid box : boxes) {

            if (pinballs[0]->collides(box)) {
                col_new = pinballs[0]->resolve_collision(box, false);
            }
            if (pinballs[1]->collides(box)) {
                col_new1 = pinballs[1]->resolve_collision(box, false);
            }
        }


        // Setting the camera to move with the ball
        updateView(window, *pinballs[0], view);
        
        window.clear();

        // Draw the background first
        window.draw(backgroundSprite);
        
        // Draw the score
        text.setString("Score: " + std::to_string(score));
        window.draw(text);

        // Draw Borders
        for (Line border : borders) window.draw(border.sfml_repr);

        // Draw boxes
        for (BoxRigid box : boxes) window.draw(box.sfml_repr);
        
        // Draw game objects
        for (Ball* pinball: pinballs) {
            window.draw(pinball->sfml_repr);
        }

        window.draw(leftF.sfmlObj());
        window.draw(rightF.sfmlObj());

        if (collision_point.x != -9999 && collision_point.y != 9999) {
            collisionSound.play();
            particles.setEmitter(collision_point.x, collision_point.y);
        }
        if (collision_point2.x != -9999 && collision_point2.y != 9999) {
            collisionSound2.play();
            particles2.setEmitter(collision_point2.x, collision_point2.y);
        }
        if (col_new.x != -9999 && col_new.y != 9999) {
            collisionSound.play();
            particles.setEmitter(col_new.x, col_new.y);
        }
        if (col_new1.x != -9999 && col_new1.y != 9999) {
            collisionSound2.play();
            particles2.setEmitter(col_new1.x, col_new1.y);
        }
        
        sf::Time elapsed = clock.restart();
        particles.update(elapsed);
        particles2.update(elapsed);
    
        window.draw(plunger.sfmlObj());
        window.draw(plunger_rect);

        for (BoxRigid& plunger_blocker : plunger_blockers) {
            window.draw(plunger_blocker.sfml_repr);
        }

		window.draw(particles);
		window.draw(particles2);

        if (pinballs[0]->velocity().length() < 1) {
            particles.setZero();
        }
        if (pinballs[1]->velocity().length() < 1) {
            particles2.setZero();
        }

        for (int i = 0; i < bumpers.size(); i++)
            window.draw(bumpers[i]->sfmlObj());

        for (Ball* pinball : pinballs) {
            if (pinball->position().y > HEIGHT * 1.3) {
                pinball->setPosition(0.96 * WIDTH, 800);
            }
        }
        
        view.setCenter(WIDTH / 2, HEIGHT / 2);
        view.zoom(1.2);
        window.setView(view);
        window.display();
    }


    return 0;
}

