#include "Proyectil.h"
Proyectil::Proyectil(Vector3 Pos, Vector3 Vel, Vector3 Acel, int Size, Vector4 Color, int LifeTime, int Masa) :
	Particle(Pos, Vel, Acel,Size, Color, LifeTime), masa(Masa) {


}
Proyectil::~Proyectil() {

}

//void Proyectil::integrate(double t)
//{
//	vel = vel + t * acel;
//	pose.p = pose.p + vel * t;
//	vel = vel * pow(doumping, t);
//
//
//	//vel = vel - acel * t;
//	//pose.p = pose.p + vel * t - (0, 5 * acel * pow(t, 2));
//	//vel = vel * pow(doumping, t);
//}
