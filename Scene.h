#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <float.h>
#include <glm/vec3.hpp>
#include "Ray.h"
#include "Shapes.h"
#include "Camera.h"
#include "Hits.h"
using namespace std;


class Scene {
public:
    Scene(Camera cam);
    void addObj(shared_ptr<Surface> newObj);

    HitResult traceRay(Ray& ray, float tmin, float tmax);
    glm::vec3 reflectRay(Ray& ray, HitResult& hit, int& limit);

    Camera getCam();

    //void setNewCam();
    // maybe have a vector of camera that you can switch between
    // void changeCam();

private:
    Camera camera;
    vector<shared_ptr<Surface>> objects;
    //std::vector<Light> lights;
    glm::vec3 backgroundColor;
    glm::vec3 ambientLight;
};