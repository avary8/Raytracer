#pragma once
#include <iostream>
#include <memory>
#include <glm/vec3.hpp>
using namespace std;

class Light {
public:
    Light(shared_ptr<glm::vec3> pos, bool status){
        this->pos = *pos;
        this->status = status;
    };

    void lightswitch(){
        cout << " old stat: " << status << endl;
        this->status = !status;
        cout << " new stat: " << status << endl;
    };

    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    bool status = false;
};