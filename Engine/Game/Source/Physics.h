#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Defs.h"
#include "Log.h"
#include "DynArray.h"
#include "Point.h"

#define METERS_PER_PIXEL 0.02f
#define PIXELS_PER_METER 50.0f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXELS_TO_METERS(p) ((float) METERS_PER_PIXEL * p)

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
	double surface; // Effective wet surface
	double cl; // Lift coefficient
	double cd; // Drag coefficient
	
	// Has physics enabled?
	bool physics_enabled = true;
	bool gravity_enabled = true;

	void SetVelocity(double vx, double vy);

	dPoint GetVelocity();
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

	//void Integrator_velocity_verlet(Ball& ball, double dt);

	int CreateBall(double mass, double rad, double x, double y, double vx, double vy);

	DynArray<Ball> balls;
	int n_balls = 0;
	Ground* ground;

private:

	bool debug;

};

#endif