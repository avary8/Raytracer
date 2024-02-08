#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 dir){
    this->origin = origin;
    this->dir = dir;
}

glm::vec3 Ray::evaluate(float t){
    return  origin + t*dir;
}

glm::vec3 Ray::getOrigin(){
    return this->origin;
}

glm::vec3 Ray::getDir(){
    return this->dir;
}