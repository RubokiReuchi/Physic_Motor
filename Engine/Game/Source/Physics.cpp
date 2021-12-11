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
	ground = new Ground();

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
	for (size_t i = 0; i < n_balls; i++)
	{
		// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
		balls.At(i)->fx = balls.At(i)->fy = 0.0;
		balls.At(i)->ax = balls.At(i)->ay = 0.0;


		// Step #1: Compute forces

		// Compute Gravity force
		double fgx = balls.At(i)->mass * 0.0;
		double fgy;
		fgy = balls.At(i)->mass * 10; // Let's assume gravity is constant and downwards
	
		//Buoyancy
		double fbuoiancy = fgy * ground->density * 1.6f;//balls.At(i)->surface;
		double fbu = -fbuoiancy;

		//Hidrodynamic Drag
		double fhidrodragy = balls.At(i)->vy * 0.5;
		double fhdy = -fhidrodragy;

		double fhidrodragx = balls.At(i)->vx * 0.5;
		double fhdx = -fhidrodragx;

		//Aerodynamic Drag
		double fdragx = 0.5 * balls.At(i)->vx * balls.At(i)->vx * balls.At(i)->surface * balls.At(i)->cd;
		double fdx = -fdragx;

		double fdragy = 0.5 * balls.At(i)->vy * balls.At(i)->vy * balls.At(i)->surface * balls.At(i)->cd;
		double fdy = -fdragy;

		// Add forces to the total accumulated force of the ball
		if (balls.At(i)->physics_enabled)
		{
			balls.At(i)->fx += fdx;
			balls.At(i)->fy += fdy;
			if (balls.At(i)->gravity_enabled)
			{
				balls.At(i)->fy += fgy;
				balls.At(i)->fx += fgx;
			}
			if (balls.At(i)->y >= ground->y - balls.At(i)->rad && balls.At(i)->x >=10)
			{
				balls.At(i)->fy += fbu;   //buoyancy
				balls.At(i)->fx += fhdx;  //hidrodynamic drag
			}
		}

		// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		balls.At(i)->ax = balls.At(i)->fx / balls.At(i)->mass;
		balls.At(i)->ay = balls.At(i)->fy / balls.At(i)->mass;

		// Step #3: Integrate --> from accel to new velocity & new position. 
		// We will use the 2nd order "Velocity Verlet" method for integration.
		// You can also move this code into a subroutine: integrator_velocity_verlet(ball, dt);
		float new_dt = dt / 1000;

		//balls.At(i)->vx += balls.At(i)->vx * dh;

		balls.At(i)->x += balls.At(i)->vx * new_dt + 0.5 * balls.At(i)->ax * new_dt * new_dt;
		balls.At(i)->y += balls.At(i)->vy * new_dt + 0.5 * balls.At(i)->ay * new_dt * new_dt;
		balls.At(i)->vx += balls.At(i)->ax * new_dt;
		balls.At(i)->vy += balls.At(i)->ay * new_dt;

		// Step #4: solve collisions
		if (balls.At(i)->y >= ground->y - balls.At(i)->rad && balls.At(i)->x <= 10)
		{
			balls.At(i)->y = ground->y - balls.At(i)->rad;
			balls.At(i)->vx = balls.At(i)->vx * 0.5;
			balls.At(i)->vy = -balls.At(i)->vy * 0.5;
			balls.At(i)->ax = balls.At(i)->ay = 0.0;
			balls.At(i)->fx = balls.At(i)->fy = 0.0;
			if (balls.At(i)->vy > -0.01 && balls.At(i)->vy < 0.01)
			{
				balls.At(i)->vy = 0.0;
				balls.At(i)->gravity_enabled = false;
			}
			//ball->physics_enabled = false;
		}

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

	for (size_t i = 1; i < n_balls; i++) // is player
	{
		app->render->DrawCircle(METERS_TO_PIXELS(balls.At(i)->x), METERS_TO_PIXELS(balls.At(i)->y), METERS_TO_PIXELS(balls.At(i)->rad), 255, 0, 0);
	}
	
	app->render->DrawRectangle({ METERS_TO_PIXELS(ground->x), METERS_TO_PIXELS(ground->y), METERS_TO_PIXELS(ground->w), METERS_TO_PIXELS(ground->h) }, 0, 255, 0, 255, false);

	return true;
}

/*void Integrator_velocity_verlet(Ball* ball, double dt)
{
	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	ball->vx += ball->ax * dt;
	ball->vy += ball->ay * dt;
}*/



// Called before quitting
bool Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!

	return true;
}

int Physics::CreateBall(double mass, double rad, double x, double y, double vx, double vy)
{
	Ball* new_ball = new Ball();

	// Set physics properties of the ball
	new_ball->mass = mass; // kg
	new_ball->rad = PIXELS_TO_METERS(rad);
	new_ball->surface = new_ball->rad * M_PI;
	new_ball->volume = new_ball->rad * M_PI * 1.3333;

	// Set initial position and velocity of the ball
	new_ball->x = PIXELS_TO_METERS(x);
	new_ball->y = PIXELS_TO_METERS(y);
	new_ball->vx = vx;
	new_ball->vy = vy;

	new_ball->cd = 0.4;

	balls.Insert(*new_ball, n_balls);
	n_balls++;
	
	return n_balls - 1;
}



void Ball::SetVelocity(double velX, double velY)
{
	vx = velX;
	vy = velY;
}

dPoint Ball::GetVelocity()
{
	dPoint velocity = { vx, vy };

	return velocity;
}