#include "Scene.h"


Scene::Scene(const shared_ptr<OrthoCam>& cam, const shared_ptr<Camera>& cam2){
    this->orthoCam = cam;
    this->perspectCam = cam2;
    this->currCam = cam2;
    cameras.push_back(cam2);
}

void Scene::addObj(const shared_ptr<Surface>& newObj){
    this->objects.push_back(newObj);
}

void Scene::addLight(const shared_ptr<glm::vec3>& pos){
    this->lights.push_back(pos);
}

vector<shared_ptr<glm::vec3>>& Scene::getLights(){
    return this->lights;
}

HitResult Scene::traceRay(Ray& ray, float tmin, float tmax) {
    HitResult hitRes;
    hitRes.hit = false;
    Surface obj;
    for (const auto& object : objects) {
        HitResult objectHit = object->intersect(ray, tmin, tmax);
        if (objectHit.hit && objectHit.t < hitRes.t) {
            hitRes = objectHit;
            hitRes.material = object->getMaterial();
            tmax = objectHit.t;
        }
    }
    return hitRes;
}


glm::vec3 Scene::shadeRay(Ray& ray, float tmin, float tmax, int& limit) {
    glm::vec3 L = {0.0f, 0.0f, 0.0f};
    HitResult hit = traceRay(ray, tmin, tmax);
    if (hit.hit){
        Material mat = hit.material;
        for (int i = 0; i < lights.size(); i++){
            glm::vec3 light = *lights[i];
            glm::vec3 lightDir = normalize(light - hit.hitPt);
            
            // Ambient
            L += mat.ambientColor * mat.ambientI;

            // shadows
            Ray shadowRay = Ray(hit.hitPt, light);
            HitResult shadowHit = traceRay(shadowRay, tmin, tmax);
            if (shadowHit.hit){
                //L *= 0.5;
                //L += mat.ambientColor * mat.ambientI;
            } else {
                // Diffuse
                L += mat.diffuseI * mat.diffuseColor * max(0.0f, glm::dot(lightDir, hit.normal));

                // Specular
                glm::vec3 vh = (light + hit.hitPt)/glm::length(light + hit.hitPt);
                //glm::vec3 vh = glm::normalize(lightDir + glm::normalize(ray.getDir()));
                L += mat.specularI * mat.specularColor * max(0.0f, glm::pow(glm::dot(vh, hit.normal), mat.p));
            }
        }  

        // mirror
        if(limit > 0){
            glm::vec3 reflectDir = ray.getDir() - 2.0f * glm::dot(ray.getDir(), hit.normal) * hit.normal;
            if (typeid(*currCam) == typeid(OrthoCam)) {
                reflectDir = glm::reflect(-ray.getDir(), hit.normal);
            }
            
            Ray reflectRay(hit.hitPt, reflectDir);
            HitResult reflectHit = traceRay(reflectRay, tmin, tmax);
            if (reflectHit.hit){
                limit -= 1;
                L += reflectHit.material.specularI * this->shadeRay(reflectRay, tmin, tmax, limit);
            }

            return L;
        } else {
            return backgroundColor;
        }
    }
    return L;
}

void Scene::switchPerspective(){
    clearCams();
    if (typeid(*currCam) == typeid(Camera)){
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

void Scene::addNewCam(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, float& FOV, int& w, int& h){
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