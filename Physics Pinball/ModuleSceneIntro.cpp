#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include"ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	pinball_tex = App->textures->Load("pinball/Pinball small.png");

	pinball_rect = App->physics->CreateRectangle(492/2, 798/2, 492, 798, b2_staticBody, false);

	App->physics->CreateP_Boundaries();
	App->physics->CreateBouncers();
	App->physics->CreateSpring();

	Rflipper_circle = new PhysBody;
	Rflipper_rectangle = new PhysBody;
	App->physics->CreateP_Flipper(Rflipper_rectangle, Rflipper_circle, true);

	Lflipper_circle = new PhysBody;
	Lflipper_rectangle = new PhysBody;
	App->physics->CreateP_Flipper(Lflipper_rectangle, Lflipper_circle, false);


	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	ball_active = false;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	/*if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}*/

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && !ball_active)
	{
		ball = App->physics->CreateCircle(465, 640, 10, b2_dynamicBody);
		ball_active = true;
		ball->listener = this;
		ball->body->SetBullet(true);
		LOG("MASS:%i", ball->body->GetMass());
	}


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		Rflipper_rectangle->body->ApplyTorque(500, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		Lflipper_rectangle->body->ApplyTorque(-500, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		spring->body->ApplyLinearImpulse({ 0, 10 }, { 0,0 }, true);

		
	}

	// Prepare for raycast ------------------------------------------------------
	if (ball_active)
	{
		App->physics->MaxSpeedCheckP(ball);
	}


	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	if (ball_active)
	{
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0F, ball->GetRotation());
	}


	//-----------------------Blit the pinball texture
	int x, y;
	pinball_rect->GetPosition(x, y);
	App->renderer->Blit(pinball_tex, x, y, NULL, 1.0f);

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->player->score += bodyA->score + bodyB->score;

	//App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
