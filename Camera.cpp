#include "Camera.h"


Camera::Camera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, float fov, int width, int height){
    this->pos = pos;
    this->lookAt = lookAt; 
    this->dir = glm::normalize(pos - lookAt); // w
    this->right = glm::normalize(glm::cross(up, this->dir)); // u - right
    this->up = glm::normalize(glm::cross(this->dir, this->right)); // v - up

    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->h = tan(fov);
    this->w = h * aspectRatio;

    this->r = width/2;
    this->l = -r;
    this->t = height/2;
    this->b = -t;
}

// right = u 
// up =  v
// dir =  w

Ray Camera::generateRay(int i, int j, int pixWidth, int pixHeight){
    float u , v;
    // pixel at position (i, j) in the raster image has the pos: 
    u = l + ((r-l) * (i+0.5))/static_cast<float>(pixWidth);
    v = b + ((t-b) * (j+0.5))/static_cast<float>(pixHeight);

    // perspective
    Ray ray (this->pos, this->right * u + this->up * v + this->dir * -800.0f);

    return ray;
}

Ray OrthoCam::generateRay(int i, int j, int pixWidth, int pixHeight){
    float u , v;
    // pixel at position (i, j) in the raster image has the pos: 
    u = l + ((r-l) * (i+0.5))/static_cast<float>(pixWidth);
    v = b + ((t-b) * (j+0.5))/static_cast<float>(pixHeight);

    // orthographic
    Ray ray (this->pos + (u * this->right) + (v * this->up), this->dir);

    return ray;
}

glm::vec3 Camera::getPos(){
    return pos;
}

glm::vec3 Camera::getDir(){
    return dir;
}
