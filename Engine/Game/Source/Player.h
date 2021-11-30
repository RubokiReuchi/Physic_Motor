#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Defs.h"
#include "Log.h"

class Ball;

class Player : public Module
{
public:
	Player();

	virtual ~Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

private:
	int x, y, r = 5;
	float speed = 5;

	int body;

	double angle_shot = 0;
	double strength = 0;
	double objective_x;
	double objective_y;
};

#endif