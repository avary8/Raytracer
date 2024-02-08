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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const unsigned int PIX_WIDTH = 800;
const unsigned int PIX_HEIGHT = 800;



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
    // maybe make this so you make scene and then ask scene to make this stuff ? idk
    // Camera camera (width, height, pos, lookAt, up, fov);
    //Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.4f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f);


    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f);

    // look from above so that z-axis is now up and down
    //Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 200.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f);


    //vector<glm::vec3> vertices = {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.0f, 0.0f, 0.0f)};

    // z closer to zero == closer to camera 
    // positive z is coming out towards screen... so closer ?
    Sphere newSphere(200.0f, glm::vec3(200.0f, 200.0f, 500.0f), glm::vec3(255, 204, 255));
    Sphere newSphere2(200.0f, glm::vec3(-500.0f, 200.0f, -4.0f), glm::vec3(255, 0, 0));
    Sphere newSphere3(100.0f, glm::vec3(350.0f, 200.0f, -1.0f), glm::vec3(0, 255, 0));

    Sphere newSphere4(50.0f, glm::vec3(-700.0f, 600.0f, 5.0f), glm::vec3(255, 255, 204));


    // Triangle newTriangle(vertices, glm::vec3(255.0, 0.0, 0.0));

    glm::vec3 floorPosition(0.0f, 0.0f, 0.0f); 
    glm::vec3 floorNormal(0.0f, 1.0f, 0.0f);
    glm::vec3 floorColor(100, 100, 100);

    // Create a Plane object with the specified parameters
    Plane floor(1600.0f, 1600.0f, floorPosition, normalize(floorNormal), floorColor);


    Scene scene(camera);
    
    //scene.addObj(make_shared<Triangle>(newTriangle));
    scene.addObj(make_shared<Sphere>(newSphere));
    scene.addObj(make_shared<Sphere>(newSphere2));
    scene.addObj(make_shared<Sphere>(newSphere3));
    scene.addObj(make_shared<Sphere>(newSphere4));
    scene.addObj(make_shared<Plane>(floor));


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Raytracing :)", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
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
    unsigned int shaderProgram = glCreateProgram();
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
    unsigned int VBO, VAO, EBO;
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
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create the image (RGB Array) to be displayed
    unsigned char* image = new unsigned char[PIX_WIDTH*PIX_HEIGHT*3];
    // for(int i = 0; i < height; i++){
    //     for (int j = 0; j < width; j++){
    //         int idx = (i * width + j) * 3;
    //         image[idx] = (unsigned char) (255 * i*j/height/width); //((i+j) % 2) * 255;
    //         image[idx+1] = 0;
    //         image[idx+2] = 0;
    //     }
    // }
    float u , v;
    float r = SCR_WIDTH;
    float l = -r;
    float t = SCR_HEIGHT;
    float b = -t;
    glm::vec3 camU, camV, camW;
    scene.getCam().getCameraBasis(camU, camV, camW);

    float ux = 0.0;
    float uy = 0.0;

    // lx = camera.getPos()[0] 
    // ly = camera.getPos()[1]
    // lz = camera.getPos()[2]
    if (scene.getCam().getDir()[0] == 0 && scene.getCam().getDir()[1] == 0){
        ux = fabs(scene.getCam().getDir()[2]);
    } else {
        ux = scene.getCam().getDir()[1];
        uy = -scene.getCam().getDir()[0];
    }
    glm::vec3 uVect = {ux, uy, 0};
    glm::vec3 vVect = cross(uVect, scene.getCam().getDir());

    int pixelSize[4] = { 800, 0, 800, 0};
    int scrWid = pixelSize[1] - pixelSize[0];
    float scrHt = pixelSize[3] - pixelSize[2];

    glm::vec3 ptLight = {-200.0f, 400.0f, 0.0f};

    for (int i = 0; i < PIX_WIDTH; i++){
        for (int j = 0; j < PIX_HEIGHT; j++){
            // get u and v --> screen coordinates 
            // u and v are just a point . like (50 , 9)

            // p(t) = e + t(s-e)
            // e = ray origin 
            // s - e = ray direction
            // s is point on img plane 

            // pixel at position (i, j) in the raster image has the pos: 
            u = l + (r-l) * (i+0.5)/PIX_WIDTH;
            v = b + (t-b) * (j+0.5)/PIX_HEIGHT;

            // u = float(i);
            // v = float(j);

            // u =  (j+0.5)/PIX_WIDTH;
            // v =  (i+0.5)/PIX_HEIGHT;
            // where (u, v) are coordinates of the pixel's pos on the image plane
        
            // i, j are the origin's wrt pixel -- screen
            Ray ray (scene.getCam().getPos() + (u * camU) + (v * camV), camW);

            // for perspective i think
            //Ray ray (scene.getCam().getPos() - camW + (u-(l/2)) * camW * u + (v - (l/2)) * h * v, camW);

            // Ray ray(scene.getCam().getPos()(), glm::normalize(scene.getCam().getViewMatrix() * scene.getCam().getProjectionMatrix() * glm::vec4(u, v, 1.0, 1.0)));

            // ... (Intersection testing, shading, etc.)
            HitResult hit = scene.traceRay(ray, .001, FLT_MAX);
        
            // If an intersection is found, get hit information
            if (hit.hit) {
                //cout << "i: " << i << " :j: " << j << " : hit" << endl;

                // ambient, diffuse, specular, shadows all for just 1 light source
                glm::vec3 L = {0, 0, 0};
                Material mat = hit.material;
                // just ambient 
                L = mat.ambientColor * mat.ambientI;

                //glm::vec3 l = (ptLight-hit.hitPt)/glm::normalize(ptLight-hit.hitPt);
                glm::vec3 l = glm::normalize(ptLight-hit.hitPt);
                L += mat.diffuseI * mat.diffuseColor * max(0.0f, glm::dot(l, hit.normal));

                //float gamma = 2.2f; 
                //glm::vec3 correctedColor = glm::pow(L, glm::vec3(1.0f / gamma));
                //L = glm::clamp(correctedColor, 0.0f, 255.0f);

                glm::vec3 vh = (ptLight + hit.hitPt)/glm::length(ptLight + hit.hitPt);
                L += mat.specularColor * max(0.0f, glm::pow(glm::dot(hit.normal, vh), mat.p));

                L = glm::clamp(L, 0.0f, 255.0f);

                // mirror
                //L += hit.reflectance;

                // shadows
                Ray shadowRay = Ray(hit.hitPt, ptLight);
                HitResult shadowHit = scene.traceRay(shadowRay, .001, FLT_MAX);
                if (shadowHit.hit){
                    L *= 0.8f;
                }

                


                // just diffuse
                //L = objMat.diffuseColor * objMat.diffuseI;
                // just specular
                //L = objMat.specularColor * objMat.specularI;

                int idx = (j * PIX_WIDTH + i) * 3;
                image[idx] = static_cast<unsigned char>(L[0]); 
                image[idx+1] = static_cast<unsigned char>(L[1]);
                image[idx+2] = static_cast<unsigned char>(L[2]);
            } else {
                int idx = (j * PIX_WIDTH + i) * 3;
                image[idx] = static_cast<unsigned char>(153); 
                image[idx+1] = static_cast<unsigned char>(153);;
                image[idx+2] = static_cast<unsigned char>(255);;
            }

            //cout << "I: " << i << " : j : " << j << " rgb: [" << image[(j * PIX_WIDTH + i) * 3] << " , " << image[((j * PIX_WIDTH + i) * 3)+1] << " , " << image[((j * PIX_WIDTH + i) * 3)+2] << endl;
            // glm::vec3 color = computeShading(hitResult, lights, scene);
            //glm::vec3 color = computeShading(hitResult);

            // glColor3f(color.r, color.g, color.b); // Red color
            // glVertex2i(i, j);
            // glColor3f(pixelColors[i][j].r, pixelColors[i][j].g, pixelColors[i][j].b);
            // glVertex2f(2.0f * i / SCR_WIDTH - 1.0f, 1.0f - 2.0f * j / SCR_HEIGHT);
        }
    }
    cout << "done" << endl;

    unsigned char *data = &image[0];
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PIX_WIDTH, PIX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}