#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <memory>
#include <float.h>
#include <glm/vec3.hpp>
#include "Ray.h"
#include "Shapes.h"
#include "Camera.h"
#include "Hits.h"
#include "Light.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace std;

class Scene {
public:
    Scene(const shared_ptr<OrthoCam>& cam, const shared_ptr<Camera>& cam2);
    void addObj(const shared_ptr<Surface>& newObj);
    void addLight(const shared_ptr<Light>& light);
    void changeLight(const int& lightNum);
    vector<shared_ptr<Light>>& getLights();

    HitResult traceRay(Ray& ray, float tmin, float tmax);
    glm::vec3 shadeRay(Ray& ray, float tmin, float tmax, int& limit);

    void switchPerspective();

    shared_ptr<Camera> getCam();   
    void addNewCam(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, int& w, int& h);
    void nextCam();
    void clearCams();

private:
    shared_ptr<Camera> currCam;
    int currCamIndex = 0;
    vector<shared_ptr<Surface>> objects;
    vector<shared_ptr<Camera>> cameras;
    vector<shared_ptr<Light>> lights;

    shared_ptr<OrthoCam> orthoCam;
    shared_ptr<Camera> perspectCam;

    glm::vec3 backgroundColor = {153, 153, 255};
};