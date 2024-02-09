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
    Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up, float fov, int width, int height);
    virtual Ray generateRay(int i, int j, int pixWidth, int pixHeight);

    glm::vec3 getPos();
    glm::vec3 getDir();
    
protected:
    glm::vec3 pos = glm::vec3(0.0f, 20.0f, 3.0f);
    glm::vec3 dir = glm::vec3(0.0f, 0.0f, 1.0f); // w
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // v
    glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f); // u right

    glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, -1.0f);

    float fov;
    float aspectRatio;
    float h;
    float w;
    float l, r, t, b;
};


class OrthoCam : public Camera {
public:
    using Camera::Camera;
    virtual Ray generateRay(int i, int j, int pixWidth, int pixHeight);
};