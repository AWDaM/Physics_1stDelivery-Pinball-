#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* pinball_rect;
	PhysBody* pinball_boundaries;
	PhysBody* pinballb_1;
	PhysBody* pinballb_2;
	PhysBody* pinballb_3;
	PhysBody* pinballb_4;
	PhysBody* pinballb_5;
	PhysBody* pinballb_6;
	PhysBody* pinballb_7;
	PhysBody* pinballb_8;

	PhysBody* Rflipper_circle;
	PhysBody* Rflipper_rectangle;
	PhysBody* Lflipper_circle;
	PhysBody* Lflipper_rectangle;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* pinball_tex;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
