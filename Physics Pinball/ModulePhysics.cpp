#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);


	// needed to create joints like mouse joint
	//b2BodyDef bd;
	//ground = world->CreateBody(&bd);

	//// big static circle as "ground" in the middle of the screen
	//int x = SCREEN_WIDTH / 2;
	//int y = SCREEN_HEIGHT / 1.5f;
	//int diameter = SCREEN_WIDTH / 2;

	//b2BodyDef body;
	//body.type = b2_staticBody;
	//body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	//b2Body* big_ball = world->CreateBody(&body);

	//b2CircleShape shape;
	//shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	//b2FixtureDef fixture;
	//fixture.shape = &shape;
	//big_ball->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type, bool fixture)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	if (fixture)
	{
		b2FixtureDef fixture;
		fixture.shape = &box;
		fixture.density = 1.0f;

		b->CreateFixture(&fixture);
	}

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type, float restCoeficient)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = restCoeficient;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

void ModulePhysics::CreateP_Boundaries()
{
	// Pivot 0, 0
	int Pinball[208] = {
		28, 23,
		19, 34,
		15, 44,
		11, 55,
		10, 69,
		12, 83,
		18, 96,
		41, 122,
		45, 127,
		43, 131,
		33, 147,
		26, 164,
		20, 180,
		15, 198,
		11, 217,
		10, 235,
		10, 251,
		12, 271,
		14, 286,
		18, 301,
		24, 318,
		33, 339,
		23, 346,
		15, 355,
		8, 363,
		4, 372,
		3, 533,
		11, 546,
		28, 562,
		18, 575,
		13, 586,
		9, 595,
		8, 605,
		8, 617,
		8, 683,
		45, 705,
		131, 764,
		132, 784,
		131, 812,
		354, 811,
		354, 764,
		398, 735,
		398, 714,
		402, 707,
		441, 679,
		441, 522,
		430, 504,
		417, 488,
		402, 475,
		381, 464,
		381, 396,
		398, 365,
		411, 330,
		427, 287,
		431, 258,
		432, 221,
		429, 195,
		415, 159,
		406, 144,
		385, 115,
		372, 102,
		350, 85,
		325, 68,
		333, 64,
		348, 70,
		360, 77,
		371, 83,
		380, 90,
		391, 100,
		405, 113,
		415, 126,
		425, 142,
		437, 168,
		445, 194,
		449, 215,
		450, 227,
		450, 742,
		480, 742,
		477, 202,
		471, 170,
		462, 146,
		451, 120,
		438, 102,
		423, 85,
		406, 70,
		386, 56,
		371, 47,
		354, 40,
		302, 23,
		273, 18,
		244, 17,
		217, 19,
		194, 23,
		175, 28,
		158, 34,
		136, 42,
		124, 31,
		114, 22,
		102, 15,
		87, 10,
		74, 8,
		63, 8,
		51, 10,
		40, 14
	};

	App->scene_intro->pinball_boundaries = CreateChain(0, 0, Pinball, 208, b2_staticBody);

	// Pivot 0, 0
	int Pinballb_1[60] = {
		328, 286,
		322, 296,
		328, 303,
		337, 304,
		350, 298,
		366, 284,
		381, 270,
		391, 246,
		396, 218,
		392, 189,
		382, 168,
		366, 144,
		351, 127,
		335, 114,
		320, 102,
		298, 85,
		297, 120,
		306, 128,
		312, 124,
		326, 134,
		343, 151,
		352, 162,
		361, 177,
		367, 191,
		372, 212,
		372, 232,
		369, 245,
		361, 257,
		351, 270,
		337, 281
	};

	App->scene_intro->pinballb_1 = CreateChain(0, 0, Pinballb_1, 60, b2_staticBody);

	// Pivot 0, 0
	int Pinballb_2[32] = {
		48, 367,
		43, 375,
		43, 390,
		48, 399,
		66, 408,
		80, 417,
		88, 427,
		91, 438,
		94, 550,
		99, 550,
		95, 437,
		83, 415,
		50, 396,
		47, 389,
		47, 377,
		50, 372
	};

	App->scene_intro->pinballb_2 = CreateChain(0, 0, Pinballb_2, 32, b2_staticBody);	
}

void ModulePhysics::CreateBouncers()
{
	int TriangleBouncer1[24] = {
		120, 567,
		162, 654,
		164, 660,
		162, 663,
		157, 663,
		114, 639,
		110, 636,
		109, 631,
		109, 567,
		111, 562,
		116, 560,
		118, 563
	};

	App->scene_intro->triangleBouncer1 = CreateChain(0, 0, TriangleBouncer1, 24, b2_staticBody, 4);

	int TriangleBouncer2[18] = {
		367, 563,
		371, 570,
		371, 632,
		369, 638,
		364, 641,
		323, 664,
		317, 664,
		317, 658,
		361, 566
	};

	App->scene_intro->triangleBouncer2 = CreateChain(0, 0, TriangleBouncer2, 18, b2_staticBody, 4);

}
// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	b2Vec2 mouse_position;
	mouse_position.x = PIXEL_TO_METERS(App->input->GetMouseX());
	mouse_position.y = PIXEL_TO_METERS(App->input->GetMouseY());

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
		
			
				if (f->TestPoint(mouse_position) && body_found == nullptr)
				{
					LOG("asdjklasdasdf")
						body_found = b;
		
				
					
				}
					
			}
			
		
			// test if the current body contains mouse position
		}
	}

if (body_found != nullptr && mouse_joint == nullptr)
{
	b2MouseJointDef def;
	def.bodyA = ground;
	def.bodyB = body_found;
	def.target = mouse_position;
	def.dampingRatio = 0.5f;
	def.frequencyHz = 2.0f;
	def.maxForce = 100.0f * body_found->GetMass();
	mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
}
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr)
	{
		mouse_joint->SetTarget(mouse_position);		
		//App->render->DrawLine();

	}
	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
	{

		//App->render->DrawLine();
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		body_found = nullptr;
	}


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	// TODO 4: If the player releases the mouse button, destroy the joint

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}