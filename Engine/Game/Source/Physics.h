#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Defs.h"
#include "Log.h"

class Ball
{
public:
	// Position
	// You could also use an array/vector
	double x;
	double y;
	double rad;

	// Velocity
	double vx;
	double vy;

	// Acceleration
	double ax;
	double ay;

	//Angle
	double angle = sqrt(vx * vx + vy * vy);

	// Force (total) applied to the ball
	double fx;
	double fy;

	// Mass
	double mass;

	// Aerodynamics stuff
	/*double surface; // Effective wet surface
	double cl; // Lift coefficient
	double cd; // Drag coefficient
	*/
	// Has physics enabled?
	bool physics_enabled = true;
	bool gravity_enabled = true;
};

class Ground
{
public:
	int x;
	int y;
	int w;
	int h;
};

class Physics : public Module
{
public:
	Physics();

	virtual ~Physics();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void integrator_velocity_verlet(Ball& ball, double dt);

	Ball* ball;
	Ground* ground;

private:

	bool debug;
};

#endif