#include "App.h"
#include "Physics.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "math.h"
#include <cmath>

#define METERS_PER_PIXEL 0.02f
#define PIXELS_PER_METER 50.0f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXELS_TO_METERS(p) ((float) METERS_PER_PIXEL * p)

// TODO 1: Include Box 2 header and library

Physics::Physics() : Module()
{
	debug = true;
}

// Destructor
Physics::~Physics()
{
}

bool Physics::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool Physics::Start()
{
	ball = new Ball();
	ground = new Ground();
	// Set physics properties of the ball
	ball->mass = 2; // kg
	ball->rad = PIXELS_TO_METERS(5);
	//ball.surface = 2; // m^2

	// Set initial position and velocity of the ball
	ball->x = PIXELS_TO_METERS(50.0);
	ball->y = PIXELS_TO_METERS(200.0);
	ball->vx = 0.2;
	ball->vy = -2;

	ball->cd = 0.4;
	ball->surface = ball->rad * M_PI;
	ground->x = PIXELS_TO_METERS(0);
	ground->y = PIXELS_TO_METERS(500);
	ground->w = PIXELS_TO_METERS(1200);
	ground->h = PIXELS_TO_METERS(50);

	return true;
}

// 
bool Physics::PreUpdate()
{

	return true;
}

bool Physics::Update(float dt)
{
	// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
	ball->fx = ball->fy = 0.0;
	ball->ax = ball->ay = 0.0;


	// Step #1: Compute forces

		// Compute Gravity force
	double fgx = ball->mass * 0.0;
	double fgy;
	fgy = ball->mass * 10; // Let's assume gravity is constant and downwards
	

	// Add gravity force to the total accumulated force of the ball
	if (ball->physics_enabled)
	{
		ball->fx += fgx;
		if (ball->gravity_enabled)
		{
			ball->fy += fgy;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		double fdrag = 0.5 * ball->vx * ball->vx * ball->surface * ball->cd;
		double fdx = -fdrag;
		ball->fx += fdx;
	}

	// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
	ball->ax = ball->fx / ball->mass;
	ball->ay = ball->fy / ball->mass;

	// Step #3: Integrate --> from accel to new velocity & new position. 
	// We will use the 2nd order "Velocity Verlet" method for integration.
	// You can also move this code into a subroutine: integrator_velocity_verlet(ball, dt);

	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	LOG("%f", ball->x);
	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	ball->vx += ball->ax * dt;
	ball->vy += ball->ay * dt;

	// Step #4: solve collisions
	if (ball->y >= ground->y - ball->rad)
	{
		// For now, just stop the ball when it reaches the ground.
		ball->y = ground->y - ball->rad;
		ball->vx = ball->vx * 0.5;
		ball->vy = -ball->vy * 0.5;
		ball->ax = ball->ay = 0.0;
		ball->fx = ball->fy = 0.0;
		if (ball->vy > -0.01 && ball->vy < 0.01)
		{
			ball->vy = 0.0;
			ball->gravity_enabled = false;
		}
		//ball->physics_enabled = false;
	}
	else
	{
		ball->vx = ball->vx * 0.99;
	}

	return true;
}


// 
bool Physics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return true;

	app->render->DrawCircle(METERS_TO_PIXELS(ball->x), METERS_TO_PIXELS(ball->y), METERS_TO_PIXELS(ball->rad), 255, 0, 0);
	app->render->DrawRectangle({ METERS_TO_PIXELS(ground->x), METERS_TO_PIXELS(ground->y), METERS_TO_PIXELS(ground->w), METERS_TO_PIXELS(ground->h) }, 0, 255, 0, 255, false);

	return true;
}

/*
void integrator_velocity_verlet(Ball& ball, double dt)
{
	ball.x += ball.vx * dt + 0.5 * ball.ax * dt * dt;
	ball.y += ball.vy * dt + 0.5 * ball.ay * dt * dt;
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
}
*/


// Called before quitting
bool Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!

	return true;
}