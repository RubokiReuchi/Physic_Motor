#include "App.h"
#include "Physics.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "math.h"
#include <cmath>

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
	// Set physics properties of the ball
	ball->mass = 1; // kg
	ball->rad = 5;
	//ball.surface = 2; // m^2

	// Set initial position and velocity of the ball
	ball->x = 50.0;
	ball->y = 200.0;
	ball->vx = 0.5;
	ball->vy = -0.5;

	ground->x = 0;
	ground->y = 500;
	ground->w = 1200;
	ground->h = 50;

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
	if (ball->gravity_enabled)
	{
		fgy = ball->mass * 0.001; // Let's assume gravity is constant and downwards
	}
	

	// Add gravity force to the total accumulated force of the ball
	if (ball->physics_enabled)
	{
		ball->fx += fgx;
		if (ball->gravity_enabled)
		{
			ball->fy += fgy;
		}
	}


	// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
	ball->ax = ball->fx / ball->mass;
	ball->ay = ball->fy / ball->mass;

	// Step #3: Integrate --> from accel to new velocity & new position. 
	// We will use the 2nd order "Velocity Verlet" method for integration.
	// You can also move this code into a subroutine: integrator_velocity_verlet(ball, dt);

	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	ball->vx += ball->ax * dt;
	ball->vy += ball->ay * dt;

	// Step #4: solve collisions
	if (ball->y > ground->y - ball->rad)
	{
		// For now, just stop the ball when it reaches the ground.
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

	app->render->DrawCircle(ball->x, ball->y, ball->rad, 255, 0, 0);
	app->render->DrawRectangle({ ground->x, ground->y, ground->w, ground->h }, 0, 255, 0, 255, false);

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