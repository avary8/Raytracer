#pragma once
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/geometric.hpp>
#include <limits>
#include <memory>
#include "Ray.h"
#include "Material.h"
#include "Hits.h"
using namespace std;

const float EPSILON = std::numeric_limits<float>::epsilon();

class Surface {
public:
	Surface();
	Surface(glm::vec3 rgb);
	virtual HitResult intersect(Ray& ray, float& tmin, float& tmax) const;
	// virtual means to use the derived class in favor of base class (if the obj calling the func is of derived class type)
	Material getMaterial();

protected:
	Material material;
};

class Plane : public Surface {
public:
    Plane(float width, float length, const glm::vec3& position, const glm::vec3& normal, const glm::vec3& rgb);
    HitResult intersect(Ray& ray, float& tmin, float& tmax) const override;

private:
    float width;
    float length;
    glm::vec3 position;
    glm::vec3 normal;
};


class Triangle : public Surface {
public:
	Triangle(vector<glm::vec3> vertices, glm::vec3 rgb);
	HitResult intersect(Ray& ray, float& tmin, float& tmax) const override;
	
private:
	vector<glm::vec3> vertices;
};

class Sphere : public Surface {
public:
	Sphere(float radius, glm::vec3 origin, glm::vec3 rgb);
	HitResult intersect(Ray& ray, float& tmin, float& tmax) const override;
	//base class implementations
	glm::vec3 getOrigin(){return origin;};

private:
	glm::vec3 origin;
	float radius;
};

