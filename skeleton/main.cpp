#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Vector3D.h"
#include "Particle.h"
#include <iostream>


std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;
//Particle* myParticle = NULL;
//Definicion de variables globales
std::vector<RenderItem*> items;
RenderItem *xRenderItem = NULL, *yRenderItem = NULL, *zRenderItem = NULL, *originRenderItem = NULL;
PxTransform x, y, z, origin;
std::vector<Particle*> particles;
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

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	////Mostrar el sistema de coordenadas
	//Vector3D ejeX(10.0f, 0.0f, 0.0f), ejeY(0.0f, 10.0f, 0.0f), ejeZ(0.0f, 0.0f, 10.0f);
	//x = PxTransform(ejeX.getX(), ejeX.getY(), ejeX.getZ());
	//y = PxTransform(ejeY.getX(), ejeY.getY(), ejeY.getZ());
	//z = PxTransform(ejeZ.getX(), ejeZ.getY(), ejeZ.getZ());
	//origin = PxTransform(0.0f, 0.0f, 0.0f);

	//xRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &x, { 1.0,0.0,0.0,1.0 });
	//yRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &y, { 0.0,1.0,0.0,1.0 });
	//zRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &z, { 0.0,0.0,1.0,1.0 });
	//originRenderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &origin, { 1.0,1.0,1.0,1.0 });
	//items.push_back(xRenderItem);
	//items.push_back(yRenderItem);
	//items.push_back(zRenderItem);
	//items.push_back(originRenderItem);

	//Particula practica 1
	//myParticle = new Particle(Vector3(-30.0f, 50.0f, 0.0f), Vector3(3.0f, 0.0f, 0.0f), Vector3(5.0f, 0.0f, 0.0f));

	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	//myParticle->integrate(t);
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->integrate(t);
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//liberar render items
	/*DeregisterRenderItem(xRenderItem);
	DeregisterRenderItem(yRenderItem);
	DeregisterRenderItem(zRenderItem);
	DeregisterRenderItem(originRenderItem);*/
	for (int i = 0; i < items.size();i++) {
		DeregisterRenderItem(items[i]);
	}
	for (int i = 0; i < particles.size();i++) {
		delete particles[i];
	}
	/*delete myParticle;*/
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
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
		Particle* newParticle = new Particle(Vector3(0.0f, 50.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, -9.8f, 0.0f));
		particles.push_back(newParticle);
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