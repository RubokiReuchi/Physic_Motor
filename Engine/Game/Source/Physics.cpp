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

	integer = 1;
	movement = 1;

	return true;
}

// 
bool Physics::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		integer = 1;
	}
	else if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		integer = 2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		integer = 3;
	}

	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		movement = 1;
	}
	else if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		movement = 2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		movement = 3;
	}

	return true;
}

bool Physics::Update(float dt)
{
	for (size_t i = 0; i < balls.Count(); i++)
	{
		// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
		balls.At(i)->fx = balls.At(i)->fy = 0.0;
		balls.At(i)->ax = balls.At(i)->ay = 0.0;

		// Step #1: Compute forces

		// Compute Gravity force
		double fgx = balls.At(i)->mass * 0.0;
		double fgy;
		fgy = balls.At(i)->mass * GRAVITY; // Let's assume gravity is constant and downwards
	
		//Buoyancy
		double fbuoiancy = GRAVITY * ground->density * balls.At(i)->volume;
		double fbu = -fbuoiancy;

		//Hidrodynamic Drag
		double H = 20; //variable a cambiar
		double Vfh = sqrt(pow(balls.At(i)->vx, 2) + pow(balls.At(i)->vy, 2));

		
		double Dfhx = -(balls.At(i)->vx / Vfh);
		double Dfhy = -(balls.At(i)->vy / Vfh);

		double Fhdx = Dfhx * H;
		double Fhdy = Dfhy * H;
		
		/*
		double Dfhx = -((balls.At(i)->vx + balls.At(i)->vy)/ sqrt(pow(balls.At(i)->vx, 2) + pow(balls.At(i)->vy, 2)));

		double Fhd = Dfhx * H;
		*/

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
				balls.At(i)->fy += fgy;   //Gravity
				balls.At(i)->fx += fgx;
			}
			if (balls.At(i)->y >= ground->y - balls.At(i)->rad && balls.At(i)->x >10)
			{
				balls.At(i)->fy += fbu;   //Buoyancy

				balls.At(i)->fy += Fhdy;   //Hidrodynamic Drag	
				balls.At(i)->fx += Fhdx;
			}
		}

		//Movimiento bola
		balls.At(i)->fx += balls.At(i)->mfx;
		balls.At(i)->fy += balls.At(i)->mfy;
		balls.At(i)->mfx = 0;
		balls.At(i)->mfy = 0;

		// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		balls.At(i)->ax = balls.At(i)->fx / balls.At(i)->mass;
		balls.At(i)->ay = balls.At(i)->fy / balls.At(i)->mass;

		// Step #3: Integrate --> from accel to new velocity & new position. 
		// We will use the 2nd order "Velocity Verlet" method for integration.
		float new_dt = dt / 1000;

		//balls.At(i)->vx += balls.At(i)->vx * dh;
		
		switch (integer)
		{
		case 1:
			Integrator_velocity_verlet(balls.At(i), new_dt);
			break;
		case 2:
			Integrator_forward_euler(balls.At(i), new_dt);
			break;
		case 3:
			Integrator_backwards_euler(balls.At(i), new_dt);
			break;
		default:
			break;
		}

		// Step #4: solve collisions
		if (balls.At(i)->y >= ground->y - balls.At(i)->rad && balls.At(i)->x < 10)
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


bool Physics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return true;

	for (size_t i = 1; i < balls.Count(); i++) // is player
	{
		app->render->DrawCircle(METERS_TO_PIXELS(balls.At(i)->x), METERS_TO_PIXELS(balls.At(i)->y), METERS_TO_PIXELS(balls.At(i)->rad), 255, 0, 0);
	}
	
	app->render->DrawRectangle({ METERS_TO_PIXELS(ground->x), METERS_TO_PIXELS(ground->y), METERS_TO_PIXELS(ground->w), METERS_TO_PIXELS(ground->h) }, 0, 255, 0, 255, false);

	return true;
}

void Physics::Integrator_velocity_verlet(Ball* ball, double dt)
{
	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;

	ball->vx += ball->ax * dt;
	ball->vy += ball->ay * dt;
}

void Physics::Integrator_forward_euler(Ball* ball, double dt)
{
	ball->vx = ball->vx + ball->ax * dt;
	ball->vy = ball->vy + ball->ay * dt;

	ball->x = ball->x + ball->vx * dt;
	ball->y = ball->y + ball->vy * dt;

}

void Physics::Integrator_backwards_euler(Ball* ball, double dt)
{
	ball->x = ball->x + ball->vx * dt;
	ball->y = ball->y + ball->vy * dt;

	ball->vx = ball->vx + ball->ax * dt;
	ball->vy = ball->vy + ball->ay * dt;

}


int Physics::CreateBall(double mass, double rad, double x, double y, double vx, double vy)
{
	Ball* new_ball = new Ball();

	// Set physics properties of the ball
	new_ball->mass = mass; // kg
	new_ball->rad = PIXELS_TO_METERS(rad);
	new_ball->surface = new_ball->rad * M_PI;
	new_ball->volume = pow(new_ball->rad,3) * M_PI * 1.3333;

	// Set initial position and velocity of the ball
	new_ball->x = PIXELS_TO_METERS(x);
	new_ball->y = PIXELS_TO_METERS(y);
	new_ball->vx = vx;
	new_ball->vy = vy;

	new_ball->cd = 0.4;

	balls.Insert(*new_ball, balls.Count());  
	return balls.Count() - 1;
}


void Ball::SetVelocity(double velX, double velY)
{
	vx = velX;
	vy = velY;
}

void Ball::AddForce(double forX, double forY)
{
	mfx = forX;
	mfy = forY;
}

void Ball::SetPosition(double X, double Y) 
{
	x += PIXELS_TO_METERS(X);
	y += PIXELS_TO_METERS(Y);
}

dPoint Ball::GetVelocity()
{
	dPoint velocity = { vx, vy };

	return velocity;
}


// Called before quitting
bool Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!

	return true;
}
