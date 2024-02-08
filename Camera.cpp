#include "Camera.h"


Camera::Camera(const unsigned int x, const unsigned int y, glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov){
    this->x = x;
    this->y = y;
    this->pos = pos;
    this->dir = glm::normalize(pos - target);
    this->up = up;
    this->fov = fov;
    this->aspectRatio = static_cast<float>(x) / static_cast<float>(y);

    r = x / 2.0;
    l = -r;
    t = y / 2.0;
    b = -t;
}

void Camera::getCameraBasis(glm::vec3& u, glm::vec3& v, glm::vec3& w){
    w = dir;
    u = glm::normalize(glm::cross(up, w));
    v = glm::normalize(glm::cross(w, u));
}

// glm::vec3 Camera::computeRay(int i, int j, float width, float height){
//     float ndcX = (2.0f * x) / width - 1.0f;
//     float ndcY = 1.0f - (2.0f * y) / height;

//     // Compute ray direction in camera space
//     float tanFOV = tan(glm::radians(fov / 2.0f));
//     float cameraX = ndcX * aspectRatio * tanFOV;
//     float cameraY = ndcY * tanFOV;

//     // Compute ray direction in world space
//     glm::vec3 forward = glm::normalize(this->lookAt - this->pos);
//     glm::vec3 right = glm::normalize(glm::cross(forward, this->up));
//     glm::vec3 upVec = glm::normalize(glm::cross(right, forward));
//     glm::vec3 rayDirection = glm::normalize(forward + cameraX * right + cameraY * upVec);

//     return rayDirection;
// }

// glm::vec2 Camera::getScreenCoords(){
//     /*  x by y screen -->  800x800
//         (r-l) by (t-b) --> [-.5, 799.5] by [-.5, 799.5]
//             which means the width spans from -.5 to 799.5 
//             and the height spans from -.5 to 799.5
//     */

//     float u , v;
   
//     for (int i = 0; i < this->x; i++){
//         for (int j = 0; j < this->y; j++){
//             Ray newRay = Ray();
//             // get u and v --> screen coordinates 
//             // u and v are just a point . like (50 , 9)
//             u = l + (r-l)(i+0.5)/x;
//             v = b + (t-b)(j+0.5)/y;

//             newRay->o = e + glm::operator*(u, camera.u) + glm::operator*(v, camera.v);
//             newRay->d = -w;
//         }
//     }
// }


glm::vec3 Camera::getPos(){
    return pos;
}

glm::vec3 Camera::getDir(){
    return dir;
}
