#include "Particle.h"
#include <cmath>
#include "ForceGenerator.h"
Particle::Particle(Vector3 Pos, Vector3 Vel,/* Vector3 Acel,*/ float Size, Vector3 Vol, Vector4 Color, double LifeTime, float Mass, std::string SHAPE) : 
	pose(Pos), vel(Vel),/* acel(Acel),*/ size(Size),vol(Vol),color(Color), life_time(LifeTime),mass(Mass), mt(rd()) {
	acel = Vector3(0.0f, 0.0f, 0.0f);
	accF = Vector3(0.0f, 0.0f, 0.f);

	if(SHAPE == "SPHERE")
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, color);

	if (SHAPE == "BOX") {
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(vol)), &pose, color);
	}
}
Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}
void Particle::setRandomMass(double mean, double desv)
{
	std::normal_distribution<double> normal_dist(mean, desv);

	double new_mass = normal_dist(mt);

	if (new_mass < 0)
		new_mass = 1.0f;

	mass = new_mass;
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
	
	if(mass > 0.0f && accF != Vector3(0,0,0))
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