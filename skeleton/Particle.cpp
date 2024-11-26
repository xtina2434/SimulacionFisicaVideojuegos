#include "Particle.h"
#include <cmath>
#include "ForceGenerator.h"
Particle::Particle(Vector3 Pos, Vector3 Vel,/* Vector3 Acel,*/ float Size, Vector4 Color, double LifeTime, float Mass) : 
	pose(Pos), vel(Vel),/* acel(Acel),*/ size(Size),color(Color), life_time(LifeTime),mass(Mass) {
	acel = Vector3(0.0f, 0.0f, 0.0f);
	accF = Vector3(0.0f, 0.0f, 0.f);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, color);
}
Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}
void Particle::addForce(Vector3 f) {
	accF += f;
}
void Particle::addForceGenerator(ForceGenerator* fg) {
	generators.push_back(fg);
}
void Particle::clearForces() {
	accF = Vector3(0.0f, 0.0f, 0.0f);
}
void Particle::integrate(double t) {
	
	

	if(mass > 0.0f)
		acel = accF / mass;

	//Euler
	/*pose.p = pose.p + t * vel;
	vel = vel + t * acel;
	vel = vel * pow(doumping, t);*/

	//Euler sempli-implicito
	vel = vel +( t * acel);
	pose.p = pose.p + (vel * t);
	vel = vel * pow(doumping, t);

	clearForces();
}
void Particle::update(double t) {

	for (auto fg : generators) {
		if (fg != nullptr) {
			fg->updateForce(this, t);
		}	
	}
	life_time -= t;
	
}