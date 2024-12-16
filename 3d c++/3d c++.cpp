// 3d c++.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 920;

Camera camera(glm::vec3(5.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float rotationX = 0.0f;
float rotationY = 0.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

glm::vec3 lightPosition(2, 2, -2);

int main()
{
	Window window;
	window.create(SCR_HEIGHT, SCR_WIDTH, "Window");
    int width, height;
    glfwGetFramebufferSize(window.getWindow(), &width, &height);
    glViewport(0, 0, width, height);
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetScrollCallback(window.getWindow(), scroll_callback);
    
    float vertices[] = {
        //Face avant
         0.5f, -0.5f, 0.5f, 1.f, 1.f, // R-D
         0.5f,  0.5f, 0.5f, 1.f, 0.f, // R-T
        -0.5f, -0.5f, 0.5f, 0.f, 1.f, // L-D
        -0.5f, -0.5f, 0.5f, 0.f, 1.f, // L-D
         0.5f,  0.5f, 0.5f, 1.f, 0.f, // R-T
        -0.5f,  0.5f, 0.5f, 0.f, 0.f, // 
        //Face droite
        0.5f, -0.5f,  0.5f, 0.f, 1.f,
        0.5f,  0.5f,  0.5f, 0.f, 0.f,
        0.5f,  0.5f, -0.5f, 1.f, 0.f,
        0.5f, -0.5f,  0.5f, 0.f, 1.f,
        0.5f, -0.5f, -0.5f, 1.f, 1.f,
        0.5f,  0.5f, -0.5f, 1.f, 0.f,
        //Face gauche
        -0.5f, -0.5f,  0.5f, 1.f, 1.f,
        -0.5f,  0.5f,  0.5f, 1.f, 0.f,
        -0.5f,  0.5f, -0.5f, 0.f, 0.f,
        -0.5f, -0.5f,  0.5f, 1.f, 1.f,
        -0.5f, -0.5f, -0.5f, 0.f, 1.f,
        -0.5f,  0.5f, -0.5f, 0.f, 0.f,
        //Face haute
        -0.5f, 0.5f,  0.5f, 0.f, 1.f,
         0.5f, 0.5f,  0.5f, 1.f, 1.f,
        -0.5f, 0.5f, -0.5f, 0.f, 0.f,
         0.5f, 0.5f,  0.5f, 1.f, 1.f,
         0.5f, 0.5f, -0.5f, 1.f, 0.f,
        -0.5f, 0.5f, -0.5f, 0.f, 0.f,
        //Face basse
        -0.5f, -0.5f,  0.5f, 0.f, 0.f,
         0.5f, -0.5f,  0.5f, 1.f, 0.f,
        -0.5f, -0.5f, -0.5f, 0.f, 1.f,
         0.5f, -0.5f,  0.5f, 1.f, 0.f,
         0.5f, -0.5f, -0.5f, 1.f, 1.f,
        -0.5f, -0.5f, -0.5f, 0.f, 1.f,
        // Face arrière
         0.5f, -0.5f, -0.5f, 1.f, 1.f,
         0.5f,  0.5f, -0.5f, 1.f, 0.f,
        -0.5f, -0.5f, -0.5f, 0.f, 1.f,
        -0.5f, -0.5f, -0.5f, 0.f, 1.f,
         0.5f,  0.5f, -0.5f, 1.f, 0.f,
        -0.5f,  0.5f, -0.5f, 0.f, 0.f,
    };

    Shader shaderLight("./shaderLight.shader", "./fragmentLight.shader");
    
	Shader s("./vertexShader.shader", "./fragmentShader.shader");
    window.SetShader(&s);
    Object object(vertices, sizeof(vertices));
    object.SetVertexAttributes(0, 3, GL_FLOAT, 5 * sizeof(float), 0);
	Object o("./cow.txt");
	Object teapot("./teapot.txt");
    //o.GetFileContentLine("./object.txt");
    o.SetVertexAttributes(0, 3, GL_FLOAT, 6 * sizeof(float), 0);
    o.SetVertexAttributes(2, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    teapot.SetVertexAttributes(0, 3, GL_FLOAT, 6 * sizeof(float), 0);
    teapot.SetVertexAttributes(2, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //o.SetVertexAttributes(1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    s.Use();
	glm::mat4 view = glm::mat4(1.0f);
	
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 teapotModel = glm::mat4(1.0f);
	//projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //view = glm::rotate(view, glm::radians(180.0f), glm::vec3(0, 1, 0));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
    teapotModel = glm::translate(teapotModel, glm::vec3(0.0f, 0.0f, -10.0f));
    

    shaderLight.Use();
    shaderLight.SetMatrix4("projection", projection);
    shaderLight.SetMatrix4("view", view);
    glm::mat4 lightmodel = glm::mat4(1.0f);
    lightmodel = glm::translate(lightmodel, lightPosition);
    lightmodel = glm::scale(lightmodel, glm::vec3(0.2f, 0.2f, 0.2f));
    ;

    s.Use();
    s.SetInt("texture", 0);

	s.Use();
    s.SetVector3("lightColor", 1.0f, 1.0f, 1.0f);
    s.SetVector3("objectColor", 1.0f, 1.0f, 1.0f);
    s.SetFloat("ambientStrengh", 0.1f);
    s.SetVector3("lightPos", lightPosition);
    s.SetVector3("viewPos", camera.Position);
	s.SetMatrix4("projection", projection);
    view = camera.GetViewMatrix();
	s.SetMatrix4("view", view);
	s.SetMatrix4("model", model);



	while (window.isOpen()) {
		window.pollEvent();
        

        processInput(window.getWindow());
		window.clear(0.2, 0.2, 0.2);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
       
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));  
        model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));  

        teapotModel = glm::mat4(1.0f);
        teapotModel = glm::translate(teapotModel, glm::vec3(10.0f, -4.0f, -10.0f));
        teapotModel = glm::rotate(teapotModel, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        teapotModel = glm::rotate(teapotModel, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));


		
        view = camera.GetViewMatrix();
        s.Use();
        s.SetMatrix4("view", view);
        s.SetMatrix4("model", model);
        s.SetVector3("lightPos", lightPosition);
        s.SetMatrix4("projection", projection);
        

        
		o.draw(GL_TRIANGLES, 0, 20000);
        s.SetMatrix4("model", teapotModel);
		teapot.draw(GL_TRIANGLES, 0, 20000);
        
        

        shaderLight.Use();
        glm::mat4 lightmodel = glm::mat4(1.0f);
        lightmodel = glm::translate(lightmodel, lightPosition);
        lightmodel = glm::scale(lightmodel, glm::vec3(0.2f, 0.2f, 0.2f));
        shaderLight.SetMatrix4("view", view); 
        shaderLight.SetMatrix4("projection", projection);
        shaderLight.SetMatrix4("model", lightmodel);
        object.draw(GL_TRIANGLES, 0, 36);

	}
	
	return 0;
	
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float lightSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) 
        lightPosition.y += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) 
        lightPosition.y -= lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) 
        lightPosition.x -= lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) 
        lightPosition.x += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) 
        lightPosition.z += lightSpeed;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) 
        lightPosition.z -= lightSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    float rotationSpeed = 50.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotationY -= rotationSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotationY += rotationSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        rotationX -= rotationSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        rotationX += rotationSpeed * deltaTime;

    if (rotationX > 360.0f || rotationX < -360.0f)
        rotationX = 0.0f;

    if (rotationY > 360.0f || rotationY < -360.0f)
        rotationY = 0.0f;
}