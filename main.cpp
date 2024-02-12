#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <cmath>
#include <float.h>
#include "Camera.h"
#include "Ray.h"
#include "Hits.h"
#include "Scene.h"
#include "Shapes.h"
#include "Light.h"

#include <math.h>
#include <cmath>

#include <vector>
#include <memory>
#include <iostream>
using namespace std;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
Scene createScene();
void generateCams(Scene& scene);
void generateCams2(Scene& scene);
void generateCams3(Scene& scene);
void rayTrace(Scene scene);
void initOpenGLstuff(GLFWwindow*& window, unsigned int& texture, unsigned int& shaderProgram, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 800;
int PIX_WIDTH = 800; 
int PIX_HEIGHT = 800;
int limit = 3;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
	"gl_Position = vec4(aPos, 1.0);\n"
	"ourColor = aColor;\n"
	"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture1, TexCoord);\n"
    "}\n\0";

struct CallbackParameters {
    GLFWwindow *window;
    Scene* scene;
};


int main(){
    GLFWwindow* window;
    unsigned int texture;
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    initOpenGLstuff(window, texture, shaderProgram, VBO, VAO, EBO);
    
    Scene scene = createScene();
    CallbackParameters callbackParams;
    callbackParams.window = window;
    callbackParams.scene = &scene;

    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)){
        // input
        // -----
        glfwSetWindowUserPointer(window, &callbackParams);
        glfwSetKeyCallback(window, key_callback);

        // float deltaTime = glfwGetTime();
        // glfwSetTime(0.0);
        // currInterval += deltaTime;

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        rayTrace(scene);
        scene.nextCam();


        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // ------------------------------------------------------------------------------
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    CallbackParameters* params = static_cast<CallbackParameters*>(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        params->scene->switchPerspective();
    } else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        params->scene->clearCams();
        generateCams(*params->scene);
    } else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        params->scene->clearCams();
        generateCams2(*params->scene);
    } else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        params->scene->changeLight(0);
    } else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        params->scene->changeLight(1);
    } else if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        params->scene->changeLight(2);
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        PIX_HEIGHT = min(PIX_HEIGHT + 100, 2000);
        PIX_WIDTH = min(PIX_WIDTH + 100, 2000);
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        PIX_HEIGHT = max(PIX_HEIGHT - 100, 0);
        PIX_WIDTH = max(PIX_WIDTH - 100, 0);
    } else if (key == GLFW_KEY_8 && action == GLFW_PRESS){
        limit = max(limit-1, 0);
    } else if (key == GLFW_KEY_9 && action == GLFW_PRESS){
        limit = min(limit+1, 5);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Scene createScene(){
    /*-------------------------Camera Init-------------------------*/
    OrthoCam orthoCam(glm::vec3(0.0f, 2.0f, 30.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT); 
    Camera perspectCam(glm::vec3(0.0f, 100.0f, 800.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);


    Camera sphereCam(glm::vec3(-145.0f, 50.1f, 45.0f), glm::vec3(-200.0f, 50.0f, 45.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);



    /*-------------------------Light Init-------------------------*/
    Light l1 = Light(make_shared<glm::vec3>(glm::vec3{-250.0f, 200.0f, 150.0f}), true);
    Light l2 = Light(make_shared<glm::vec3>(glm::vec3{0.0f, 250.0f, 150.0f}), true);
    Light l3 = Light(make_shared<glm::vec3>(glm::vec3{100.0f, 250.0f, 200.0f}), true);
    

    /*-------------------------Shape Init-------------------------*/
    Sphere newSphere(25.0f, glm::vec3(175.0f, 125.1f, -2.0f), glm::vec3(20, 20, 255));
    Sphere newSphere2(25.0f, glm::vec3(-100.0f, 25.1f, 20.0), glm::vec3(255, 0, 0));
    Sphere newSphere3(40.0f, glm::vec3(25.0f, 40.1f, 20.0f), glm::vec3(0, 255, 0));
    Sphere newSphere4(50.0f, glm::vec3(-200.0f, 50.1f, 45.0f), glm::vec3(255, 255, 204));


    vector<glm::vec3> vertices = {
        glm::vec3(175.0f, 100.2f, 0.0f),
        glm::vec3(250.0f, 0.2f, 0.0f),
        glm::vec3(100.0f, 0.2f, 0.0f),
        glm::vec3(175.0f, 0.2f, 150.0f)
    };

    Triangle tri1(vector<glm::vec3>{vertices[0], vertices[1], vertices[2]}, glm::vec3(255, 204, 255));
    Triangle tri2(vector<glm::vec3>{vertices[0], vertices[1], vertices[3]}, glm::vec3(255, 204, 255));
    Triangle tri3(vector<glm::vec3>{vertices[0], vertices[2], vertices[3]}, glm::vec3(255, 204, 255));
    Triangle tri4(vector<glm::vec3>{vertices[1], vertices[2], vertices[3]}, glm::vec3(255, 204, 255));

    glm::vec3 floorPosition(-400.0f, 0.0f, 5000.0f); 
    glm::vec3 floorNormal(0.0f, 1.0f, 0.0f);
    glm::vec3 floorColor(200, 200, 200);
    Plane floor(floorPosition, normalize(floorNormal), floorColor);


    /*---------------Saving to Scene------------*/
    Scene scene(make_shared<OrthoCam>(orthoCam), make_shared<Camera>(perspectCam));
    //Scene scene(make_shared<OrthoCam>(orthoCam), make_shared<Camera>(sphereCam));
    scene.addLight(make_shared<Light>(l1));
    scene.addLight(make_shared<Light>(l2));
    scene.addLight(make_shared<Light>(l3));

    scene.addObj(make_shared<Plane>(floor));
    scene.addObj(make_shared<Triangle>(tri1));
    scene.addObj(make_shared<Triangle>(tri2));
    scene.addObj(make_shared<Triangle>(tri3));
    scene.addObj(make_shared<Triangle>(tri4));
    scene.addObj(make_shared<Sphere>(newSphere));
    scene.addObj(make_shared<Sphere>(newSphere2));
    scene.addObj(make_shared<Sphere>(newSphere3));
    scene.addObj(make_shared<Sphere>(newSphere4));

    //generateCams(scene);
    return scene;
}

void generateCams(Scene& scene){
    float radius = scene.getCam()->getPos().z;
    int steps = 20;
    float increment = 2 * M_PI / steps;

    for (int i = 0; i < steps; i++){
        float angle = increment * i;
        float x = radius * cos(angle);
        float z =  radius * sin(angle);
        float y = scene.getCam()->getPos().y;
        
        scene.addNewCam(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    }
    // angle = 0.0f;
    // while (angle < 1){
    //     float a =  angle;
    //     scene.addNewCam(glm::vec3(0.0f, 2.0f, -a), glm::vec3(0.0f, 0.0f, -80.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV, SCR_WIDTH, SCR_HEIGHT);
    //     angle += 0.1f;
    // }
}

void generateCams2(Scene& scene){
    float radius = scene.getCam()->getPos().z;
    int steps = 20;
    float increment = 2 * M_PI / steps;

    for (int i = 0; i < steps; i++){
        float angle = increment * i;
        float x = radius * cos(angle);
        float z =  radius * sin(angle);
        float y = scene.getCam()->getPos().y;
        
        scene.addNewCam(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
        if (i == steps/4){
            for (int j = 0; j < steps; j++){
                if (j > steps/2){
                    z += (j - (steps/2)) * 10;
                } else {
                    z -= j * 10;
                }
                scene.addNewCam(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
            }
        }
    }
}

// used to make animation #3 
void generateCams3(Scene& scene){
    // radius : 50.0f
    // origin :(-200.0f, 50.1f, 45.0f)
    // radius --> 60

    glm::vec3 lookAt = {-200.0f, 50.0f, 45.0f};
    float x = -145.0f;
    float y = 50.1f;
    float z = 45.0f;

    cout << "before loop 1 x: " << x << " Y: " << y << " Z: " << z << endl;
    //-------------------loop 1
    // starts at: -145, 50.1, 45 
    int steps = 20;
    for (int i = 0; i < steps; i++){
        y -= 2;
        scene.addNewCam(glm::vec3(x, y, z), lookAt, glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    }

    //-------------------loop 2
    // starts at: -145, 10.1, 45 
    cout << "before loop 2 x: " << x << " Y: " << y << " Z: " << z << endl;
    for (int i = 0; i < steps; i++){
        x -= 10;
        z += 10;
        scene.addNewCam(glm::vec3(x, y, z), lookAt, glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    }

    //-------------------loop 3
    // starts at: -345, 10.1, 245 
    cout << "before loop 3 x: " << x << " Y: " << y << " Z: " << z << endl;
    float incX = (-x)/steps;
    float incY = (100-y)/steps;
    float incZ = (800-z)/steps;

    for (int i = 0; i < steps; i++){
        x += incX;
        y += incY;
        z += incZ;
        scene.addNewCam(glm::vec3(x, y, z), lookAt, glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    }

    //-------------------reverse loop 3
    // starts at: 0, 100, 800 
    // needs to end at: -345, 10.1, 245 
    cout << "before loop 4 x: " << x << " Y: " << y << " Z: " << z << endl;
    for (int i = 0; i < steps; i++){
        x -= incX;
        y -= incY;
        z -= incZ;
        scene.addNewCam(glm::vec3(x, y, z), lookAt, glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    }

    //-------------------reverse loop 2
    // starts at: -345, 10.1, 245 
    // needs to end at: -145, 10.1, 45 
    cout << "before loop 5 x: " << x << " Y: " << y << " Z: " << z << endl;
    // x = -145.0f + i * 10;
    // z = 45.0f + i * 10;
    for (int i = 0; i < steps; i++){
        x += 10;
        z -= 10;
        scene.addNewCam(glm::vec3(x, y, z), lookAt, glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    }

    //-------------------reverse loop 1
    // starts at: -145, 10.1, 45  
    // needs to end at: -145, 50.1, 45
    cout << "before loop 6 x: " << x << " Y: " << y << " Z: " << z << endl;
    for (int i = 0; i < steps; i++){
       y += 2;
       scene.addNewCam(glm::vec3(x, y, z), lookAt, glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    }
    
    // // new pos == {0, 100, 800}
    // float radius = 800.0f;
    // float increment = 2 * M_PI / steps;
    // for (int i = 0; i < steps; i++){
    //     float angle = increment * i;
    //     float x = radius - (radius * cos(angle));
    //     float z = radius - (radius * sin(angle));
    //     float y = lastY;

    //     // fix this part
    //     scene.addNewCam(glm::vec3(x, y, z), lookAt, glm::vec3(0.0f, 1.0f, 0.0f), SCR_WIDTH, SCR_HEIGHT);
    // }
}


void rayTrace(Scene scene){
    // Create the image (RGB Array) to be displayed
    unsigned char* image = new unsigned char[PIX_WIDTH*PIX_HEIGHT*3];

    for (int j = 0; j < PIX_HEIGHT; j++){
        for (int i = 0; i < PIX_WIDTH; i++){
            Ray ray = scene.getCam()->generateRay(i, j, PIX_WIDTH, PIX_HEIGHT);
            HitResult hit = scene.traceRay(ray, .001, FLT_MAX);

            if (hit.hit) {
                int l = limit;
                glm::vec3 L = scene.shadeRay(ray, 0.001, FLT_MAX, l);
                L = glm::pow(L, glm::vec3(1.0f / 1.3f));
                L = glm::clamp(L, 0.0f, 255.0f);

                

                int idx = (j * PIX_WIDTH + i) * 3;
                image[idx] = static_cast<unsigned char>(L[0]); 
                image[idx+1] = static_cast<unsigned char>(L[1]);
                image[idx+2] = static_cast<unsigned char>(L[2]);
            } else {
                int idx = (j * PIX_WIDTH + i) * 3;
                image[idx] = static_cast<unsigned char>(153); 
                image[idx+1] = static_cast<unsigned char>(153);
                image[idx+2] = static_cast<unsigned char>(255);
            }
        }
    }

    unsigned char *data = &image[0];
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PIX_WIDTH, PIX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
}



void initOpenGLstuff(GLFWwindow*& window, unsigned int& texture, unsigned int& shaderProgram, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Raytracing :)", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glViewport(-SCR_WIDTH, -SCR_HEIGHT, SCR_WIDTH, SCR_HEIGHT);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture 
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}