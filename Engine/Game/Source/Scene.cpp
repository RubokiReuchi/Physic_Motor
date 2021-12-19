#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Fonts.h"
#include "Collisions.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	char lookupTableChars[] = { " !'#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[/]^_ abcdefghijklmnopqrstuvwxyz{|}~ çüéâäàaçêëèïîìäaéÆæôöòûù" };
	textFont = app->fonts->Load("Assets/pixel_font.png", lookupTableChars, 8);
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->ChangeFps();


	if (app->col->debug == false) {
		app->fonts->BlitText(0, 0, textFont, "F2 TO SHOW COLIDERS");
	}
	if (app->col->debug == true) {
		app->fonts->BlitText(0, 0, textFont, "F2 TO HIDE COLIDERS");
	}
		app->fonts->BlitText(0, 0, textFont, "                         F3 TO TOGGLE FPS");

	sprintf_s(ballsLeft, 3, "%02d", 11 - app->physics->balls.Count());
	app->fonts->BlitText(0, 0, textFont, "|||||||BALLS LEFT: ");
	app->fonts->BlitText(12*10 + 12, 20*7 +7, textFont, ballsLeft);
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
