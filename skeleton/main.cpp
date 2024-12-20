#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Vector3D.h"
#include "Particle.h"
#include "Proyectil.h"
#include <iostream>
#include <list>
#include "ParticlesSystem.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "GravityForceGenerator.h"
#include "ElasticBandForceGenerator.h"
#include "BuoyancyForceGenerator.h"
#include "RigidSolid.h"
#include "RigidSolidSystem.h"
std::string display_text = "This is a test";


using namespace physx;
PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
//escenas
PxScene*				gScene1      = NULL;
PxScene*				gScene2		 = NULL;
PxScene*				gScene3		 = NULL;
PxScene*				gScene4		 = NULL;
PxScene*				currentScene = NULL;
ContactReportCallback gContactReportCallback;
//Definicion de variables globales
std::list<RenderItem*> items;

RenderItem *xRenderItem = NULL, *yRenderItem = NULL, *zRenderItem = NULL, *originRenderItem = NULL;
PxTransform x, y, z, origin;
std::list<Particle*> particles;
std::list<ParticlesSystem*> systems;

ParticlesSystem* system1;
ParticlesSystem* system2;
ParticlesSystem* system3;

ExplosionForceGenerator* explosion_generator;
SpringForceGenerator* spring_generator;

std::list<RigidSolid*> rigidSolids;
std::list<RigidSolidSystem*> solidSystems;

RigidSolidSystem* system1_r;

RigidSolid* player = nullptr;
PxScene* createScene() {
	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	PxScene* newScene = gPhysics->createScene(sceneDesc);
	return newScene;
}
void cleanScreen() {
	for (auto it = items.begin(); it != items.end(); ++it) {
		DeregisterRenderItem(*it);
	}
	items.clear();
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		delete *it;
	}
	particles.clear();
	for (auto it = rigidSolids.begin(); it != rigidSolids.end(); ++it) {
		delete* it;
	}
	rigidSolids.clear();
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	systems.clear();
	for (auto it = solidSystems.begin(); it != solidSystems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	solidSystems.clear();
}
void initScene1() {
	if (currentScene) {
		cleanScreen();
		currentScene->release();
		
	}
	gScene1 = createScene();
	currentScene = gScene1;
	
	//Mostrar el sistema de coordenadas
	Vector3D ejeX(10.0f, 0.0f, 0.0f), ejeY(0.0f, 10.0f, 0.0f), ejeZ(0.0f, 0.0f, 10.0f);
	x = PxTransform(ejeX.getX(), ejeX.getY(), ejeX.getZ());
	y = PxTransform(ejeY.getX(), ejeY.getY(), ejeY.getZ());
	z = PxTransform(ejeZ.getX(), ejeZ.getY(), ejeZ.getZ());
	origin = PxTransform(0.0f, 0.0f, 0.0f);

	xRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &x, { 1.0,0.0,0.0,1.0 });
	yRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &y, { 0.0,1.0,0.0,1.0 });
	zRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &z, { 0.0,0.0,1.0,1.0 });
	originRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &origin, { 1.0,1.0,1.0,1.0 });
	
	items.push_back(xRenderItem);
	items.push_back(yRenderItem);
	items.push_back(zRenderItem);
	items.push_back(originRenderItem);

}
void initScene2() {
	
	if (currentScene) {
		cleanScreen();
		currentScene->release();
		
	}
	gScene2 = createScene();
	currentScene = gScene2;

}
void initScene3() {

	if (currentScene) {
		cleanScreen();
		currentScene->release();

	}
	gScene3 = createScene();
	currentScene = gScene3;

}
void initScene4() {

	if (currentScene) {
		cleanScreen();
		currentScene->release();

	}
	gScene4 = createScene();
	currentScene = gScene4;

	////generador de actores estaticos
	////suelo, definir posicion y forma
	//PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
	//PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	//suelo->attachShape(*shape);
	//gScene4->addActor(*suelo); //aniade el elemeno a la escena

	////pintar suelo
	//RenderItem* item = new RenderItem(shape, suelo, { 0.8,0.8,0.8,1 });


	/*RigidSolid* solid1 = new RigidSolid(gPhysics, gScene4, gMaterial,
		Vector3(-70, 50, -70), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(5, 5, 5), Vector4(0.8, 0.8, 0.8, 1),
		0.65, 500, "BOX");
	rigidSolids.push_back(solid1);
	RigidSolid* solid2 = new RigidSolid(gPhysics, gScene4, gMaterial,
		Vector3(-70, 100, -70), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(5, 5, 5), Vector4(0.8, 0.8, 0.8, 1),
		0.15, 500, "SPHERE");
	rigidSolids.push_back(solid2);*/
}
// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	//Inicializar escena 1 por defecto
	initScene1();
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	if (currentScene) {
		currentScene->simulate(t);
		currentScene->fetchResults(true);
	}
	//myParticle->integrate(t);
	if (currentScene == gScene2 || currentScene == gScene3) {
		
		for (auto it = particles.begin(); it != particles.end();) {
			(*it)->integrate(t);
			(*it)->update(t);
			////comprobar si siguen vivas, si no es asi se elimina
			if (!(*it)->isAlive()) {
				delete (*it);
				it = particles.erase(it);
			}
			else {
				++it;
			}
		}
		for (auto it = systems.begin(); it != systems.end(); ++it) {
			if (*it != nullptr) {
				(*it)->update(t);
			}
		}
		/*if (fog_system) {
			fog_system->update(t);
		}
		if (fire_system) {
			fire_system->update(t);
		}
		if (rain_system) {
			rain_system->update(t);
		}
		if (smoke_system) {
			smoke_system->update(t);
		}
		if (rain_gravity_system) {
			rain_gravity_system->update(t);
		}
		if (wind_system) {
			wind_system->update(t);
		}
		if (whirlwind_system) {
			whirlwind_system->update(t);
		}
		if (system1) {
			system1->update(t);
		}
		if (system2) {
			system2->update(t);
		}
		if (system3) {
			system3->update(t);
		}*/
	}

	if (currentScene == gScene4) {
		for (auto it = rigidSolids.begin(); it != rigidSolids.end();) {
			(*it)->integrate(t);
			(*it)->update(t);
			if (!(*it)->isAlive()) {
				delete (*it);
				it = rigidSolids.erase(it);
			}
			else {
				++it;
			}
		}
		for (auto it = solidSystems.begin(); it != solidSystems.end(); ++it) {
			if (*it != nullptr) {
				(*it)->update(t);
			}
		}
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	for (auto it = items.begin(); it != items.end(); ++it) {
		DeregisterRenderItem(*it);
	}
	items.clear();
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		delete* it;
	}
	particles.clear();
	for (auto it = rigidSolids.begin(); it != rigidSolids.end(); ++it) {
		delete* it;
	}
	rigidSolids.clear();
	for (auto it = solidSystems.begin(); it != solidSystems.end(); ++it) {
		delete* it;
	}
	solidSystems.clear();
	for (auto it = systems.begin(); it != systems.end(); ++it) {
		if (*it != nullptr) {
			delete* it;
		}
	}
	systems.clear();
	/*if (fog_system) {
		delete fog_system;
	}
	if (fire_system) {
		delete fire_system;
	}
	if (rain_system) {
		delete rain_system;
	}
	if (smoke_system) {
		delete smoke_system;
	}
	if (rain_gravity_system) {
		delete rain_gravity_system;
	}
	if (wind_system) {
		delete wind_system;
	}
	if (whirlwind_system) {
		delete whirlwind_system;
	}
	if (explosion_generator) {
		delete explosion_generator;
	}
	if (system1) {
		delete system1;
	}
	if (system2) {
		delete system2;
	}
	if (system3) {
		delete system3;
	}*/
	/*delete myParticle;*/
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	currentScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
}
// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case 'Z':
	{
		if (currentScene == gScene4) {
			if (player) {
				Vector3 pos = player->getPosition();
				pos.x += 1.0f;
				player->setPosition(pos);
			}
		}
		break;
	}
	case 'P':
	{
		if (currentScene == gScene2) {
			Camera* cam = GetCamera();
			if (cam != nullptr) {
				Vector3 pos = cam->getEye();
				Vector3 dir = cam->getDir();
				/*Particle* newParticle = new Particle(Vector3(0.0f, 50.0f, 0.0f), Vector3(500.0f, 0.0f, 0.0f), Vector3(9.8f, 0.0f, 0.0f), 0.005);
			*/
				float speed = 25.0f;

				physx::PxVec3 vel = dir * speed;
				Proyectil* newProyectil = new Proyectil
				(pos, vel, Vector3(0.0f, -1.0f, 0.0f), 1.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f), 60, 0.0f);
				particles.push_back(newProyectil);

			}
			
		}
		break;
	}
	case 'L':
	{
		if (currentScene == gScene2) {
			Camera* cam = GetCamera();
			if (cam != nullptr) {
				Vector3 pos = cam->getEye();
				Vector3 dir = cam->getDir();

				float speed = 150.0f;

				physx::PxVec3 vel = dir * speed;
				Proyectil* newProyectil = new Proyectil
				(pos, vel, Vector3(0.0f, -1.0f, 0.0f), 1.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f), 60, 0.0f);
				particles.push_back(newProyectil);

			}
			
		}
		else if (currentScene == gScene3) {
			if (spring_generator) {
				spring_generator->changeK(-5.0);
			}
		}
		break;
	}
	case 'N':
	{
		if (currentScene == gScene2) {
			ParticlesSystem* fog_system = new ParticlesSystem(Vector4(1.0, 1.0, 1.0, 0.1), Vector3(0, 30.0, 0), Vector3(0.1, 0.2, 0.1),150, 0.05f, 0.9f, 0.0f,0.0f);
			fog_system->set_u_Distribution(false);

			fog_system->setNormalDistribPos(1.0, 1.5);
			fog_system->setNormalDistribVel(0.0, 0.1);
			fog_system->setNormalDistribLifeTime(5.0, 5.0);

			systems.push_back(fog_system);
		}
		if (currentScene == gScene3) {

			

			Particle* p1 = new Particle(Vector3(-10.0, 10.0, 0.0), Vector3(0.0, 0.0, 0.0), 1.0f, Vector3(0.0,0.0,0.0),Vector4(1.0, 0.0, 0.0, 1.0), 60, 1.0, "SPHERE");

			Particle* p2 = new Particle(Vector3(10.0, 10.0, 0.0), Vector3(0.0, 0.0, 0.0), 1.0f, Vector3(0.0, 0.0, 0.0), Vector4(0.0, 0.0, 1.0, 1.0), 60, 2.0f, "SPHERE");

			/*SpringForceGenerator* f1 = new SpringForceGenerator(1, 10, p2);
			SpringForceGenerator* f2 = new SpringForceGenerator(1, 10, p1);*/

			ElasticBandForceGenerator* f1 = new ElasticBandForceGenerator(1, 10, p2);
			ElasticBandForceGenerator* f2 = new ElasticBandForceGenerator(1, 10, p1);

			//GravityForceGenerator* g = new GravityForceGenerator(Vector3(0.0, -9.8, 0.0));

			/*p1->addForceGenerator(g);
			p2->addForceGenerator(g);*/

			p1->addForceGenerator(f1);
			p2->addForceGenerator(f2);

			particles.push_back(p1);
			particles.push_back(p2);
		}
		if (currentScene == gScene4) {
			
	
			PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
			PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
			suelo->attachShape(*shape);
			gScene4->addActor(*suelo); 
			RenderItem* item = new RenderItem(shape, suelo, { 0.1,0.5,0.1,1 });

			PxRigidStatic* tronco = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
			PxShape* shape2 = CreateShape(PxBoxGeometry(5, 20, 5));
			tronco->attachShape(*shape2);
			gScene4->addActor(*tronco);
			RenderItem* item2 = new RenderItem(shape2, tronco, { 0.6,0.1,0.1,1 });

			PxRigidStatic* esfera = gPhysics->createRigidStatic(PxTransform({ 0,30,0 }));
			PxShape* shape3 = CreateShape(PxSphereGeometry(10));
			esfera->attachShape(*shape3);
			gScene4->addActor(*esfera);
			RenderItem* item3 = new RenderItem(shape3, esfera, { 1.0,0.0,0.0,1 });

			RigidSolidSystem* snow_solid_system = new RigidSolidSystem(gPhysics, gScene4,
				Vector3(0.0f, 50.0f, 0.0f), Vector3(0.0f, -9.8f, 0.0f), Vector3(0, 0, 0),
				Vector3(0.5f, 0.5f, 0.5f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
				5, 0.7f, 1.0f, "SPHERE");

			snow_solid_system->set_u_Distribution(false); //usar distribucion  normal
			snow_solid_system->setNormalDistribPos(0.0, 2.0);
			snow_solid_system->setNormalDistribLinealVel(0.0, 5.0);
			snow_solid_system->setNormalDistribAngularVel(0.0, 1.0);
			snow_solid_system->setNormalDistribLifeTime(10.0, 2.0);
			
			snow_solid_system->setMaterial(0.1f, 0.1f, 0.01f);
			solidSystems.push_back(snow_solid_system);
		}
		break;
	}
	case 'F':
	{
		if (currentScene == gScene2) {
			ParticlesSystem* fire_system = new ParticlesSystem(Vector4(1.0, 0.5, 0.0, 1.0), Vector3(-15.0, 50.0, 0), Vector3(0, 0, 0), 20, 1.5f, 0.8f, 0.0f,0.0f);
			fire_system->set_u_Distribution(false);

			fire_system->setNormalDistribPos(1.0, 0.5);
			fire_system->setNormalDistribVel(1.0, 0.5);
			fire_system->setNormalDistribLifeTime(1.0, 0.5);

			systems.push_back(fire_system);
		}
		break;
	}
	case 'R':
	{
		if (currentScene == gScene2) {
			ParticlesSystem* rain_system = new ParticlesSystem(Vector4(0.0, 0.0, 1.0, 0.5), Vector3(15.0, 50.0, 0), Vector3(0.0, 1.0, 0.0), 100, 0.05f, 0.9f, -10.0f, 0.0f);
			rain_system->set_u_Distribution(true);

			rain_system->setUniformDistribPos(1.0, 10.0);
			rain_system->setUniformDistribVel(1.0, 3.0);
			rain_system->setNormalDistribLifeTime(1.0,5.0);

			systems.push_back(rain_system);
		}
		if (currentScene == gScene3) {
			Particle* waterPlane = new Particle(Vector3(10.0, 40.0, -20.0), Vector3(0.0,0.0,0.0),0.0, Vector3(20.0, 0.2, 10.0), Vector4(0.0, 0.0, 1.0, 0.0), 0.0f, 0.0f, "BOX");
			Particle* floor = new Particle(Vector3(10.0, 20.0, -20.0), Vector3(0.0, 0.0, 0.0), 0.0, Vector3(20.0, 0.2, 10.0), Vector4(0.5, 0.5, 0.5, 0.0), 0.0f, 0.0f, "BOX");
			Particle* floatingParticle1 = new Particle(Vector3(0.0, 40.0, -20.0), Vector3(0.0, 0.0, 0.0), 1.0f, Vector3(1.0, 1.0, 1.0), Vector4(1.0, 0.5, 0.0, 1.0), 60.0f, 10000.0f, "BOX");
			Particle* floatingParticle2 = new Particle(Vector3(10.0, 40.0, -20.0), Vector3(0.0, 0.0, 0.0), 1.0f, Vector3(1.0, 1.0, 1.0), Vector4(1.0, 0.5, 0.0, 1.0), 60.0f, 500.0f, "BOX");
			Particle* floatingParticle3 = new Particle(Vector3(20.0, 40.0, -20.0), Vector3(0.0, 0.0, 0.0), 1.0f, Vector3(1.0, 1.0, 1.0), Vector4(1.0, 0.5, 0.0, 1.0), 60.0f, 8000.0f, "BOX");
			BuoyancyForceGenerator* buoyancy_generator = new BuoyancyForceGenerator(1000.0f);
			GravityForceGenerator* g = new GravityForceGenerator(Vector3(0.0, -9.8, 0.0));
			buoyancy_generator->setliquid_particle(waterPlane);

			floatingParticle1->addForceGenerator(buoyancy_generator);
			floatingParticle1->addForceGenerator(g);

			floatingParticle2->addForceGenerator(buoyancy_generator);
			floatingParticle2->addForceGenerator(g);

			floatingParticle3->addForceGenerator(buoyancy_generator);
			floatingParticle3->addForceGenerator(g);


			//particles.push_back(waterPlane);
			particles.push_back(floatingParticle1);
			particles.push_back(floatingParticle2);
			particles.push_back(floatingParticle3);
			
			
			

		}
		break;
	}
	case 'H':
	{
		if (currentScene == gScene2) {
			ParticlesSystem* smoke_system = new ParticlesSystem(Vector4(0.6, 0.6, 0.6, 0.5), Vector3(0, 0, 0), Vector3(0.0, 5.0, 0.0), 1, 1.0f, 0.1f, 10.0f, 0.0f);
			smoke_system->set_u_Distribution(false);

			smoke_system->setNormalDistribPos(1.0, 0.5);
			smoke_system->setNormalDistribVel(5.0, 2.0);
			smoke_system->setNormalDistribLifeTime(1.0, 0.5);

			systems.push_back(smoke_system);
		}
		break;
	}
	case 'G':
	{
		if (currentScene == gScene2) {

			/*rain_gravity_system = new ParticlesSystem(Vector4(0.0, 0.0, 1.0, 0.5), Vector3(0.0, 50.0, 0), Vector3(0.0, 0.0, 0.0), 100, 0.05f, 0.9f, 0.0f, 0.1f);
			
			rain_gravity_system->set_u_Distribution(true);
			rain_gravity_system->setGravityForce();

			rain_gravity_system->setUniformDistribPos(1.0, 10.0);
			rain_gravity_system->setUniformDistribVel(0.0, 0.0);
			rain_gravity_system->setNormalDistribLifeTime(1.0, 5.0);*/
			
			Particle* p1 = new Particle(Vector3(-10.0f, 70.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 2.0f, Vector3(0.0, 0.0, 0.0), Vector4(1.0f, 0.0f, 0.0f, 1.0f), 50, 2.0f, "SPHERE");
			Particle* p2 = new Particle(Vector3(0.0f, 70.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, Vector3(0.0, 0.0, 0.0), Vector4(0.0f, 0.0f, 1.0f, 1.0f), 50, 0.1f, "SPHERE");

			GravityForceGenerator* g = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));

			p1->addForceGenerator(g);
			p2->addForceGenerator(g);

			particles.push_back(p1);
			particles.push_back(p2);
		}
		if (currentScene == gScene4) {

			RigidSolid* solid1 = new RigidSolid(gPhysics, gScene4, gMaterial,
				Vector3(0, 100, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), Vector4(0.8, 0.8, 0.8, 1),
				0.65, 500, "SPHERE");

			GravityForceGenerator* g = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));
			solid1->addForceGenerator(g);

			rigidSolids.push_back(solid1);
			

		}
		break;
	}
	case 'V':
	{
		if (currentScene == gScene2) {

			ParticlesSystem* wind_system = new ParticlesSystem(Vector4(1.0, 0.0, 1.0, 1.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0),
				1, 1.0f, 0.1f, 0.0f, 1.0f);

			wind_system->set_u_Distribution(true);
			wind_system->setGravityForce();
			wind_system->setWindForce(Vector3(0.0f, 10.0f, 0.0f), 10.0f);

			/*wind_system->setNormalDistribPos(5.0, 2.0);
			wind_system->setNormalDistribVel(3.0, 1.0);*/
			wind_system->setUniformDistribPos(0.0, 10.0);
			wind_system->setUniformDistribVel(0.0, 10.0);
			wind_system->setNormalDistribLifeTime(10.0, 2.0);

			systems.push_back(wind_system);
		}
		if (currentScene == gScene4) {

			RigidSolidSystem* wind_solid_system = new RigidSolidSystem(gPhysics, gScene4,
				Vector3(0, 50, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), Vector4(0, 0, 1, 1),
				5, 0.7f, 1.0f, "BOX");


			wind_solid_system->set_u_Distribution(true);
			wind_solid_system->setUniformDistribLinearVel(5.0,15.0);
			wind_solid_system->setUniformDistribAngularVel(0.0, 10.0);
			wind_solid_system->setUniformDistribPos(-10.0, 10.0);
			wind_solid_system->setNormalDistribLifeTime(10.0, 2.0);

			wind_solid_system->setGravityForce();
			wind_solid_system->setWindForce(Vector3(0,10,0), 0.1f);
			solidSystems.push_back(wind_solid_system);
		}
		break;
	}
	case 'T':
	{
		if (currentScene == gScene2) {

			ParticlesSystem* whirlwind_system = new ParticlesSystem(Vector4(1.0, 0.0, 1.0, 1.0), Vector3(1.0, 0.0, 1.0), Vector3(0.0, 0.0, 0.0), 1, 1.0f, 0.5f, 0.0f, 0.5f);

			whirlwind_system->set_u_Distribution(false);
			whirlwind_system->setGravityForce();
			whirlwind_system->setWhirlWindForce(20.0f,0.5f);

			whirlwind_system->setNormalDistribPos(5.0, 2.0);
			whirlwind_system->setNormalDistribVel(3.0, 1.0);
			whirlwind_system->setNormalDistribLifeTime(5.0, 2.0);

			systems.push_back(whirlwind_system);
		
		}
		if (currentScene == gScene4) {
			RigidSolidSystem* whirlwind_solid_system = new RigidSolidSystem(gPhysics, gScene4,
				Vector3(1, 0, 1), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), Vector4(0, 0, 1, 1),
				1, 0.5f, 0.5f, "BOX");


			whirlwind_solid_system->set_u_Distribution(false);
			whirlwind_solid_system->setGravityForce();
			whirlwind_solid_system->setNormalDistribPos(5.0, 2.0);
			whirlwind_solid_system->setNormalDistribLinealVel(3.0, 1.0);
			whirlwind_solid_system->setNormalDistribAngularVel(0.0, 0.01);
			whirlwind_solid_system->setNormalDistribLifeTime(10.0, 2.0);
			whirlwind_solid_system->setWhirlWindForce(20.0f, 0.5f);
			solidSystems.push_back(whirlwind_solid_system);
		}
		break;
	}
	case 'E':
	{
		if (currentScene == gScene2) {

			/*Particle* p1 = new Particle(Vector3(0.0f, 30.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f), 20, 1.0f);
			Particle* p2 = new Particle(Vector3(5.0f, 30.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f), 20, 5.0f);
			Particle* p3 = new Particle(Vector3(-5.0f, 30.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, Vector4(0.0f, 0.0f, 1.0f, 1.0f), 20, 0.1f);

			particles.push_back(p1);
			particles.push_back(p2);
			particles.push_back(p3);*/
			system1 = new ParticlesSystem(Vector4(1.0, 0.0, 0.0, 1.0), Vector3(0.0f, 30.0f, 0.0f), Vector3(0.0, 0.0, 0.0), 1, 1.0f, 0.2f, 0.0f, 1.0f);
			system2 = new ParticlesSystem(Vector4(0.0, 1.0, 0.0, 1.0), Vector3(5.0f, 30.0f, 0.0f), Vector3(0.0, 0.0, 0.0), 1, 1.0f, 0.2f, 0.0f, 5.0f);
			system3 = new ParticlesSystem(Vector4(0.0, 0.0, 1.0, 1.0), Vector3(-5.0f, 30.0f, 0.0f), Vector3(0.0, 0.0, 0.0), 1, 1.0f, 0.2f, 0.0f, 0.1f);

			system1->set_u_Distribution(true);

			system1->setUniformDistribPos(-5.0, 5.0);
			system1->setUniformDistribVel(0.0, 0.0);
			system1->setNormalDistribLifeTime(10.0, 5.0);

			system2->set_u_Distribution(true);

			system2->setUniformDistribPos(-5.0, 5.0);
			system2->setUniformDistribVel(0.0, 0.0);
			system2->setNormalDistribLifeTime(10.0, 5.0);

			system3->set_u_Distribution(true);

			system3->setUniformDistribPos(-5.0, 5.0);
			system3->setUniformDistribVel(0.0, 0.0);
			system3->setNormalDistribLifeTime(10.0, 5.0);

			systems.push_back(system1);
			systems.push_back(system2);
			systems.push_back(system3);

		}
		if (currentScene == gScene4) {
			system1_r = new RigidSolidSystem(gPhysics, gScene4,
				Vector3(0.0f, 30.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0, 0, 0),
				Vector3(1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
				1, 0.2f, 1.0f, "SPHERE");

			system1_r->set_u_Distribution(true);
			system1_r->setUniformDistribPos(-5.0, 5.0);
			system1_r->setUniformDistribLinearVel(0.0, 0.0);
			system1_r->setUniformDistribAngularVel(0.0, 0.0);
			system1_r->setNormalDistribLifeTime(10.0, 5.0);

			system1_r->setMaterial(0.9f, 0.9f, 0.8f);
			solidSystems.push_back(system1_r);
		}
		break;
	}
	case 'X':
	{
		if (currentScene == gScene2 || currentScene == gScene4) {
			/*explosion_generator = new ExplosionForceGenerator(200.0f, 2000.f, 1.0f, Vector3(0.0f,0.0f, 0.0f));

			for (auto p : particles) {
				p->addForceGenerator(explosion_generator);
			}*/
			explosion_generator = new ExplosionForceGenerator(50.0f, 2000.0, 0.5f, Vector3(0.0f, 30.0f, 0.0f));

			for (auto p : particles) {
				p->addForceGenerator(explosion_generator);
			}
			if (system1 && system2 && system3) {
				system1->addForceGenerator(explosion_generator);
				system2->addForceGenerator(explosion_generator);
				system3->addForceGenerator(explosion_generator);
			}
			if (system1_r) {
				system1_r->addForceGenerator(explosion_generator);
			}

		}
		break;
	}
	case 'M':
	{
		if (currentScene == gScene3) {
			
			//el punto de anclaje es un cubo
			const Vector3 anchorPoint(0.0, 20.0, 0.0);
			RenderItem* cube = new RenderItem(CreateShape(physx::PxBoxGeometry(2, 2, 2)),
				new physx::PxTransform(anchorPoint), Vector4(0, 0, 1, 1));

			Particle* p1 = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(0.0, 0.0, 0.0), 1.0f, Vector3(0.0, 0.0, 0.0), Vector4(1.0, 0.0, 0.0, 1.0), 60, 1.0f, "SPHERE");

			//Particle* p2 = new Particle(Vector3(10.0, 10.0, 0.0), Vector3(0.0, 0.0, 0.0), 1.0f, Vector4(0.0, 0.0, 1.0, 1.0), 60, 2.0f);

			//SpringForceGenerator* f1 = new SpringForceGenerator(1, 10, p2);

			GravityForceGenerator* g = new GravityForceGenerator(Vector3(0.0, -9.8, 0.0));
			spring_generator = new SpringForceGenerator(50.0, 5.0, anchorPoint);

			p1->addForceGenerator(g);
			p1->addForceGenerator(spring_generator);
			//p2->addForceGenerator(f2);

			particles.push_back(p1);
			//particles.push_back(p2);
		}
		if (currentScene == gScene4) {

			//el punto de anclaje es un cubo
			const Vector3 anchorPoint(0.0, 20.0, 0.0);
			RenderItem* cube = new RenderItem(CreateShape(physx::PxBoxGeometry(2, 2, 2)),
				new physx::PxTransform(anchorPoint), Vector4(0, 0, 1, 1));

			RigidSolid* s = new RigidSolid(gPhysics, gScene4, gMaterial,
				Vector3(0, 20,10), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1), Vector4(0.0, 0.0, 1, 1),
				0.3, 500, "SPHERE");

			rigidSolids.push_back(s);

			GravityForceGenerator* g = new GravityForceGenerator(Vector3(0.0, -9.8, 0.0));
			spring_generator = new SpringForceGenerator(200.0, 1.0, anchorPoint);

			s->addForceGenerator(g);
			s->addForceGenerator(spring_generator);
			s->setInertia(Vector3(1, 1, 1));

			s->getSolid()->setRigidDynamicLockFlags(
				PxRigidDynamicLockFlag::eLOCK_LINEAR_X |
				PxRigidDynamicLockFlag::eLOCK_LINEAR_Y
			);

			player = new RigidSolid(gPhysics, gScene4, gMaterial,
				Vector3(-5, 20, 8), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), Vector4(1.0, 0.0,0.0, 1),
				0.3, 500, "BOX");

			player->setMaterialProperties(0.0f, 0.5f, 0.4f);
			player->setInertia(Vector3(2, 2, 2));
			rigidSolids.push_back(player);
			

			PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,18,0 }));
			PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
			suelo->attachShape(*shape);
			gScene4->addActor(*suelo); //aniade el elemeno a la escena
			RenderItem* item = new RenderItem(shape, suelo, { 0.8,0.8,0.8,1 });
			
		}
		break;
	}
	case 'K':
	{
		if (currentScene == gScene3) {
			if (spring_generator) {
				spring_generator->changeK(10.0);
			}
		}
		break;
	}
	case 'C':
	{
		if (currentScene == gScene4) {
			RigidSolid* cylinder = new RigidSolid(gPhysics, gScene4, gMaterial,
				Vector3(0, 20, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(5,10, 5), Vector4(1, 0, 0, 1),
				1.0f, 500, "CYLINDER");
			rigidSolids.push_back(cylinder);

			PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
			PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
			suelo->attachShape(*shape);
			gScene4->addActor(*suelo); 
			RenderItem* item = new RenderItem(shape, suelo, { 0,0,1,1 });
		}
		break;
	}
	case '1':
	{
		initScene1();
		break;
	}
	case '2':
	{
		initScene2();
		break;
	}
	case '3':
	{
		initScene3();
		break;
	}
	case '4':
	{
		initScene4();
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}