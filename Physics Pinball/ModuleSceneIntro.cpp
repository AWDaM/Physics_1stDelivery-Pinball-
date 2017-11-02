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
	circle = NULL;
	ray_on = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->audio->PlayMusic("pinball/bgm.ogg");
	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	pinball_tex = App->textures->Load("pinball/Pinball small.png");
	upper_pinball_tex = App->textures->Load("pinball/UperLayer.png");
	flipper_tex = App->textures->Load("pinball/flipper.png");
	lights_tex = App->textures->Load("pinball/TopLights.png");
	spring_tex = App->textures->Load("pinball/spring.png");


	pinball_rect = App->physics->CreateRectangle(492/2, 798/2, 492, 798, b2_staticBody, false);

	App->physics->CreateP_Boundaries();
	App->physics->CreateBouncers();
	App->physics->CreateSpring();
	App->physics->CreateP_Holes();
	App->physics->CreateP_Lights();

	Rflipper_circle = new PhysBody;
	Rflipper_rectangle = new PhysBody;
	App->physics->CreateP_Flipper(Rflipper_rectangle, Rflipper_circle, true);

	Lflipper_circle = new PhysBody;
	Lflipper_rectangle = new PhysBody;
	App->physics->CreateP_Flipper(Lflipper_rectangle, Lflipper_circle, false);


	dyingSensor = App->physics->CreateRectangleSensor(200, 850, 700, 100);

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	flipper_fx = App->audio->LoadFx("pinball/flipper.wav");

	ball_active = false;
	maintainBallStopped = false;
	total_stoptime = 3000;
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
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->player->score = 0;
		App->player->lives = 3;
		App->player->isInGame = true;
		if(App->player->ball)App->physics->DestroyBall();
		App->player->CreateBall();
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipper_fx);
		Rflipper_rectangle->body->ApplyTorque(500, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipper_fx);
		Lflipper_rectangle->body->ApplyTorque(-500, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		spring->body->ApplyLinearImpulse({ 0, 10 }, { 0,0 }, true);
	}

	if (maintainBallStopped)
	{
		Stopped_ball_timer();
	}
	// Prepare for raycast ------------------------------------------------------
	if (App->player->ball)
	{
		App->physics->MaxSpeedCheckP(App->player->ball);
	}


	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	//-----------------------Blit the pinball texture
	int x, y;
	pinball_rect->GetPosition(x, y);
	App->renderer->Blit(pinball_tex, x, y, NULL, 1.0f);

	spring->GetPosition(x, y);
	App->renderer->Blit(spring_tex, x, y, NULL, 1.0f);

	Lflipper_circle->GetPosition(x, y);
	App->renderer->Blit(flipper_tex, x - 75, y - 15, NULL, 1.0f, Lflipper_rectangle->GetRotation());
	Rflipper_circle->GetPosition(x, y);
	App->renderer->Blit(flipper_tex, x - 65, y - 15, NULL, 1.0f, Rflipper_rectangle->GetRotation(), SDL_FLIP_HORIZONTAL);

	if(App->player->ball)App->renderer->Blit(circle, App->player->ballPossition.x, App->player->ballPossition.y, NULL, 1.0F, App->player->ball->GetRotation());


	p2List_item<PhysBody*>* light = Lights.getFirst();
	p2List_item<Animation*>* light_animation = lights.getFirst();
	while (light != NULL || light_animation != NULL)
	{
		int x, y;
		light->data->GetPosition(x, y);
		App->renderer->Blit(lights_tex, x, y, &light_animation->data->GetCurrentFrame());
		light = light->next;
		light_animation = light_animation->next;
	}

	pinball_rect->GetPosition(x, y);
	App->renderer->Blit(upper_pinball_tex, x, y, NULL, 1.0f);


	// ray -----------------
	/*if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}



void ModuleSceneIntro::Stopped_ball_timer()
{
	if (stopBall)
		App->player->ball = App->physics->CreateCircle(App->player->ballPossition.x + ball_radius, App->player->ballPossition.y + ball_radius, 10, b2_staticBody), stopBall = false;

	if (SDL_GetTicks() - initial_time >= total_stoptime)
	{
		maintainBallStopped = false;
		App->physics->DestroyBall();
		App->player->ball = App->physics->CreateCircle(App->player->ballPossition.x, App->player->ballPossition.y, 10, b2_dynamicBody);
		App->player->ball->body->ApplyLinearImpulse({ 3,3 }, { 0,0 }, true);
		App->player->ball->body->SetBullet(true);
	}
}
