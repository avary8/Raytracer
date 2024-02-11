#include "Shapes.h"


Surface::Surface(){
    material = Material();
}

Surface::Surface(glm::vec3& rgb){
    material = Material(rgb);
}

HitResult Surface::intersect(Ray & ray, float & tmin, float & tmax) const {
    return HitResult();
}

Material Surface::getMaterial() {
    return material;
}

Plane::Plane(glm::vec3& position, const glm::vec3& normal, glm::vec3& rgb){
    this->position = position;
    this->normal = glm::normalize(normal);
    this->material = Material(rgb);
}

HitResult Plane::intersect(Ray& ray, float& tmin, float& tmax) const {
    HitResult hitRes;
    hitRes.hit = false;

    float denom = glm::dot(this->normal, ray.getDir());
    if (std::abs(denom) > tmin) {
        glm::vec3 rayToPlane = this->position - ray.getOrigin();
        float t = glm::dot(rayToPlane, this->normal) / denom;

        if (t >= tmin && t <= tmax) {
            //cout << "hit";
            hitRes.hit = true;
            hitRes.t = t;
            hitRes.hitPt = ray.evaluate(t);
            hitRes.normal = this->normal;
        }
    }
    return hitRes;
}

Triangle::Triangle(const vector<glm::vec3>& vertices, const glm::vec3& rgb){
    this->vertices = vertices;
    this->material = Material(rgb);
    this->normal = glm::cross(vertices[0], vertices[1]);
}

HitResult Triangle::intersect(Ray& ray, float& tmin, float& tmax) const {
    HitResult hitRes;
    hitRes.hit = false;
    // p = ray's origin
    float a = vertices[0].x - vertices[1].x;
    float b = vertices[0].y - vertices[1].y;
    float c = vertices[0].z - vertices[1].z;

    float d = vertices[0].x - vertices[2].x;
    float e = vertices[0].y - vertices[2].y;
    float f = vertices[0].z - vertices[2].z;

    float g = ray.getDir().x;
    float h = ray.getDir().y;
    float i = ray.getDir().z;

    float j = vertices[0].x - ray.getOrigin().x;
    float k = vertices[0].y - ray.getOrigin().y;
    float l = vertices[0].z - ray.getOrigin().z;


    float M = a * ((e * i)-(h * f)) + b * ((g * f) - (d * i) + c * ((d * h) - (e * g)));

    float beta = (j * ((e * i) - (h * f)) + k * ((g * f) - (d * i)) + l * ((d * h) - (e * g))) / M;

    float gamma = (i * ((a * k) - (j * b)) + h * ((j * c) - (a * l)) + g * ((b * l) - (k * c))) / M;

    float t = -(f * ((a * k) - (j * b)) + e * ((j * c) - (a * l)) + d * ((b * l) - (k * c))) / M;

    if (t > tmin && t < tmax && gamma > 0 && gamma < 1 && beta > 0 && beta < 1-gamma){
        hitRes.hit = true;
        hitRes.t = t;
        hitRes.hitPt = ray.evaluate(t);
        hitRes.normal = glm::normalize(glm::cross((vertices[1] - vertices[0]), (vertices[2] - vertices[0])));
    }

    return hitRes;
}

Sphere::Sphere(float radius, const glm::vec3& origin, const glm::vec3& rgb){
    this->radius = radius;
    this->origin = origin;
    material = Material(rgb);
}

HitResult Sphere::intersect(Ray& ray, float& tmin, float& tmax) const {
    HitResult hitRes;
    hitRes.hit = false;
    
    glm::vec3 ec = ray.getOrigin() - this->origin;
    float a = glm::dot(ray.getDir(), ray.getDir());
    float b = glm::dot(ec, ray.getDir());
    float c = glm::dot(ec, ec) - pow(radius, 2);
    float discriminant = b * b - a * c;

    if (discriminant == 0) {
        float t = -b / a;
        hitRes.hit = true;
        hitRes.t = t;
        hitRes.hitPt = ray.evaluate(t);
        hitRes.normal = glm::normalize(hitRes.hitPt - this->origin);

    } else if (discriminant > 0){
        float t = (-b - sqrt(discriminant)) / a;
        float t2 = (-b + sqrt(discriminant)) / a;
        if ((t < t2 || t > t2) && (t > tmin && t < tmax)) {
            hitRes.hit = true;
            hitRes.t = t;
            hitRes.hitPt = ray.evaluate(t);
            hitRes.normal = glm::normalize(hitRes.hitPt - this->origin);
        } else if ((t > t2 || t < t2) && (t2 > tmin && t2 < tmax)) {
            hitRes.hit = true;
            hitRes.t = t2;
            hitRes.hitPt = ray.evaluate(t2);
            hitRes.normal = glm::normalize(hitRes.hitPt - this->origin);
        }
    }
    return hitRes;
}