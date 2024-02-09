#include "Scene.h"


Scene::Scene(shared_ptr<OrthoCam> cam, shared_ptr<Camera> cam2){
    this->orthoCam = cam;
    this->perspectCam = cam2;
    this->currCam = cam2;
    cameras.push_back(cam2);
}

void Scene::addObj(shared_ptr<Surface> newObj){
    this->objects.push_back(newObj);
}

void Scene::addLight(shared_ptr<glm::vec3> light){
    this->lights.push_back(light);
}

vector<shared_ptr<glm::vec3>> Scene::getLights(){
    return this->lights;
}

HitResult Scene::traceRay(Ray& ray, float tmin, float tmax) {
    HitResult hitRes;
    hitRes.hit = false;
    Surface obj;
    for (const auto& object : objects) {
        HitResult objectHit = object->intersect(ray, tmin, tmax);
        // if (objectHit.hit && object->getRgb()[1] == 0 ){
        //     if (objectHit.t > hitRes.t){
        //         cout << "objectHit t: " << objectHit.t << " hitRes t:  " << hitRes.t << endl;
        //     }
        // }
        if (objectHit.hit && objectHit.t < hitRes.t) {
            hitRes = objectHit;
            hitRes.material = object->getMaterial();
            tmax = objectHit.t;
        }
    }
    return hitRes;
}

glm::vec3 Scene::reflectRay(Ray& ray, HitResult& hit, int& limit) {
    if (limit > 0) {
        //glm::vec3 reflectDir = ray.getDir() - 2.0f * glm::dot(ray.getDir(), hit.normal) * hit.normal;
        glm::vec3 reflectDir = ray.getDir() - 2.0f * glm::dot(ray.getDir(), hit.normal) * hit.normal;
        Ray reflectRay(hit.hitPt, reflectDir);
        HitResult reflectHit = traceRay(reflectRay, 0.001f, FLT_MAX);
        if (reflectHit.hit){
            limit -= 1;
            return reflectHit.material.L + this->reflectRay(ray, reflectHit, limit);
        }
    }
    return hit.material.L;
}

void Scene::switchPerspective(){
    clearCams();
    if (this->currCam == this->perspectCam){
        this->currCam = this->orthoCam;
        cameras.push_back(currCam);
        return;
    }
    this->currCam = this->perspectCam;
    cameras.push_back(currCam);
}

void Scene::clearCams(){
    cameras.clear();
}

void Scene::addNewCam(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up, float FOV, unsigned int w, unsigned int h){
    if (typeid(*currCam) == typeid(OrthoCam)) {
        cameras.push_back(make_shared<OrthoCam>(OrthoCam(pos, lookAt, up, FOV, w, h)));
    } else {
        cameras.push_back(make_shared<Camera>(Camera(pos, lookAt, up, FOV, w, h)));
    }
}

void Scene::nextCam(){
    for (int i = 0; i < cameras.size(); i++) {
        if (cameras[i] == currCam){
            if (i == cameras.size()-1) {
                currCam = cameras[0];
            } else {
                currCam = cameras[++i];
            }
            return;
        }
    }
    if (cameras.size() > 0){
        currCam = cameras[0];
    }
}


shared_ptr<Camera> Scene::getCam(){
    return currCam;
}