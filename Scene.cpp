#include "Scene.h"


Scene::Scene(shared_ptr<Camera> cam){
    cameras.push_back(cam);
    this->currCam = cam;
}

void Scene::addObj(shared_ptr<Surface> newObj){
    this->objects.push_back(newObj);
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
            obj = *object;
            hitRes = objectHit;
            hitRes.material = object->getMaterial();
        }
    }
    if (hitRes.hit){
        int limit = 2;
        //hitRes.reflectance = reflectRay(ray, hitRes, limit); 
    }
    return hitRes;
}


glm::vec3 Scene::reflectRay(Ray& ray, HitResult& hit, int& limit) {
    if (limit > 0) {
        glm::vec3 reflectDir = ray.getDir() - 2.0f * glm::dot(ray.getDir(), hit.normal) * hit.normal;
        Ray reflectRay(hit.hitPt, reflectDir);
        HitResult reflectHit = traceRay(reflectRay, 0.001f, FLT_MAX);
        limit -= 1;
        if (reflectHit.hit){
            return reflectHit.material.diffuseColor;// * this->reflectRay(reflectRay, reflectHit, limit);
        }
    }
    return {0, 0, 0};
}


void Scene::addNewCam(glm::vec3 pos, glm::vec3 target, glm::vec3 up){
    cameras.push_back(make_shared<Camera>(Camera(pos, target, up)));
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
}


shared_ptr<Camera> Scene::getCam(){
    return currCam;
}