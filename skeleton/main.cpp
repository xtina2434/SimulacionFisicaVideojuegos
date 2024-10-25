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
PxScene*				currentScene = NULL;
ContactReportCallback gContactReportCallback;
//Definicion de variables globales
std::list<RenderItem*> items;

RenderItem *xRenderItem = NULL, *yRenderItem = NULL, *zRenderItem = NULL, *originRenderItem = NULL;
PxTransform x, y, z, origin;
std::list<Particle*> particles;
ParticlesSystem* fog_system;
ParticlesSystem* fire_system;
ParticlesSystem* rain_system;
ParticlesSystem* smoke_system;

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
	if (currentScene == gScene2) {
		
		for (auto it = particles.begin(); it != particles.end();) {
			(*it)->integrate(t);
			////comprobar si siguen vivas, si no es asi se elimina
			if (!(*it)->isAlive()) {
				delete (*it);
				it = particles.erase(it);
			}
			else {
				++it;
			}
		}
		if (fog_system) {
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
	if (fog_system) {
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
				(pos, vel, Vector3(0.0f, -1.0f, 0.0f), 1, Vector4(0.0f, 0.0f, 0.0f, 1.0f), 60, 2);
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
				(pos, vel, Vector3(0.0f, -1.0f, 0.0f), 1, Vector4(0.0f, 0.0f, 0.0f, 1.0f), 60, 2);
				particles.push_back(newProyectil);

			}
			
		}
		break;
	}
	case 'N':
	{
		if (currentScene == gScene2) {
			fog_system = new ParticlesSystem(Vector4(1.0, 1.0, 1.0, 0.1), Vector3(0, 30.0, 0), Vector3(0.1, 0.2, 0.1),150, 0.05f, 0.9f, 0.0f);
			fog_system->set_u_Distribution(false);

			fog_system->setNormalDistribPos(1.0, 1.5);
			fog_system->setNormalDistribVel(0.0, 0.1);
			fog_system->setNormalDistribLifeTime(5.0, 5.0);
		}
		break;
	}
	case 'F':
	{
		if (currentScene == gScene2) {
			fire_system = new ParticlesSystem(Vector4(1.0, 0.5, 0.0, 1.0), Vector3(-15.0, 50.0, 0), Vector3(0, 0, 0), 20, 1.5f, 0.8f, 0.0f);
			fire_system->set_u_Distribution(false);

			fire_system->setNormalDistribPos(1.0, 0.5);
			fire_system->setNormalDistribVel(1.0, 0.5);
			fire_system->setNormalDistribLifeTime(1.0, 0.5);
		}
		break;
	}
	case 'R':
	{
		if (currentScene == gScene2) {
			rain_system = new ParticlesSystem(Vector4(0.0, 0.0, 1.0, 0.5), Vector3(15.0, 50.0, 0), Vector3(0.0, 1.0, 0.0), 100, 0.05f, 0.9f, -10.0f);
			rain_system->set_u_Distribution(true);

			rain_system->setUniformDistribPos(1.0, 10.0);
			rain_system->setUniformDistribVel(1.0, 3.0);
			rain_system->setNormalDistribLifeTime(1.0,5.0);
		}
		break;
	}
	case 'H':
	{
		if (currentScene == gScene2) {
			smoke_system = new ParticlesSystem(Vector4(0.6, 0.6, 0.6, 0.5), Vector3(0, 0, 0), Vector3(0.0, 5.0, 0.0), 1, 1.0f, 0.1f, 10.0f);
			smoke_system->set_u_Distribution(false);

			smoke_system->setNormalDistribPos(1.0, 0.5);
			smoke_system->setNormalDistribVel(5.0, 2.0);
			smoke_system->setNormalDistribLifeTime(1.0, 0.5);
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