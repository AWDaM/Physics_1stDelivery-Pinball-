#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
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
	isInGame = false;
	toDestroy = false;

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
	if (ball)ball->GetPosition(ballPossition.x, ballPossition.y);

	if (toDestroy)
		App->physics->DestroyBall(), toDestroy = false;

	p2SString* tmp;

	if (lives > 0)
		tmp = new p2SString("Lives left: %d || Current score: %i", lives, score);
	else if (i < 50)
		tmp = new p2SString("Final score: %i || Highest Score: %d || Press 1 to play again.", score, highscore);
	else
		tmp = new p2SString("YOU LOST");


	if (++i > 100)i = 0;
	App->window->SetTitle(tmp->GetString());

	if (youFell)
		SpawnNextBall();


	return UPDATE_CONTINUE;
}

void ModulePlayer::youDied()
{
	if (score > highscore)highscore = score;

	

	isInGame = false;
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
	App->audio->PlayFx(App->scene_intro->bonus_fx);

	App->player->score += bodyB->score;

	if (bodyB == App->scene_intro->dyingSensor || bodyA == App->scene_intro->dyingSensor)
		youFell = true;

	if (bodyB == App->scene_intro->hole_1 || bodyB == App->scene_intro->hole_2)
	{
		toDestroy = true;
		App->scene_intro->maintainBallStopped = true;
		App->scene_intro->stopBall = true;
		App->scene_intro->initial_time = SDL_GetTicks();
	}

	if (App->scene_intro->Lights.find(bodyB) != -1)
	{
		bodyB->anim->current_frame = 0;
	}
}
