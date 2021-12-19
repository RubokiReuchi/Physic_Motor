#include "App.h"
#include "Physics.h"
#include "Player.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"
#include "Fonts.h"

#include <math.h>

Player::Player() : Module()
{
}

// Destructor
Player::~Player()
{
}

bool Player::Awake()
{
	bool ret = true;

	return ret;
}

bool Player::Start()
{
	losec = 10;

	char lookupTableChars[] = { " !'#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[/]^_ abcdefghijklmnopqrstuvwxyz{|}~ çüéâäàaçêëèïîìäaéÆæôöòûù" };
	textFont = app->fonts->Load("Assets/pixel_font.png", lookupTableChars, 8);


	body = app->physics->CreateBall(30, 10, 50, 200, 0, 0);
	x = (app->physics->balls.At(body)->x);
	y = (app->physics->balls.At(body)->y);
	double rad = (app->physics->balls.At(body)->rad);

	app->col->RemoveCollider(app->physics->balls.At(body)->ball_col);
	app->physics->balls.At(body)->ball_col = nullptr;
	app->physics->balls.At(body)->ball_col = app->col->AddCollider({ x - rad, y - rad, rad * 2, rad * 2 }, Collider::Type::PLAYER, this);
	return true;

	return true;
}

// 
bool Player::PreUpdate()
{
	x = METERS_TO_PIXELS(app->physics->balls.At(body)->x);
	y = METERS_TO_PIXELS(app->physics->balls.At(body)->y);
	
	return true;
}

bool Player::Update(float dt)
{
	if (app->physics->winc == true) {

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			angle_shot -= 0.1;
		}

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			angle_shot += 0.1;
		}

		if (angle_shot > 1)
		{
			angle_shot = 1;
		}
		else if (angle_shot < -1)
		{
			angle_shot = -1;
		}

		objective_x = angle_shot;
		objective_y = sqrt(1 - pow(angle_shot, 2));

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			strength += 0.3;
		}
		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			/*int xM, yM;
			SDL_GetMouseState(&xM, &yM);

			double objective_x = (double)xM - x;
			double objective_y = (double)yM - y;*/

			if (strength > 15)
			{
				strength = 15;
			}

			app->physics->CreateBall(10, 10, x, y, objective_x * strength, -objective_y * strength);
  			losec--;
			strength = 0;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{

			switch (app->physics->movement)
			{
			case 1:
				app->physics->balls.At(body)->SetVelocity(-3, app->physics->balls.At(body)->GetVelocity().y);
				break;
			case 2:
				app->physics->balls.At(body)->AddForce(-3000, 0);
				break;
			case 3:
				app->physics->balls.At(body)->SetPosition(-3, 0);
				break;
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			app->physics->balls.At(body)->SetVelocity(0, app->physics->balls.At(body)->GetVelocity().y);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			switch (app->physics->movement)
			{
			case 1:
				app->physics->balls.At(body)->SetVelocity(3, app->physics->balls.At(body)->GetVelocity().y);
				break;
			case 2:
				app->physics->balls.At(body)->AddForce(3000, 0);
				break;
			case 3:
				app->physics->balls.At(body)->SetPosition(3, 0);
				break;
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			app->physics->balls.At(body)->SetVelocity(0, app->physics->balls.At(body)->GetVelocity().y);
		}

		if (losec == 0)
		{
			app->physics->winc = false;
		}

		return true;

	}
	else
	{
		if (losec != 0)
		{
			app->fonts->BlitText(1280/2 - 50, 720/2 - 50, textFont, "YOU WIN|PRESS F5 TO CONTINUE|");
			
		}
		else
		{
			app->fonts->BlitText(1280 / 2 - 50, 720 / 2 - 50, textFont, "YOU LOSE|PRESS F5 TO CONTINUE|");
			losec = 10;
		}

		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->physics->winc = true;
		}
	}
	
	return true;
}

bool Player::PostUpdate()
{
	double px = METERS_TO_PIXELS(app->physics->balls.At(body)->x);
	double py = METERS_TO_PIXELS(app->physics->balls.At(body)->y);

	app->render->DrawCircle(METERS_TO_PIXELS(app->physics->balls.At(body)->x), METERS_TO_PIXELS(app->physics->balls.At(body)->y), METERS_TO_PIXELS(app->physics->balls.At(body)->rad), 0, 0, 255);

	if (strength < 3)
	{
		app->render->DrawLine(px, py, px + objective_x * 30, py - objective_y * 30, 0, 255, 0);
	}
	else if (strength > 15)
	{
		app->render->DrawLine(px, py, px + objective_x * 150, py - objective_y * 150, 0, 255, 0);
	}
	else
	{
		app->render->DrawLine(px, py, px + objective_x * strength * 10, py - objective_y * strength * 10, 0, 255, 0);
	}
	
	return true;
}

void  Player::OnCollision(Collider* c1, Collider* c2) {


}

// Called before quitting
bool Player::CleanUp()
{
	return true;
}