#pragma once
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
using namespace std;

class Ray {
public:
    Ray(glm::vec3 origin, glm::vec3 dir);
    glm::vec3 evaluate(float t);

    glm::vec3 getOrigin();
    glm::vec3 getDir();
    

private:
    glm::vec3 origin; // origin 
    // orthographic : direction is fixed and negative
    glm::vec3 dir; // ray direction
};