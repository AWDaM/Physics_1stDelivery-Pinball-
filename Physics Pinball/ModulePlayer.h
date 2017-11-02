#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void CreateBall();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void SpawnNextBall();

	void youDied();
	
public:
	PhysBody* ball;
	int i = 0;
	int highscore;
	int score;
	int lives;
	bool youFell;
	bool isInGame;
	bool toDestroy;
	iPoint ballPossition;
};