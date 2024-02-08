#include "Camera.h"


Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up){
    this->pos = pos;
    this->dir = glm::normalize(pos - target);
    this->up = up;
}

void Camera::getCameraBasis(glm::vec3& u, glm::vec3& v, glm::vec3& w){
    w = dir;
    u = glm::normalize(glm::cross(up, w));
    v = glm::normalize(glm::cross(w, u));
    // u = right
    // v = up
    // w = direction
}

glm::vec3 Camera::getPos(){
    return pos;
}

glm::vec3 Camera::getDir(){
    return dir;
}
