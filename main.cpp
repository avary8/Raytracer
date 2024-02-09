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
void processInput(GLFWwindow *window);
Scene createScene();
Scene generateOrthographic();
Scene generatePerspective();
void generateCams(Scene& scene);
void rayTrace(Scene scene);
void initOpenGLstuff(GLFWwindow*& window, unsigned int& texture, unsigned int& shaderProgram, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const unsigned int PIX_WIDTH = 800;
const unsigned int PIX_HEIGHT = 800;
const float FOV = 60.0f;

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



int main(){
    GLFWwindow* window;
    unsigned int texture;
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    initOpenGLstuff(window, texture, shaderProgram, VBO, VAO, EBO);
    
    Scene scene = createScene();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)){
        // input
        // -----
        processInput(window);
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
        // -------------------------------------------------------------------------------
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



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Scene createScene(){
    Scene orthoScene = generateOrthographic();
    //Scene perspectiveScene = generatePerspective();

    generateCams(orthoScene);
    return orthoScene;
}

Scene generateOrthographic(){
    // maybe make this so you make scene and then ask scene to make this stuff ? idk
    // Camera camera (width, height, pos, lookAt, up, fov);
    //Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.4f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f);
    // > positive z makes camera walk away from scene and towards (screen (me))
    // ^ zooming out
    // when we go negative z. we walk to the other side basically . and the objects are flipped because we are viewing from diff side
    Camera camera(glm::vec3(0.0f, 3.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV, SCR_WIDTH, SCR_HEIGHT); 

    // look from above so that z-axis is now up and down
    //Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 200.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f);

    // z closer to zero == closer to camera 
    // positive z is coming out towards screen... so closer ?
    // a visible object origin z-value NEEDS to be > radius + camera.pos.y 
    // otherwise it is not visible because it is too close / behind camera
    // | negative z-values | when greater than radius + cam.pos.y are also visible.... which one shouldnt be though ?
    // i think negative should be visible and positive not

    // Sphere newSphere(50.0f, glm::vec3(20.0f, 50.0f, 0.0f), glm::vec3(255, 204, 255));
    // Sphere newSphere2(60.0f, glm::vec3(250.0f, 60.0f, 100.0f), glm::vec3(255, 0, 0));
    // Sphere newSphere3(70.0f, glm::vec3(300.0f, 75.0f, 100.0f), glm::vec3(0, 255, 0));
    // Sphere newSphere4(50.0f, glm::vec3(-350.0f, 160.0f, -1.0f), glm::vec3(255, 255, 204));

    Sphere newSphere(25.0f, glm::vec3(100.0f, 50.0f, 0.0f), glm::vec3(255, 204, 255));
    Sphere newSphere2(20.0f, glm::vec3(-100.0f, 20.0f, 0.0), glm::vec3(255, 0, 0));
    Sphere newSphere3(40.0f, glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0, 255, 0));

    Sphere newSphere4(50.0f, glm::vec3(-200.0f, 50.0f, 0.0f), glm::vec3(255, 255, 204));

    vector<glm::vec3> vertices = {
        glm::vec3(175.0f, 200.0f, 0.0f),
        glm::vec3(250.0f, 0.1f, 0.0f),
        glm::vec3(100.0f, 0.1f, 0.0f),
        glm::vec3(175.0f, 0.1f, 150.0f)
    };


    Triangle tri1(vector<glm::vec3>{vertices[0], vertices[1], vertices[2]}, glm::vec3(255, 204, 255));
    Triangle tri2(vector<glm::vec3>{vertices[0], vertices[1], vertices[3]}, glm::vec3(255, 204, 255));
    Triangle tri3(vector<glm::vec3>{vertices[0], vertices[2], vertices[3]}, glm::vec3(255, 204, 255));
    Triangle tri4(vector<glm::vec3>{vertices[1], vertices[2], vertices[3]}, glm::vec3(255, 204, 255));


    glm::vec3 floorPosition(-400.0f, 0.0f, 0.0f); 
    glm::vec3 floorNormal(0.0f, 1.0f, 0.0f);
    glm::vec3 floorColor(100, 100, 100);

    // Create a Plane object with the specified parameters
    Plane floor(800.0f, 800.0f, floorPosition, normalize(floorNormal), floorColor);

    Scene scene(make_shared<Camera>(camera));

    scene.addObj(make_shared<Triangle>(tri1));
    scene.addObj(make_shared<Triangle>(tri2));
    scene.addObj(make_shared<Triangle>(tri3));
    scene.addObj(make_shared<Triangle>(tri4));
    scene.addObj(make_shared<Sphere>(newSphere));
    scene.addObj(make_shared<Sphere>(newSphere2));
    scene.addObj(make_shared<Sphere>(newSphere3));
    scene.addObj(make_shared<Sphere>(newSphere4));
    scene.addObj(make_shared<Plane>(floor));

    return scene;
}

Scene generatePerspective(){
    // perspective cams
    // bigger z = moving farther away from scene . moving towards screen (me)
    //Camera camera(glm::vec3(0.0f, 700.0f, 1500.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV, SCR_WIDTH, SCR_HEIGHT);

    Camera camera(glm::vec3(0.0f, 100.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV, SCR_WIDTH, SCR_HEIGHT);

    // not this one
    //Camera camera(glm::vec3(0.0f, 100.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 
    // positive z is coming out towards screen... so closer ?
    // bigger z == closer to camera == bigger
    Sphere newSphere(50.0f, glm::vec3(100.0f, 50.0f, 200.0f), glm::vec3(255, 204, 255));
    Sphere newSphere2(20.0f, glm::vec3(-100.0f, 20.0f, 60.0), glm::vec3(255, 0, 0));
    Sphere newSphere3(40.0f, glm::vec3(0.0f, 40.0f, 200.0f), glm::vec3(0, 255, 0));

    Sphere newSphere4(50.0f, glm::vec3(-200.0f, 50.0f, 1.0f), glm::vec3(255, 255, 204));

    
    vector<glm::vec3> vertices = {
        glm::vec3(350.0f, 400.0f, 0.0f),
        glm::vec3(500.0f, 0.0f, 0.0f),
        glm::vec3(200.0f, 0.0f, 0.0f),
        glm::vec3(350.0f, 0.0f, 300.0f)
    };


    Triangle tri1(vector<glm::vec3>{vertices[0], vertices[1], vertices[2]}, glm::vec3(255, 204, 255));
    Triangle tri2(vector<glm::vec3>{vertices[0], vertices[1], vertices[3]}, glm::vec3(255, 204, 255));
    Triangle tri3(vector<glm::vec3>{vertices[0], vertices[2], vertices[3]}, glm::vec3(255, 204, 255));
    Triangle tri4(vector<glm::vec3>{vertices[1], vertices[2], vertices[3]}, glm::vec3(255, 204, 255));


    glm::vec3 floorPosition(-400.0f, 0.0f, 0.0f); 
    glm::vec3 floorNormal(0.0f, 1.0f, 0.0f);
    glm::vec3 floorColor(100, 100, 100);

    // Create a Plane object with the specified parameters
    Plane floor(1600.0f, 1600.0f, floorPosition, normalize(floorNormal), floorColor);

    Scene scene(make_shared<Camera>(camera));
    scene.addObj(make_shared<Triangle>(tri1));
    scene.addObj(make_shared<Triangle>(tri2));
    scene.addObj(make_shared<Triangle>(tri3));
    scene.addObj(make_shared<Triangle>(tri4));
    scene.addObj(make_shared<Sphere>(newSphere));
    scene.addObj(make_shared<Sphere>(newSphere2));
    scene.addObj(make_shared<Sphere>(newSphere3));
    scene.addObj(make_shared<Sphere>(newSphere4));
    scene.addObj(make_shared<Plane>(floor));

    return scene;
}


void generateCams(Scene& scene){
    // perspective
    //float radius = 800.0f; 
    // ortho
    float radius = 20.0f; 
    float angle = 0.0f;
    while (angle < 360){
        float x = radius * std::cos(angle * (M_PI / 180.0f));
        float z =  radius * std::sin(angle * (M_PI / 180.0f));

        // perspective
        //scene.addNewCam(glm::vec3(x, 200.0f, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV, SCR_WIDTH, SCR_HEIGHT);

        // ortho
        scene.addNewCam(glm::vec3(x, 20.0f, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV, SCR_WIDTH, SCR_HEIGHT);

        angle += 10;
    }
    // while (angle > 0){
    //     float a =  angle;
    //     scene.addNewCam(glm::vec3(0.0f, 100.0f, a), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), FOV, SCR_WIDTH, SCR_HEIGHT);
    //     angle -= 500;
    // }
}



void rayTrace(Scene scene){
     // Create the image (RGB Array) to be displayed
    unsigned char* image = new unsigned char[PIX_WIDTH*PIX_HEIGHT*3];

    // i think this is like the light direction
    // perspective cam light
    glm::vec3 ptLight = {-300.0f, 300.0f, 50.0f};
    //glm::vec3 ptLight = {0.0f, 100.0f, 300.0f};

    // i think this is like the light direction
    // ortho cam light
    //glm::vec3 ptLight = {-200.0f, 200.0f, 50.0f};

    for (int j = 0; j < PIX_HEIGHT; j++){
        for (int i = 0; i < PIX_WIDTH; i++){
            // get u and v --> screen coordinates 
            // u and v are just a point . like (50 , 9)

            // p(t) = e + t(s-e)
            // e = ray origin 
            // s - e = ray direction
            // s is point on img plane 

            //Ray ray(scene.getCam()->getPos(), glm::normalize(glm::vec3{u, v, -1.0f} - scene.getCam()->getPos()));

            // where (u, v) are coordinates of the pixel's pos on the image plane
            // u = i;
            // v = j;

            // i, j are the origin's wrt pixel -- screen

            Ray ray = scene.getCam()->generateRay(i, j, PIX_WIDTH, PIX_HEIGHT);

           

            //Ray ray(scene.getCam()->getPos(), glm::normalize(u * camU + v * camV + camW));


            // ... (Intersection testing, shading, etc.)
            HitResult hit = scene.traceRay(ray, .001, FLT_MAX);
        
            // If an intersection is found, get hit information
            if (hit.hit) {
                //cout << "i: " << i << " :j: " << j << " : hit" << endl;

                // ambient, diffuse, specular, shadows all for just 1 light source

                glm::vec3 lightDir = normalize(ptLight - hit.hitPt);
                
                glm::vec3 L = {0, 0, 0};
                Material mat = hit.material;

                // Ambient
                L = mat.ambientColor * mat.ambientI;

                //glm::vec3 l = (ptLight-hit.hitPt)/glm::normalize(ptLight-hit.hitPt);

                // Diffuse
                L += mat.diffuseI * mat.diffuseColor * max(0.0f, glm::dot(lightDir, hit.normal));

                // Specular
                glm::vec3 vh = (ptLight + hit.hitPt)/glm::length(ptLight + hit.hitPt);
                //glm::vec3 vh = glm::normalize(lightDir + glm::normalize(ray.getDir()));
                L += mat.specularI * mat.specularColor * max(0.0f, glm::pow(glm::dot(vh, hit.normal), mat.p));

                // shadows
                Ray shadowRay = Ray(hit.hitPt, ptLight);
                HitResult shadowHit = scene.traceRay(shadowRay, .001, FLT_MAX);
                if (shadowHit.hit){
                    //L *= 0.85;
                    L = mat.ambientColor * mat.ambientI;
                }

                int limit = 1;
                hit.reflectance = scene.reflectRay(ray, hit, limit); 
                // mirror
                 L += 0.01f * hit.reflectance;
                //L *= hit.reflectance;
                //L += mat.specularI * glm::normalize(mat.specularColor * hit.reflectance) * hit.reflectance;
            
                glm::vec3 correctedColor = glm::pow(L, glm::vec3(1.0f / 1.0f));
                L = glm::clamp(correctedColor, 0.0f, 255.0f);

                //L = glm::clamp(L, 0.0f, 255.0f);


                hit.material.L = L;
                

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