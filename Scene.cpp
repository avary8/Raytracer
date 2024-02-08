#include "Scene.h"


Scene::Scene(Camera cam){
    this->camera = cam;
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



Camera Scene::getCam(){
    return camera;
}