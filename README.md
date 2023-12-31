# Pinball
## vectors_and_collisions contain the vector and collision detection libraries
## pinball contains the code for the pinball game and the physics

### Video
https://youtu.be/xz8Y2HDfwtQ?si=kMJlZsT2l3rX4-JZ
https://youtu.be/HACWhvSED8E?si=0smM92yNTfkHlnGV

## Description
### Basic Pinball Dynamics: 
A ball falling down the screen accelerating under gravity is simulated. The ball has natural bouncing interactions with other objects and the environment as it falls down. Implemention was done by updating velocity at each time step based on the acceleration due to gravity. After that position was updated based on the current position and velocity and the timestep. Balls colliding with other objects were reflected from the surfaces using vector reflection while considering the coefficient of restitution to give the motion a more natural feel.

### Multiple Balls Interacting: 
Multiple balls interacting with each other while falling down and interacting with the environment was implemented. To handle ball-ball collisions first collision detection was done by treating the balls as circles. If the distances between the circles was greater than the sum of the radii then collision did not happen, otherwise a collision was recorded. If a collision was detected then the balls were separated by the overlap to resolve collision and then velocity of the balls were updated. To update the velocity of the balls, the momentum conservation and the coefficient of restitution was considered.

### Circular Obstacles: 
Circular obstacles were added on screen to show balls' interaction with immovable obstacles. Similar to the multiple balls interaction case, the ball and the circular obstacles were tested for collision using circle-circle collision test. If the ball collided with the circular object, the was moved in the opposite direction of its movement before the collision by the amount of overlap.

### Line-Segment and/or Polygonal Obstacles: 
The borders were implemented as line segments. Line-segments other than the borders were also added to the game. Polygonal obstacle were also included. To check the collision between the balls and the polygonal obstacles, the polygonal obstacles were first seperated into multiple edges which are line-segments. Then the closest points were determined from the ball to all the line segments. After that the closest point and the closest line from the ball was determined. Then, the line-circle collision test using CCD was used to detect collision. Furthermore, if the line-segment was not a part of a polygon then it was treated as a immovable obstacle and collision resolution was done by reflecting the velocity vector of the ball. To perform the reflection operation, the normal vector was determined by calculating the perpendicular vector of the line-segment. The coefficient of restitution was also multiplied with the ball's velocity vector for achieve the natural feel of some kinetic energy dissipation.

### Particle System Effects: 
When a ball collides with any object a particle system is triggered. The particle system used in this code has spawns 500 particles every time a collision is detected at the point of collision. The particles are also given a lifetime of 3 seconds. The particles transparency or the alpha value tuned based on its remaining lifetime every time step before drawing each frame.  Each particle is given a random velocity generated using a random speed and angle value at the time of particle creation. The x and the y components of the velocity vector of each particle is set to speed*cos(angle) and speed*sin(speed) respectively. This position vector updates based on this velocity vector results in particles radiating around the collision point. The original particle sytem code is taken from https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php. However, some modification to the code was done to meet this project's needs.

### Plunger/Launcher to shoot balls: 
A plunger was implemented as well. The plunger is an instance of the ball class but it is drawn to screen as a rectangle. Physics for the plunger object was not written seperately. Pressing w adds some upward velocity per timestep otherwise, the plunger falls down due to gravity. There is a celling for the plunger that the plunger cannot cross in the upward direction. However, the balls do not collide with that celling. So, placing the balls above the plunger and adding upward velocity to the plunger can cause a collision between the ball and the plunger. The collision resolution results in the ball being launched to the main portion of the game.

### Textured Background: 
A texture was added to the background. This background texture is drawn to screen before anything else.

### Textured Obstacles: 
Textures were also added to the circular obstacles and the pinballs.

### Reactive Obstacles: 
The circular obstacles change color after being hit and the new color is shown for 1000 timesteps. If the circular obstacle was not hit by the ball in the last 1000 timesteps then it is filled with the default color (different default colors and textures were chosen for different obstacles at the start of the game).

### Sound Effects: 
Every time a ball hits a surface, a sound effect representing the hit is played.

### Score Display: 
Top right corner of the screen shows the current score graphically.

### Multiple Material Types: 
To represent multiple material types, different textures were applied to different obstacles. For different "kick-backs" from different obstacles, the coefficient of restitution attached with the object was assigned different values. Higher values of which resulted in higher extra bouncy or "kick-back" effects.

### Camera: 
A camera that can follow the ball is implemented.

### Two Flippers: 
Two flippers were added that can be individually controlled by the player. The left and the right flippers will rotate if the keys A and D are held respectively. The edges of the flippers are treated as line-segments. Therefore, CCD was used to detect collision between the edges of a flipper and a ball. To resolve collision, first the velocity vector of the ball was reflected using the normal from the closest point on the flipper. After that if the flippers were not at their initial state, that meant they moved from their initial position and could hit the ball to add more velocity to the ball. Using the momentum conservation, this extra velocity was calculated. The distance from the point where a flipper is fixed (flipper origin) and the hit point is considered while calculating this extra velocity. If the ball was hit by the flipper at the flipper origin, then the extra velocity would be zero and as distance increased the extra velocity added to the ball was also increased.
