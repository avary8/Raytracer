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
    Scene(shared_ptr<Camera> cam);
    void addObj(shared_ptr<Surface> newObj);

    HitResult traceRay(Ray& ray, float tmin, float tmax);
    glm::vec3 reflectRay(Ray& ray, HitResult& hit, int& limit);

    shared_ptr<Camera> getCam();

    void addNewCam(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
    void nextCam();

private:
    shared_ptr<Camera> currCam;
    vector<shared_ptr<Surface>> objects;
    vector<shared_ptr<Camera>> cameras;

    //std::vector<Light> lights;
    glm::vec3 backgroundColor;
    glm::vec3 ambientLight;
};