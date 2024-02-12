#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <float.h>
#include "Ray.h"
#include "Material.h"

using namespace std;

class HitResult {
public:
    bool hit = false;
	glm::vec3 hitPt;
    glm::vec3 normal;
    float t = FLT_MAX;
    Material material;
};