#pragma once
#include <iostream>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Ray.h"
using namespace std;

class Camera {
public:
    Camera(){};
    Camera(unsigned int x, unsigned int y, glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float fov);
        
    // glm::vec2 getScreenCoords();
    //glm::vec3 computeRay(int i, int j, float width, float height);
    glm::vec3 getPos();
    glm::vec3 getDir();
    void getCameraBasis(glm::vec3& u, glm::vec3& v, glm::vec3& w);
    


private:
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 3.0f);

    glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 v = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 w = glm::vec3(0.0f, 1.0f, 0.0f);



    float aspectRatio;
    float fov = 45.0f;


    unsigned int x, y;
    float l, r, b, t;

};