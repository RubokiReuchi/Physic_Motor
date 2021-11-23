#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Defs.h"
#include "Log.h"

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
	int x, y, w = 50, h = 50;
	float speed = 5;

	Ball* body;
};

#endif