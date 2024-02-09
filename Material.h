#pragma once
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
using namespace std;


class Material {
public:
    Material(){
        ambientColor = {0, 0, 0};
        diffuseColor = {0, 0, 0};
        specularColor = {0, 0, 0};
    }
    Material(const glm::vec3& rgb){
        ambientColor = rgb;
        diffuseColor = rgb;
        specularColor = rgb * 3.0f;
        L = rgb;
    };

    glm::vec3 evaluate(glm::vec3& l, glm::vec3& v, glm::vec3& n);

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    float ambientI = 0.2;
    float diffuseI = 0.4;
    float specularI = 0.4;

    glm::vec3 L;

    float p = 15.0f;
};
