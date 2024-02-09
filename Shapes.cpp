#include "Shapes.h"


Surface::Surface(){
    material = Material();
}


Surface::Surface(glm::vec3 rgb){
    material = Material(rgb);
}

HitResult Surface::intersect(Ray & ray, float & tmin, float & tmax) const {
    return HitResult();
}

Material Surface::getMaterial() {
    return material;
}


Plane::Plane(float width, float length, const glm::vec3& position, const glm::vec3& normal, const glm::vec3& rgb){
    this->width = width;
    this->length = length;
    this->position = position;
    this->normal = glm::normalize(normal);
    material = Material(rgb);
}

// HitResult Plane::intersect(Ray& ray, float& tmin, float& tmax) const {
//     // Check if the ray intersects the floor
//     // Compute the hit point, normal, material color, etc.
//     // Update tmin and tmax accordingly
//     // Return a HitResult structure

//     // Example implementation:
//     HitResult hitResult;
//     hitResult.hit = false;
//     float denom = glm::dot(this->normal, ray.getDir());
//     if (std::fabs(denom) > EPSILON) {
//         glm::vec3 rayToPlane = this->position - ray.getOrigin();
//         float t = glm::dot(rayToPlane, this->normal) / denom;
//         t = (- dot(this->normal, ray.getOrigin()) / denom);
//         // Intersection within the valid range
//         if (t >= tmin && t <= tmax) {
//             hitResult.hit = true;
//             hitResult.t = t;
//             hitResult.hitPt = ray.evaluate(t);
//             hitResult.normal = normal;
//         }
//     }
//     return hitResult;
// }

// HitResult Plane::intersect(Ray& ray, float& tmin, float& tmax) const {
//     // Check if the ray intersects the floor
//     // Compute the hit point, normal, material color, etc.
//     // Update tmin and tmax accordingly
//     // Return a HitResult structure

//     // Example implementation:
//     HitResult hitResult;
//     hitResult.hit = false;

//     float denom = glm::dot(ray.getDir(), this->normal);
//     if (std::abs(denom) > EPSILON) {
//         glm::vec3 rayToPlane = this->position - ray.getOrigin();
//         float t = glm::dot(rayToPlane, this->normal) / denom;

//         // Intersection within the valid range
//         if (t >= tmin && t <= tmax) {
//             glm::vec3 intersectPt = ray.evaluate(t);

//             // Calculate local coordinates relative to the plane's origin
//             glm::vec3 localIntersect = intersectPt - position;
//             glm::vec3 axis1 = glm::normalize(glm::cross(normal, glm::vec3(0, 1, 0)));
//             glm::vec3 axis2 = glm::normalize(glm::cross(normal, axis1));
//             float distWidth = glm::dot(localIntersect, glm::normalize(axis1));
//             float distLength = glm::dot(localIntersect, glm::normalize(axis2));

//             // Check if the intersection point is within the bounds of the plane
//             if (distWidth >= 0 && distWidth <= width &&
//                 distLength >= 0 && distLength <= length) {
//                     //cout << "hit";
//                 hitResult.hit = true;
//                 hitResult.t = t;
//                 hitResult.hitPt = intersectPt;
//                 hitResult.normal = normal;
//             }
//         }
//     }
//     return hitResult;
// }

HitResult Plane::intersect(Ray& ray, float& tmin, float& tmax) const {
    // Check if the ray intersects the floor
    // Compute the hit point, normal, material color, etc.
    // Update tmin and tmax accordingly
    // Return a HitResult structure

    // Example implementation:
    HitResult hitResult;
    hitResult.hit = false;

    float denom = glm::dot(this->normal, ray.getDir());
    if (std::abs(denom) > EPSILON) {
        glm::vec3 rayToPlane = this->position - ray.getOrigin();
        float t = glm::dot(rayToPlane, this->normal) / denom;

        // Intersection within the valid range
        if (t >= tmin && t <= tmax) {
            //cout << "hit";
            hitResult.hit = true;
            hitResult.t = t;
            hitResult.hitPt = ray.evaluate(t);
            hitResult.normal = normal;
            
        }
    }
    return hitResult;
}


Triangle::Triangle(vector<glm::vec3> vertices, glm::vec3 rgb){
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
        //hitRes.normal = glm::normalize(hitRes.hitPt);
    }

    return hitRes;
}


Sphere::Sphere(float radius, glm::vec3 origin, glm::vec3 rgb){
    this->radius = radius;
    this->origin = origin;
    material = Material(rgb);
}

// HitResult Sphere::intersect(Ray& ray, float& tmin, float& tmax) const {
//     HitResult hitRes;
//     hitRes.hit = false;
//     float t, t2;
//     // p = ray's origin

//     //float discriminant = pow(dot(ray.getDir(), ray.getOrigin()), 2) - dot(ray.getOrigin(), ray.getOrigin()) + 1;
//     float discriminant = dot(ray.getDir(), ray.getDir()) * (dot(ray.getOrigin(), ray.getOrigin()) - radius * radius);

//     // float DO = -dot(ray.getDir(), ray.getOrigin());
//     // float DD = dot(ray.getDir(), ray.getDir());

//     float DO = -dot(ray.getDir(), ray.getOrigin());
//     float DD = dot(ray.getDir(), ray.getDir());

//     //float discriminant = pow(dot(ray.getDir(), origin), 2) - dot(ray.getDir(), ray.getDir()) * dot(origin, origin) - pow(radius, 2);


//     if (discriminant == 0){
//         cout << "hit0";
//         // ray grazes the sphere. touching at exactly 1 point
//         hitRes.hit = true;
//         hitRes.t = DO;
//         hitRes.hitPt = ray.evaluate(DO);
//         hitRes.normal = (hitRes.hitPt - this->origin);
//         /// radius; // Normalized vector from center to hit point
//     } else if (discriminant > 0){
//         cout << "hit";
//         // 2 solutions.
//         //   1. ray enters sphere
//         //   2. ray leaves sphere 
//         t = (DO + sqrt(discriminant));
//         t2 = (DO - sqrt(discriminant));
//         if ((t < t2 || t > t2) && t > tmin && t < tmax){
//             cout << " t ";
//             hitRes.hit = true;
//             hitRes.t = t;
//             hitRes.hitPt = ray.evaluate(t);
//             hitRes.normal = glm::normalize(hitRes.hitPt - this->origin);
//             // / radius; // Normalized vector from center to hit point
//         } else if ((t2 < t || t2 > t) && t2 > tmin && t2 < tmax){
//             cout << " t2 ";
//             hitRes.hit = true;
//             hitRes.t = t2;
//             hitRes.hitPt = ray.evaluate(t2);
//             hitRes.normal = glm::normalize(hitRes.hitPt - this->origin);
//             // / radius; // Normalized
//         }
//     } 
//     cout << endl;

//     // else if negative
//     // line and sphere do not intersect
//     return hitRes;
// }

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
        if (t > tmin && t < tmax) {
            hitRes.hit = true;
            hitRes.t = t;
            hitRes.hitPt = ray.evaluate(t);
            hitRes.normal = glm::normalize(hitRes.hitPt - this->origin);
        }
        t = (-b + sqrt(discriminant)) / a;
        if (t > tmin && t < tmax) {
            hitRes.hit = true;
            hitRes.t = t;
            hitRes.hitPt = ray.evaluate(t);
            hitRes.normal = glm::normalize(hitRes.hitPt - this->origin);
        }
    }
    return hitRes;
}