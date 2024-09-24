#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
//Clase Vector3D

class Vector3D {
	private:
		float x, y, z;
	public:
		//Constructor vacio
		Vector3D() noexcept : x(0.0f), y(0.0f), z(0.0f) {}

		//Constructor
		Vector3D(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}

		//Destructora
		~Vector3D() {}

		//Getters
		inline float getX() const noexcept { return x; }
		inline float getY() const noexcept { return y; }
		inline float getZ() const noexcept { return z; }

		//Setters
		void setX(float newX) noexcept { x = newX; }
		void setY(float newY) noexcept { y = newY; }
		void setZ(float newZ) noexcept { z = newZ; }

		//Modulo de un vector
		float modulo() const{
			return sqrt(powf(x, 2) + powf(y, 2) + powf(z, 2));
		}

		//Normalizar un vector
		Vector3D normalizar() const {
			const float mod = modulo();
			if (mod == 0.0f) {
				return Vector3D(0.0f,0.0f,0.0f);
			}
			return Vector3D(x / mod, y / mod, z / mod);
		}
		
		//producto de un vector por un escalar
		Vector3D operator*(float escalar) const noexcept {
			return Vector3D(x * escalar, y * escalar, z * escalar);
		}

		//producto escalar entre dos vectores
		float operator*(const Vector3D& other) const noexcept {
			return ((x * other.getX() ) + (y * other.getY() )+ (z * other.getZ()));
		}

		//sumar
		Vector3D operator+(const Vector3D& other) const noexcept {
			return Vector3D(x + other.getX(), y + other.getY(), z + other.getZ());
		}

		//restar
		Vector3D operator-(const Vector3D& other) const noexcept {
			return Vector3D(x - other.getX(), y - other.getY(), z - other.getZ());
		}

		//igualar
		Vector3D& operator=(const Vector3D& other) noexcept {
			if (this != &other) {
				x = other.getX();
				y = other.getY();
				z = other.getZ();
				return *this;
			}
		}
};
#endif // !VECTOR3D_H