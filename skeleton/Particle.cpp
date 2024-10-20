#include "Particle.h"
#include <cmath>
Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acel, float Size, Vector4 Color, double LifeTime) : pose(Pos), vel(Vel), acel(Acel), size(Size),color(Color), life_time(LifeTime){
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), &pose, color);
}
Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}
void Particle::integrate(double t) {
	
	//Euler
	/*pose.p = pose.p + t * vel;
	vel = vel + t * acel;
	vel = vel * pow(doumping, t);*/

	
	//Euler sempli-implicito
	vel = vel +( t * acel);
	pose.p = pose.p + (vel * t);
	vel = vel * pow(doumping, t);

	life_time -= t;
}