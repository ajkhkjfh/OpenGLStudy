#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include"Renderer.h"

#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"Shader.h"
#include"VertexArray.h"
#include"VertexBufferLayout.h"

#include"Texture.h"

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"

#include"Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float xLast = SCR_WIDTH / 2.0f;
float yLast = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLCALL(glEnable(GL_DEPTH_TEST));

    float positions[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    /*GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));*/

    VertexArray cubeVa;
    VertexBuffer vb(positions, 36, 8, sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    cubeVa.AddBuffer(vb, layout);

    VertexArray lightVa;
    lightVa.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader lightingShader("res/shaders/MultipleLightingShader.shader");
    Shader lightCubeShader("res/shaders/lightCubeShader.shader");

    lightingShader.Bind();
    Texture diffuseMap("res/textures/container2.png");
    diffuseMap.Bind(1);
    lightingShader.SetUniform1i("material.diffuse", 0);

    Texture specularMap("res/textures/lighting_maps_specular_color.png");
    specularMap.Bind(2);
    lightingShader.SetUniform1i("material.specular", 1);

    /*Texture emissionMap("res/textures/matrix.jpg");
    emissionMap.Bind(3);
    lightingShader.SetUniform1i("material.emission", 2);*/

    float color[3] = { 1.0f, 0.5f, 0.3f };

    Renderer renderer;

    /*{
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }*/

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        /*ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();*/

        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

        lightingShader.Bind();
        lightingShader.SetUniform3f("viewPos", camera.Position);
        lightingShader.SetUniform1f("material.shininess", 64.0f);

        // directional light
        lightingShader.SetUniform3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        lightingShader.SetUniform3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniform3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.SetUniform3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        // point light 1
        lightingShader.SetUniform3f("pointLights[0].position", pointLightPositions[0]);
        lightingShader.SetUniform3f("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniform3f("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniform3f("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[0].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[0].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightingShader.SetUniform3f("pointLights[1].position", pointLightPositions[1]);
        lightingShader.SetUniform3f("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniform3f("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniform3f("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[1].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[1].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightingShader.SetUniform3f("pointLights[2].position", pointLightPositions[2]);
        lightingShader.SetUniform3f("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniform3f("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniform3f("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[2].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[2].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightingShader.SetUniform3f("pointLights[3].position", pointLightPositions[3]);
        lightingShader.SetUniform3f("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.SetUniform3f("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniform3f("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("pointLights[3].constant", 1.0f);
        lightingShader.SetUniform1f("pointLights[3].linear", 0.09f);
        lightingShader.SetUniform1f("pointLights[3].quadratic", 0.032f);
        // spotLight
        lightingShader.SetUniform3f("spotLight.position", camera.Position);
        lightingShader.SetUniform3f("spotLight.direction", camera.Front);
        lightingShader.SetUniform3f("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.SetUniform3f("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.SetUniform1f("spotLight.constant", 1.0f);
        lightingShader.SetUniform1f("spotLight.linear", 0.09f);
        lightingShader.SetUniform1f("spotLight.quadratic", 0.032f);
        lightingShader.SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        //Pass projection Matrix to shader
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.SetUniformMat4f("projection", projection);

        //Camera/View
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.SetUniformMat4f("view", view);

        for (unsigned int i = 0;i < 10;i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.SetUniformMat4f("model", model);
            
            renderer.DrawArrays(cubeVa, lightingShader, vb);
        }


        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.SetUniformMat4f("model", model);

        

        lightCubeShader.Bind();
        lightCubeShader.SetUniformMat4f("projection", projection);
        lightCubeShader.SetUniformMat4f("view", view);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(lightVa, lightCubeShader, vb);
        }     

        /*ImGui::Begin("My name is window,ImGui window");
        ImGui::Text("Hello there adventurer");
        ImGui::ColorEdit3("objectColor", color);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();*/

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        xLast = xpos;
        yLast = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - xLast;
    float yoffset = yLast - ypos;//窗口左上角是（0，0）

    xLast = xpos;
    yLast = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
