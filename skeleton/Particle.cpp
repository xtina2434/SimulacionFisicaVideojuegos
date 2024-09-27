#include "Particle.h"
#include <cmath>
Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 Acel) : pose(Pos), vel(Vel), acel(Acel){
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, { 0.0,0.0,0.0,1.0 });
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
	vel = vel + t * acel;
	pose.p = pose.p + vel * t;
	vel = vel * pow(doumping, t);
}