#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "j1Animation.h"

class PhysBody;
class SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Stopped_ball_timer();

public:
	PhysBody* pinball_rect;
	PhysBody* spring;
	PhysBody* springPivot;
	PhysBody* pinball_boundaries;
	PhysBody* pinballb_1;
	PhysBody* pinballb_2;
	PhysBody* pinballb_3;
	PhysBody* pinballb_4;
	PhysBody* pinballb_5;
	PhysBody* pinballb_6;
	PhysBody* pinballb_7;
	PhysBody* pinballb_8;
	PhysBody* pinballb_9;
	PhysBody* pinballb_10;
	PhysBody* triangleBouncer1;
	PhysBody* triangleBouncer2;
	PhysBody* circleBouncer1;
	PhysBody* circleBouncer2;
	PhysBody* circleBouncer3;
	PhysBody* circleBouncer4;
	PhysBody* dyingSensor;
	PhysBody* hole_1;
	PhysBody* hole_2;

	PhysBody* Rflipper_circle;
	PhysBody* Rflipper_rectangle;
	PhysBody* Lflipper_circle;
	PhysBody* Lflipper_rectangle;

	p2List<PhysBody*> Lights;
	p2List<Animation*>  lights;
	Animation light;
	
	SDL_Texture* circle;
	SDL_Texture* pinball_tex;
	SDL_Texture* upper_pinball_tex;
	SDL_Texture* flipper_tex;
	SDL_Texture* lights_tex;

	SDL_Texture* spring_tex;
	uint bonus_fx;
	uint flipper_fx;
	p2Point<int> ray;
	bool ray_on;
	bool ball_active;
	bool maintainBallStopped = false;
	bool stopBall = false;
	uint initial_time;
	uint total_stoptime = 3000;
	uint16_t ball_radius = 10;
};
