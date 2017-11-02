#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleWindow.h"
#include "p2SString.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	score = 00000;
	lives = 3;
	youFell = false;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::CreateBall()
{
	ball = App->physics->CreateCircle(465, 640, 10, b2_dynamicBody);
	ball->listener = this;
	ball->body->SetBullet(true);
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (lives > 0)
	{
		p2SString* tmp = new p2SString("Current score: %i", score);
		App->window->SetTitle(tmp->GetString());
	}

	if (youFell)
		SpawnNextBall();

	return UPDATE_CONTINUE;
}

void ModulePlayer::youDied()
{
	p2SString* tmp = new p2SString("You lost. Final score: %i. Press NOTHING to play again.", score);
	App->window->SetTitle(tmp->GetString());
}

void ModulePlayer::SpawnNextBall()
{
	youFell = false;
	lives -= 1;
	App->physics->DestroyBall();

	if (lives > 0)
		CreateBall();
	else
		youDied();
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == App->scene_intro->dyingSensor || bodyA == App->scene_intro->dyingSensor)
		youFell = true;
}
