#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
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
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	p2SString* tmp = new p2SString("Current score: %i", score);
	App->window->SetTitle(tmp->GetString());

	return UPDATE_CONTINUE;
}



