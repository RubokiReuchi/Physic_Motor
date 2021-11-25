#include "App.h"
#include "Physics.h"
#include "Player.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

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

	return true;
}

// 
bool Player::PreUpdate()
{
	/*x = body->x;
	y = body->y;
	*/
	return true;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

	}

	return true;
}


// 
bool Player::PostUpdate()
{
	//app->render->DrawRectangle({ x, y, w, h }, 0, 0, 255, 255, false);

	return true;
}


// Called before quitting
bool Player::CleanUp()
{
	return true;
}