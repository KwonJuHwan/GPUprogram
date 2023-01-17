#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(-9.4f, 1.0f, 7.5f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
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

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // ÅØ½ºÃ³ ¶È¹Ù·Î
    stbi_set_flip_vertically_on_load(true);

    // build and compile shaders
    // -------------------------
    Shader shader("shader/3.1.blending.vs", "shader/3.1.blending.fs");
    Shader normalshader("shader/4.normal_mapping.vs", "shader/4.normal_mapping.fs");
    Shader shaderSingleColor("shader/3.1.blending.vs", "shader/2.stencil_single_color.fs");
    Shader lightCubeShader("shader/4.2.light_cube.vs", "shader/4.2.light_cube.fs");
    Shader lightingShader("shader/5.4.light_casters.vs", "shader/5.4.light_casters.fs");
    //Shader lightingShader("shader/4.2.lighting_maps.vs", "shader/4.2.lighting_maps.fs");
 
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float gmdVertices[] = {  // °ü¹°´ë
        // positions          // texture Coords
        -0.5f, -1.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.6f, 0.0f, // ÁÂ ÇÏ // µÞ¸é
         0.5f, -1.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.5f,  1.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // ¿ì »ó
         0.5f,  1.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // ¿ì »ó
        -0.5f,  1.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.6f, 1.0f, // ÁÂ »ó
        -0.5f, -1.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.6f, 0.0f, // ÁÂ ÇÏ

        -0.5f, -1.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.6f, 0.0f, // ÁÂ ÇÏ // ¾Õ¸é
         0.5f, -1.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.5f,  1.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // ¿ì »ó
         0.5f,  1.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // ¿ì »ó
        -0.5f,  1.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.6f, 1.0f, // ÁÂ »ó
        -0.5f, -1.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.6f, 0.0f, // ÁÂ ÇÏ

        -0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ // ÁÂÃø
        -0.5f,  1.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.6f, 0.0f, // ¿ì ÇÏ
        -0.5f, -1.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.6f, 1.0f, // ¿ì »ó
        -0.5f, -1.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.6f, 1.0f, // ¿ì »ó
        -0.5f, -1.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // ÁÂ »ó
        -0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

         0.5f,  1.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ // ¿ìÃø
         0.5f,  1.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.6f, 0.0f, // ¿ì ÇÏ
         0.5f, -1.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.6f, 1.0f, // ¿ì »ó
         0.5f, -1.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.6f, 1.0f, // ¿ì »ó
         0.5f, -1.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // ÁÂ »ó
         0.5f,  1.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

        -0.5f, -1.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ // ¾Æ·¡
         0.5f, -1.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.5f, 0.0f, // ¿ì ÇÏ
         0.5f, -1.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.5f, 0.5f, // ¿ì »ó
         0.5f, -1.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.5f, 0.5f, // ¿ì »ó
        -0.5f, -1.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.5f, // ÁÂ »ó
        -0.5f, -1.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

        -0.5f,  1.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ // À­¸é
         0.5f,  1.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.5f, 0.0f, // ¿ì ÇÏ
         0.5f,  1.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.5f, 0.5f, // ¿ì »ó
         0.5f,  1.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.5f, 0.5f, // ¿ì »ó
        -0.5f,  1.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.5f, // ÁÂ »ó
        -0.5f,  1.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f // ÁÂ ÇÏ
    };

    float maruVertices[] = { // ¸¶·ç
        // positions         // texture Coords 
        -5.5f, -0.5f, -2.5f,0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // ÁÂ ÇÏ // µÞ¸é
         5.5f, -0.5f, -2.5f, 0.0f,  0.0f, -1.0f, 5.0f, 0.0f, // ¿ì ÇÏ
         5.5f,  0.5f, -2.5f, 0.0f,  0.0f, -1.0f, 5.0f, 0.5f, // ¿ì »ó
         5.5f,  0.5f, -2.5f, 0.0f,  0.0f, -1.0f, 5.0f, 0.5f, // ¿ì »ó
        -5.5f,  0.5f, -2.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.5f, // ÁÂ »ó
        -5.5f, -0.5f, -2.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

        -5.5f, -0.5f,  2.5f,0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // ÁÂ ÇÏ // ¾Õ¸é
         5.5f, -0.5f,  2.5f, 0.0f,  0.0f,  1.0f, 5.0f, 0.0f, // ¿ì ÇÏ
         5.5f,  0.5f,  2.5f,0.0f,  0.0f,  1.0f,  5.0f, 0.5f, // ¿ì »ó
         5.5f,  0.5f,  2.5f, 0.0f,  0.0f,  1.0f, 5.0f, 0.5f, // ¿ì »ó
        -5.5f,  0.5f,  2.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.5f, // ÁÂ »ó
        -5.5f, -0.5f,  2.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

        -5.5f,  0.5f,  2.5f, -1.0f,  0.0f,  0.0f,   5.0f, 0.5f, // ¿ì »ó // ÁÂÃø
        -5.5f,  0.5f, -2.5f,-1.0f,  0.0f,  0.0f,    0.0f, 0.5f, // ÁÂ »ó
        -5.5f, -0.5f, -2.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // ÁÂ ÇÏ
        -5.5f, -0.5f, -2.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // ÁÂ ÇÏ
        -5.5f, -0.5f,  2.5f, -1.0f,  0.0f,  0.0f,   5.0f, 0.0f, // ¿ì ÇÏ
        -5.5f,  0.5f,  2.5f, -1.0f,  0.0f,  0.0f,   5.0f, 0.5f, // ¿ì »ó

         5.5f,  0.5f,  2.5f, 1.0f,  0.0f,  0.0f,  5.0f, 0.5f, // ¿ì »ó // ¿ìÃø
         5.5f,  0.5f, -2.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.5f, // ÁÂ »ó
         5.5f, -0.5f, -2.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ
         5.5f, -0.5f, -2.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ
         5.5f, -0.5f,  2.5f,  1.0f,  0.0f,  0.0f, 5.0f, 0.0f, // ¿ì ÇÏ
         5.5f,  0.5f,  2.5f, 1.0f,  0.0f,  0.0f,  5.0f, 0.5f, // ¿ì »ó

        -5.5f, -0.5f, -2.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.5f, // ÁÂ ÇÏ // ¾Æ·¡
         5.5f, -0.5f, -2.5f, 0.0f, -1.0f,  0.0f, 5.0f, 0.5f, // ¿ì ÇÏ
         5.5f, -0.5f,  2.5f, 0.0f, -1.0f,  0.0f, 5.0f, 1.0f, // ¿ì »ó
         5.5f, -0.5f,  2.5f, 0.0f, -1.0f,  0.0f, 5.0f, 1.0f, // ¿ì »ó
        -5.5f, -0.5f,  2.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // ÁÂ »ó
        -5.5f, -0.5f, -2.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.5f, // ÁÂ ÇÏ

        -5.5f,  0.5f, -2.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.5f, // ÁÂ ÇÏ // À­¸é
         5.5f,  0.5f, -2.5f, 0.0f,  1.0f,  0.0f, 5.0f, 0.5f, // ¿ì ÇÏ
         5.5f,  0.5f,  2.5f, 0.0f,  1.0f,  0.0f, 5.0f, 1.0f, // ¿ì »ó
         5.5f,  0.5f,  2.5f, 0.0f,  1.0f,  0.0f, 5.0f, 1.0f, // ¿ì »ó
        -5.5f,  0.5f,  2.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // ÁÂ »ó
        -5.5f,  0.5f, -2.5f,  0.0f,  1.0f,  0.0f,0.0f, 0.5f // ÁÂ ÇÏ
    };

    float planeVertices[] = {
        // positions          // texture Coords 
         9.0f, -0.5f,  8.0f, 0.0f,  1.0f, 0.0f,  5.0f, 0.0f,
        -9.0f, -0.5f,  8.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
        -9.0f, -0.5f, -8.0f, 0.0f,  1.0f, 0.0f, 0.0f, 5.0f,

         9.0f, -0.5f,  8.0f, 0.0f,  1.0f, 0.0f ,5.0f, 0.0f,
        -9.0f, -0.5f, -8.0f, 0.0f,  1.0f, 0.0f, 0.0f, 5.0f,
         9.0f, -0.5f, -8.0f, 0.0f,  1.0f, 0.0f, 5.0f, 5.0f
    };

    float wall1Vertices[] = { // ¾ÕµÚ (Ä«¸Þ¶ó±âÁØ)
        // positions          // normals           // texture coords
        -9.5f, -4.0f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         9.5f, -4.0f, -0.15f,  0.0f,  0.0f, -1.0f,  6.0f,  0.0f,
         9.5f,  4.0f, -0.15f,  0.0f,  0.0f, -1.0f,  6.0f,  6.0f,
         9.5f,  4.0f, -0.15f,  0.0f,  0.0f, -1.0f,  6.0f,  6.0f,
        -9.5f,  4.0f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f,  6.0f,
        -9.5f, -4.0f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -9.5f, -4.0f,  0.15f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         9.5f, -4.0f,  0.15f,  0.0f,  0.0f,  1.0f,  6.0f,  0.0f,
         9.5f,  4.0f,  0.15f,  0.0f,  0.0f,  1.0f,  6.0f,  6.0f,
         9.5f,  4.0f,  0.15f,  0.0f,  0.0f,  1.0f,  6.0f,  6.0f,
        -9.5f,  4.0f,  0.15f,  0.0f,  0.0f,  1.0f,  0.0f,  6.0f,
        -9.5f, -4.0f,  0.15f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -9.5f,  4.0f,  0.15f, -1.0f,  0.0f,  0.0f,  6.0f,  0.0f,
        -9.5f,  4.0f, -0.15f, -1.0f,  0.0f,  0.0f,  6.0f,  6.0f,
        -9.5f, -4.0f, -0.15f, -1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
        -9.5f, -4.0f, -0.15f, -1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
        -9.5f, -4.0f,  0.15f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -9.5f,  4.0f,  0.15f, -1.0f,  0.0f,  0.0f,  6.0f,  0.0f,

         9.5f,  4.0f,  0.15f,  1.0f,  0.0f,  0.0f,  6.0f,  0.0f,
         9.5f,  4.0f, -0.15f,  1.0f,  0.0f,  0.0f,  6.0f,  6.0f,
         9.5f, -4.0f, -0.15f,  1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
         9.5f, -4.0f, -0.15f,  1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
         9.5f, -4.0f,  0.15f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         9.5f,  4.0f,  0.15f,  1.0f,  0.0f,  0.0f,  6.0f,  0.0f,

        -9.5f, -4.0f, -0.15f,  0.0f, -1.0f,  0.0f,  0.0f,  6.0f,
         9.5f, -4.0f, -0.15f,  0.0f, -1.0f,  0.0f,  6.0f,  6.0f,
         9.5f, -4.0f,  0.15f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
         9.5f, -4.0f,  0.15f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
        -9.5f, -4.0f,  0.15f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -9.5f, -4.0f, -0.15f,  0.0f, -1.0f,  0.0f,  0.0f,  6.0f,

        -9.5f,  4.0f, -0.15f,  0.0f,  1.0f,  0.0f,  0.0f,  6.0f,
         9.5f,  4.0f, -0.15f,  0.0f,  1.0f,  0.0f,  6.0f,  6.0f,
         9.5f,  4.0f,  0.15f,  0.0f,  1.0f,  0.0f,  6.0f,  0.0f,
         9.5f,  4.0f,  0.15f,  0.0f,  1.0f,  0.0f,  6.0f,  0.0f,
        -9.5f,  4.0f,  0.15f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -9.5f,  4.0f, -0.15f,  0.0f,  1.0f,  0.0f,  0.0f,  6.0f
    };

    float wall2Vertices[] = { // ÁÂ¿ì (Ä«¸Þ¶ó ±âÁØ)
        // positions          // normals           // texture coords
        -0.15f, -4.0f, -8.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.15f, -4.0f, -8.0f,  0.0f,  0.0f, -1.0f,  6.0f,  0.0f,
         0.15f,  4.0f, -8.0f,  0.0f,  0.0f, -1.0f,  6.0f,  6.0f,
         0.15f,  4.0f, -8.0f,  0.0f,  0.0f, -1.0f,  6.0f,  6.0f,
        -0.15f,  4.0f, -8.0f,  0.0f,  0.0f, -1.0f,  0.0f,  6.0f,
        -0.15f, -4.0f, -8.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.15f, -4.0f,  8.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.15f, -4.0f,  8.0f,  0.0f,  0.0f,  1.0f,  6.0f,  0.0f,
         0.15f,  4.0f,  8.0f,  0.0f,  0.0f,  1.0f,  6.0f,  6.0f,
         0.15f,  4.0f,  8.0f,  0.0f,  0.0f,  1.0f,  6.0f,  6.0f,
        -0.15f,  4.0f,  8.0f,  0.0f,  0.0f,  1.0f,  0.0f,  6.0f,
        -0.15f, -4.0f,  8.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.15f,  4.0f,  8.0f, -1.0f,  0.0f,  0.0f,  6.0f,  0.0f,
        -0.15f,  4.0f, -8.0f, -1.0f,  0.0f,  0.0f,  6.0f,  6.0f,
        -0.15f, -4.0f, -8.0f, -1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
        -0.15f, -4.0f, -8.0f, -1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
        -0.15f, -4.0f,  8.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.15f,  4.0f,  8.0f, -1.0f,  0.0f,  0.0f,  6.0f,  0.0f,

         0.15f,  4.0f,  8.0f,  1.0f,  0.0f,  0.0f,  6.0f,  0.0f,
         0.15f,  4.0f, -8.0f,  1.0f,  0.0f,  0.0f,  6.0f,  6.0f,
         0.15f, -4.0f, -8.0f,  1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
         0.15f, -4.0f, -8.0f,  1.0f,  0.0f,  0.0f,  0.0f,  6.0f,
         0.15f, -4.0f,  8.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.15f,  4.0f,  8.0f,  1.0f,  0.0f,  0.0f,  6.0f,  0.0f,

        -0.15f, -4.0f, -8.0f,  0.0f, -1.0f,  0.0f,  0.0f,  6.0f,
         0.15f, -4.0f, -8.0f,  0.0f, -1.0f,  0.0f,  6.0f,  6.0f,
         0.15f, -4.0f,  8.0f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
         0.15f, -4.0f,  8.0f,  0.0f, -1.0f,  0.0f,  6.0f,  0.0f,
        -0.15f, -4.0f,  8.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.15f, -4.0f, -8.0f,  0.0f, -1.0f,  0.0f,  0.0f,  6.0f,

        -0.15f,  4.0f, -8.0f,  0.0f,  1.0f,  0.0f,  0.0f,  6.0f,
         0.15f,  4.0f, -8.0f,  0.0f,  1.0f,  0.0f,  6.0f,  6.0f,
         0.15f,  4.0f,  8.0f,  0.0f,  1.0f,  0.0f,  6.0f,  0.0f,
         0.15f,  4.0f,  8.0f,  0.0f,  1.0f,  0.0f,  6.0f,  0.0f,
        -0.15f,  4.0f,  8.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.15f,  4.0f, -8.0f,  0.0f,  1.0f,  0.0f,  0.0f,  6.0f
    };

    float tvVertices[] = {  // tv ´ë·« 40ÀÎÄ¡
        // positions          // texture Coords
        -0.1f, -0.7f, -1.11f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // ÁÂ ÇÏ // ¾Õ¸é
         0.1f, -0.7f, -1.11f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.1f,  0.7f, -1.11f, 0.0f,  0.0f, -1.0f, 1.0f, 0.4f, // ¿ì »ó
         0.1f,  0.7f, -1.11f, 0.0f,  0.0f, -1.0f, 1.0f, 0.4f, // ¿ì »ó
        -0.1f,  0.7f, -1.11f, 0.0f,  0.0f, -1.0f, 0.0f, 0.4f, // ÁÂ »ó
        -0.1f, -0.7f, -1.11f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

        -0.1f, -0.7f,  1.11f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.1f, -0.7f,  1.11f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.1f,  0.7f,  1.11f, 0.0f,  0.0f,  1.0f, 1.0f, 0.4f, // ¿ì »ó
         0.1f,  0.7f,  1.11f, 0.0f,  0.0f,  1.0f, 1.0f, 0.4f, // ¿ì »ó
        -0.1f,  0.7f,  1.11f, 0.0f,  0.0f,  1.0f, 0.0f, 0.4f, // ÁÂ »ó
        -0.1f, -0.7f,  1.11f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

        -0.1f,  0.7f,  1.11f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // ¿ì »ó // ÁÂÃø
        -0.1f,  0.7f, -1.11f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // ÁÂ »ó
        -0.1f, -0.7f, -1.11f, -1.0f,  0.0f,  0.0f, 0.0f, 0.4f, // ÁÂ ÇÏ
        -0.1f, -0.7f, -1.11f, -1.0f,  0.0f,  0.0f, 0.0f, 0.4f, // ÁÂ ÇÏ
        -0.1f, -0.7f,  1.11f, -1.0f,  0.0f,  0.0f, 1.0f, 0.4f, // ¿ì ÇÏ
        -0.1f,  0.7f,  1.11f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // ¿ì »ó

         0.1f,  0.7f,  1.11f, 1.0f,  0.0f,  0.0f, 1.0f, 0.4f, // ¿ì »ó // ¿ìÃø
         0.1f,  0.7f, -1.11f, 1.0f,  0.0f,  0.0f, 0.0f, 0.4f, // ÁÂ »ó
         0.1f, -0.7f, -1.11f, 1.0f,  0.0f,  0.0f, 0.0f, 0.4f, // ÁÂ ÇÏ
         0.1f, -0.7f, -1.11f, 1.0f,  0.0f,  0.0f, 0.0f, 0.4f, // ÁÂ ÇÏ
         0.1f, -0.7f,  1.11f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.1f,  0.7f,  1.11f, 1.0f,  0.0f,  0.0f, 1.0f, 0.4f, // ¿ì »ó

        -0.1f, -0.7f, -1.11f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
         0.1f, -0.7f, -1.11f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.1f, -0.7f,  1.11f, 0.0f, -1.0f,  0.0f, 1.0f, 0.4f, // ¿ì »ó
         0.1f, -0.7f,  1.11f, 0.0f, -1.0f,  0.0f, 1.0f, 0.4f, // ¿ì »ó
        -0.1f, -0.7f,  1.11f, 0.0f, -1.0f,  0.0f, 0.0f, 0.4f, // ÁÂ »ó
        -0.1f, -0.7f, -1.11f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ

        -0.1f,  0.7f, -1.11f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
         0.1f,  0.7f, -1.11f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.1f,  0.7f,  1.11f, 0.0f,  1.0f,  0.0f, 1.0f, 0.4f, // ¿ì »ó
         0.1f,  0.7f,  1.11f, 0.0f,  1.0f,  0.0f, 1.0f, 0.4f, // ¿ì »ó
        -0.1f,  0.7f,  1.11f, 0.0f,  1.0f,  0.0f, 0.0f, 0.4f, // ÁÂ »ó
        -0.1f,  0.7f, -1.11f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f // ÁÂ ÇÏ
    };

    float tvTableVertices[] = {  // tvTable
        // positions          // texture Coords
        -0.5f, -1.0f, -1.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -1.0f, -1.5f,  0.0f,  0.0f, -1.0f, 0.2f, 0.0f,
         0.5f,  1.0f, -1.5f,  0.0f,  0.0f, -1.0f, 0.2f, 0.2f,
         0.5f,  1.0f, -1.5f,  0.0f,  0.0f, -1.0f, 0.2f, 0.2f,
        -0.5f,  1.0f, -1.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.2f,
        -0.5f, -1.0f, -1.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -1.0f,  1.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -1.0f,  1.5f, 0.0f,  0.0f,  1.0f,  0.2f, 0.0f,
         0.5f,  1.0f,  1.5f,  0.0f,  0.0f,  1.0f, 0.2f, 0.2f,
         0.5f,  1.0f,  1.5f, 0.0f,  0.0f,  1.0f,  0.2f, 0.2f,
        -0.5f,  1.0f,  1.5f, 0.0f,  0.0f,  1.0f,  0.0f, 0.2f,
        -0.5f, -1.0f,  1.5f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  1.0f,  1.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  1.0f, -1.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -1.0f, -1.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.47f,
        -0.5f, -1.0f, -1.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.47f,
        -0.5f, -1.0f,  1.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.47f,
        -0.5f,  1.0f,  1.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

         0.5f,  1.0f,  1.5f, 1.0f,  0.0f,  0.0f, 0.2f, 0.0f,
         0.5f,  1.0f, -1.5f, 1.0f,  0.0f,  0.0f, 0.2f, 0.2f,
         0.5f, -1.0f, -1.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.2f,
         0.5f, -1.0f, -1.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.2f,
         0.5f, -1.0f,  1.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  1.0f,  1.5f,  1.0f,  0.0f,  0.0f,0.2f, 0.0f,

        -0.5f, -1.0f, -1.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.2f,
         0.5f, -1.0f, -1.5f, 0.0f, -1.0f,  0.0f, 0.2f, 0.2f,
         0.5f, -1.0f,  1.5f,  0.0f, -1.0f,  0.0f,0.2f, 0.0f,
         0.5f, -1.0f,  1.5f,  0.0f, -1.0f,  0.0f,0.2f, 0.0f,
        -0.5f, -1.0f,  1.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.2f,
        -0.5f, -1.0f, -1.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.2f,

        -0.5f,  1.0f, -1.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.2f,
         0.5f,  1.0f, -1.5f,  0.0f,  1.0f,  0.0f,  0.2f, 0.2f,
         0.5f,  1.0f,  1.5f,  0.0f,  1.0f,  0.0f,  0.2f, 0.0f,
         0.5f,  1.0f,  1.5f,  0.0f,  1.0f,  0.0f,  0.2f, 0.0f,
        -0.5f,  1.0f,  1.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  1.0f, -1.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.2f
    };

    float gshelfVertices[] = {  // gshelf W850, D450 , H1570
        // positions          // texture Coords
        -0.45f, -1.57f, -0.85f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.45f, -1.57f, -0.85f,  0.0f,  0.0f, -1.0f, 0.2f, 0.0f,
         0.45f,  1.57f, -0.85f,  0.0f,  0.0f, -1.0f, 0.2f, 0.2f,
         0.45f,  1.57f, -0.85f,  0.0f,  0.0f, -1.0f, 0.2f, 0.2f,
        -0.45f,  1.57f, -0.85f,  0.0f,  0.0f, -1.0f, 0.0f, 0.2f,
        -0.45f, -1.57f, -0.85f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.45f, -1.57f,  0.85f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.45f, -1.57f,  0.85f,  0.0f,  0.0f,  1.0f, 0.2f, 0.0f,
         0.45f,  1.57f,  0.85f,  0.0f,  0.0f,  1.0f, 0.2f, 0.2f,
         0.45f,  1.57f,  0.85f,  0.0f,  0.0f,  1.0f, 0.2f, 0.2f,
        -0.45f,  1.57f,  0.85f,  0.0f,  0.0f,  1.0f, 0.0f, 0.2f,
        -0.45f, -1.57f,  0.85f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -0.45f,  1.57f,  0.85f, -1.0f,  0.0f,  0.0f, 0.2f, 0.0f,
        -0.45f,  1.57f, -0.85f, -1.0f,  0.0f,  0.0f, 0.2f, 0.2f,
        -0.45f, -1.57f, -0.85f, -1.0f,  0.0f,  0.0f, 0.0f, 0.2f,
        -0.45f, -1.57f, -0.85f, -1.0f,  0.0f,  0.0f, 0.0f, 0.2f,
        -0.45f, -1.57f,  0.85f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.45f,  1.57f,  0.85f, -1.0f,  0.0f,  0.0f, 0.2f, 0.0f,

         0.45f,  1.57f,  0.85f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.45f,  1.57f, -0.85f, 1.0f,  0.0f,  0.0f, 0.45f, 1.0f,
         0.45f, -1.57f, -0.85f, 1.0f,  0.0f,  0.0f, 0.45f, 0.0f,
         0.45f, -1.57f, -0.85f, 1.0f,  0.0f,  0.0f, 0.45f, 0.0f,
         0.45f, -1.57f,  0.85f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.45f,  1.57f,  0.85f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

        -0.45f, -1.57f, -0.85f, 0.0f, -1.0f,  0.0f, 0.0f, 0.2f,
         0.45f, -1.57f, -0.85f, 0.0f, -1.0f,  0.0f, 0.2f, 0.2f,
         0.45f, -1.57f,  0.85f, 0.0f, -1.0f,  0.0f, 0.2f, 0.0f,
         0.45f, -1.57f,  0.85f, 0.0f, -1.0f,  0.0f, 0.2f, 0.0f,
        -0.45f, -1.57f,  0.85f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.45f, -1.57f, -0.85f, 0.0f, -1.0f,  0.0f, 0.0f, 0.2f,

        -0.45f,  1.57f, -0.85f, 0.0f,  1.0f,  0.0f, 0.0f, 0.2f,
         0.45f,  1.57f, -0.85f, 0.0f,  1.0f,  0.0f, 0.2f, 0.2f,
         0.45f,  1.57f,  0.85f, 0.0f,  1.0f,  0.0f, 0.2f, 0.0f,
         0.45f,  1.57f,  0.85f, 0.0f,  1.0f,  0.0f, 0.2f, 0.0f,
        -0.45f,  1.57f,  0.85f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.45f,  1.57f, -0.85f, 0.0f,  1.0f,  0.0f, 0.0f, 0.2f
    };

    float doorVertices[] = {  // Door Ã¢¹® ¶Õ¾î¾ßµÇ´Âµ¥..
        // positions          // texture Coords  
        -0.1f, -2.0f, -1.0f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // ÁÂ ÇÏ // ¾Õ¸é
         0.1f, -2.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.5f, 1.0f, // ¿ì ÇÏ
         0.1f,  2.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.5f, 0.0f, // ¿ì »ó
         0.1f,  2.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.5f, 0.0f, // ¿ì »ó
        -0.1f,  2.0f, -1.0f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // ÁÂ »ó
        -0.1f, -2.0f, -1.0f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // ÁÂ ÇÏ

        -0.1f, -2.0f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // ÁÂ ÇÏ // µÞ¸é
         0.1f, -2.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.5f, 1.0f, // ¿ì ÇÏ
         0.1f,  2.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.5f, 0.0f, // ¿ì »ó
         0.1f,  2.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.5f, 0.0f, // ¿ì »ó
        -0.1f,  2.0f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // ÁÂ »ó
        -0.1f, -2.0f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // ÁÂ ÇÏ

        -0.1f,  2.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.45f, 1.0f, // ¿ì »ó // ÁÂÃø
        -0.1f,  2.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // ÁÂ »ó
        -0.1f, -2.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ
        -0.1f, -2.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,// ÁÂ ÇÏ
        -0.1f, -2.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.45f, 0.0f, // ¿ì ÇÏ
        -0.1f,  2.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.45f, 1.0f, // ¿ì »ó

         0.1f,  2.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.45f, 1.0f, // ¿ì »ó // ¿ìÃø
         0.1f,  2.0f, -1.0f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // ÁÂ »ó
         0.1f, -2.0f, -1.0f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // ÁÂ ÇÏ
         0.1f, -2.0f, -1.0f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,// ÁÂ ÇÏ
         0.1f, -2.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.45f, 0.0f, // ¿ì ÇÏ
         0.1f,  2.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.45f, 1.0f, // ¿ì »ó

        -0.1f, -2.0f, -1.0f, 0.0f, -1.0f,  0.0f, 0.5f, 0.0f, // ÁÂ ÇÏ // ¾Æ·¡
         0.1f, -2.0f, -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.1f, -2.0f,  1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // ¿ì »ó
         0.1f, -2.0f,  1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // ¿ì »ó
        -0.1f, -2.0f,  1.0f, 0.0f, -1.0f,  0.0f, 0.5f, 1.0f, // ÁÂ »ó
        -0.1f, -2.0f, -1.0f, 0.0f, -1.0f,  0.0f, 0.5f, 0.0f, // ÁÂ ÇÏ

        -0.1f,  2.0f, -1.0f, 0.0f,  1.0f,  0.0f, 0.5f, 0.0f, // ÁÂ ÇÏ // À­¸é
         0.1f,  2.0f, -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // ¿ì ÇÏ
         0.1f,  2.0f,  1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // ¿ì »ó
         0.1f,  2.0f,  1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // ¿ì »ó
        -0.1f,  2.0f,  1.0f, 0.0f,  1.0f,  0.0f, 0.5f, 1.0f, // ÁÂ »ó
        -0.1f,  2.0f, -1.0f, 0.0f,  1.0f,  0.0f, 0.5f, 0.0f // ÁÂ ÇÏ

    };

    float radiatorVertices[] = {  // radiatorTable
      // positions          // texture Coords
      -0.25f, -0.8f, -1.25f,0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
       0.25f, -0.8f, -1.25f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
       0.25f,  0.8f, -1.25f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       0.25f,  0.8f, -1.25f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
      -0.25f,  0.8f, -1.25f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
      -0.25f, -0.8f, -1.25f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

      -0.25f, -0.8f,  1.25f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
       0.25f, -0.8f,  1.25f,0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       0.25f,  0.8f,  1.25f,  0.0f,  0.0f,  1.0f,1.0f, 1.0f,
       0.25f,  0.8f,  1.25f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
      -0.25f,  0.8f,  1.25f,0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
      -0.25f, -0.8f,  1.25f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

      -0.25f,  0.8f,  1.25f,-1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      -0.25f,  0.8f, -1.25f,  -1.0f,  0.0f,  0.0f,1.0f, 1.0f,
      -0.25f, -0.8f, -1.25f,-1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      -0.25f, -0.8f, -1.25f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
      -0.25f, -0.8f,  1.25f,  -1.0f,  0.0f,  0.0f,0.0f, 0.0f,
      -0.25f,  0.8f,  1.25f,-1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

       0.25f,  0.8f,  1.25f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,
       0.25f,  0.8f, -1.25f,  1.0f,  0.0f,  0.0f,1.0f, 1.0f,
       0.25f, -0.8f, -1.25f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       0.25f, -0.8f, -1.25f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       0.25f, -0.8f,  1.25f,1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       0.25f,  0.8f,  1.25f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,

      -0.25f, -0.8f, -1.25f,   0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
       0.25f, -0.8f, -1.25f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
       0.25f, -0.8f,  1.25f,   0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
       0.25f, -0.8f,  1.25f,   0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
      -0.25f, -0.8f,  1.25f,   0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
      -0.25f, -0.8f, -1.25f,   0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

      -0.25f,  0.8f, -1.25f,   0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
       0.25f,  0.8f, -1.25f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
       0.25f,  0.8f,  1.25f,   0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
       0.25f,  0.8f,  1.25f,   0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
      -0.25f,  0.8f,  1.25f, 0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
      -0.25f,  0.8f, -1.25f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    float lightvertices[] = {
        // positions          // normals           // texture coords
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

    float bmsjVertices[] = {
        // positions          // normals           // texture coords
        -0.1f, -0.6f, -1.1f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.1f, -0.6f, -1.1f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.1f,  0.6f, -1.1f,  0.0f,  0.0f, -1.0f, 1.0f, 0.4f,
         0.1f,  0.6f, -1.1f,  0.0f,  0.0f, -1.0f, 1.0f, 0.4f,
        -0.1f,  0.6f, -1.1f,  0.0f,  0.0f, -1.0f, 0.0f, 0.4f,
        -0.1f, -0.6f, -1.1f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.1f, -0.6f,  1.1f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.1f, -0.6f,  1.1f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.1f,  0.6f,  1.1f,  0.0f,  0.0f,  1.0f, 1.0f, 0.4f,
         0.1f,  0.6f,  1.1f, 0.0f,  0.0f,  1.0f,  1.0f, 0.4f,
        -0.1f,  0.6f,  1.1f, 0.0f,  0.0f,  1.0f,  0.0f, 0.4f,
        -0.1f, -0.6f,  1.1f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.1f,  0.6f,  1.1f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.1f,  0.6f, -1.1f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.1f, -0.6f, -1.1f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.5f,
        -0.1f, -0.6f, -1.1f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.5f,
        -0.1f, -0.6f,  1.1f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.5f,
        -0.1f,  0.6f,  1.1f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

         0.1f,  0.6f,  1.1f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.1f,  0.6f, -1.1f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.1f, -0.6f, -1.1f, 1.0f,  0.0f,  0.0f, 1.0f, 0.4f,
         0.1f, -0.6f, -1.1f, 1.0f,  0.0f,  0.0f, 1.0f, 0.4f,
         0.1f, -0.6f,  1.1f, 1.0f,  0.0f,  0.0f, 0.0f, 0.4f,
         0.1f,  0.6f,  1.1f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,

        -0.1f, -0.6f, -1.1f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
         0.1f, -0.6f, -1.1f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.1f, -0.6f,  1.1f,  0.0f, -1.0f, 0.0f, 1.0f, 0.4f,
         0.1f, -0.6f,  1.1f,  0.0f, -1.0f, 0.0f, 1.0f, 0.4f,
        -0.1f, -0.6f,  1.1f, 0.0f, -1.0f,  0.0f, 0.0f, 0.4f,
        -0.1f, -0.6f, -1.1f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

        -0.1f,  0.6f, -1.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         0.1f,  0.6f, -1.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.1f,  0.6f,  1.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.4f,
         0.1f,  0.6f,  1.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.4f,
        -0.1f,  0.6f,  1.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.4f,
        -0.1f,  0.6f, -1.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };

    float garvageVertices[] = {  // trash
        // positions          // texture Coords  
        -0.1f, -2.0f, -1.0f,  1.0f, 1.0f, // ÁÂ ÇÏ // ¾Õ¸é
         0.1f, -2.0f, -1.0f,  0.5f, 1.0f, // ¿ì ÇÏ
         0.1f,  2.0f, -1.0f,  0.5f, 0.0f, // ¿ì »ó
         0.1f,  2.0f, -1.0f,  0.5f, 0.0f, // ¿ì »ó
        -0.1f,  2.0f, -1.0f,  1.0f, 0.0f, // ÁÂ »ó
        -0.1f, -2.0f, -1.0f,  1.0f, 1.0f, // ÁÂ ÇÏ

        -0.1f, -2.0f,  1.0f,  1.0f, 1.0f, // ÁÂ ÇÏ // µÞ¸é
         0.1f, -2.0f,  1.0f,  0.5f, 1.0f, // ¿ì ÇÏ
         0.1f,  2.0f,  1.0f,  0.5f, 0.0f, // ¿ì »ó
         0.1f,  2.0f,  1.0f,  0.5f, 0.0f, // ¿ì »ó
        -0.1f,  2.0f,  1.0f,  1.0f, 0.0f, // ÁÂ »ó
        -0.1f, -2.0f,  1.0f,  1.0f, 1.0f, // ÁÂ ÇÏ

        -0.1f,  2.0f,  1.0f,  0.45f, 1.0f, // ¿ì »ó // ÁÂÃø
        -0.1f,  2.0f, -1.0f,  0.0f, 1.0f, // ÁÂ »ó
        -0.1f, -2.0f, -1.0f,  0.0f, 0.0f, // ÁÂ ÇÏ
        -0.1f, -2.0f, -1.0f,  0.0f, 0.0f,// ÁÂ ÇÏ
        -0.1f, -2.0f,  1.0f,  0.45f, 0.0f, // ¿ì ÇÏ
        -0.1f,  2.0f,  1.0f,  0.45f, 1.0f, // ¿ì »ó

         0.1f,  2.0f,  1.0f,  0.45f, 1.0f, // ¿ì »ó // ¿ìÃø
         0.1f,  2.0f, -1.0f,  0.0f, 1.0f, // ÁÂ »ó
         0.1f, -2.0f, -1.0f,  0.0f, 0.0f, // ÁÂ ÇÏ
         0.1f, -2.0f, -1.0f,  0.0f, 0.0f,// ÁÂ ÇÏ
         0.1f, -2.0f,  1.0f,  0.45f, 0.0f, // ¿ì ÇÏ
         0.1f,  2.0f,  1.0f,  0.45f, 1.0f, // ¿ì »ó

        -0.1f, -2.0f, -1.0f,  0.5f, 0.0f, // ÁÂ ÇÏ // ¾Æ·¡
         0.1f, -2.0f, -1.0f,  1.0f, 0.0f, // ¿ì ÇÏ
         0.1f, -2.0f,  1.0f,  1.0f, 1.0f, // ¿ì »ó
         0.1f, -2.0f,  1.0f,  1.0f, 1.0f, // ¿ì »ó
        -0.1f, -2.0f,  1.0f,  0.5f, 1.0f, // ÁÂ »ó
        -0.1f, -2.0f, -1.0f,  0.5f, 0.0f, // ÁÂ ÇÏ

        -0.1f,  2.0f, -1.0f,  0.5f, 0.0f, // ÁÂ ÇÏ // À­¸é
         0.1f,  2.0f, -1.0f,  1.0f, 0.0f, // ¿ì ÇÏ
         0.1f,  2.0f,  1.0f,  1.0f, 1.0f, // ¿ì »ó
         0.1f,  2.0f,  1.0f,  1.0f, 1.0f, // ¿ì »ó
        -0.1f,  2.0f,  1.0f,  0.5f, 1.0f, // ÁÂ »ó
        -0.1f,  2.0f, -1.0f,  0.5f, 0.0f // ÁÂ ÇÏ

    };




    // °ü¹°´ë VAO
    unsigned int gmdVAO, gmdVBO;
    glGenVertexArrays(1, &gmdVAO);
    glGenBuffers(1, &gmdVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gmdVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gmdVertices), gmdVertices, GL_STATIC_DRAW);
    glBindVertexArray(gmdVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // maru VAO
    unsigned int mrVAO, mrVBO;
    glGenVertexArrays(1, &mrVAO);
    glGenBuffers(1, &mrVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mrVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(maruVertices), maruVertices, GL_STATIC_DRAW);
    glBindVertexArray(mrVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
 
    // plane VAO  
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glBindVertexArray(planeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // wall1 VAO  
    unsigned int wall1VAO, wall1VBO;
    glGenVertexArrays(1, &wall1VAO);
    glGenBuffers(1, &wall1VBO);
    glBindBuffer(GL_ARRAY_BUFFER, wall1VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall1Vertices), &wall1Vertices, GL_STATIC_DRAW);
    glBindVertexArray(wall1VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // wall2 VAO  
    unsigned int wall2VAO, wall2VBO;
    glGenVertexArrays(1, &wall2VAO);
    glGenBuffers(1, &wall2VBO);
    glBindBuffer(GL_ARRAY_BUFFER, wall2VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall2Vertices), &wall2Vertices, GL_STATIC_DRAW);
    glBindVertexArray(wall2VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // tv VAO
    unsigned int tvVAO, tvVBO;
    glGenVertexArrays(1, &tvVAO);
    glGenBuffers(1, &tvVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tvVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tvVertices), &tvVertices, GL_STATIC_DRAW);
    glBindVertexArray(tvVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // tvTable VAO
    unsigned int tvTVAO, tvTVBO;
    glGenVertexArrays(1, &tvTVAO);
    glGenBuffers(1, &tvTVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tvTVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tvTableVertices), &tvTableVertices, GL_STATIC_DRAW);
    glBindVertexArray(tvTVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // gshelf VAO
    unsigned int gshelfVAO, gshelfVBO;
    glGenVertexArrays(1, &gshelfVAO);
    glGenBuffers(1, &gshelfVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gshelfVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gshelfVertices), &gshelfVertices, GL_STATIC_DRAW);
    glBindVertexArray(gshelfVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // door VAO
    unsigned int doorVAO, doorVBO;
    glGenVertexArrays(1, &doorVAO);
    glGenBuffers(1, &doorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), &doorVertices, GL_STATIC_DRAW);
    glBindVertexArray(doorVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // bmsj VAO
    unsigned int bmsjVAO, bmsjVBO;
    glGenVertexArrays(1, &bmsjVAO);
    glGenBuffers(1, &bmsjVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bmsjVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bmsjVertices), &bmsjVertices, GL_STATIC_DRAW);
    glBindVertexArray(bmsjVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // radiator VAO
    unsigned int radiatorVAO, radiatorVBO;
    glGenVertexArrays(1, &radiatorVAO);
    glGenBuffers(1, &radiatorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, radiatorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(radiatorVertices), &radiatorVertices, GL_STATIC_DRAW);
    glBindVertexArray(radiatorVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // garvage VAO
    unsigned int garvageVAO, garvageVBO;
    glGenVertexArrays(1, &garvageVAO);
    glGenBuffers(1, &garvageVBO);
    glBindBuffer(GL_ARRAY_BUFFER, garvageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(garvageVertices), &garvageVertices, GL_STATIC_DRAW);
    glBindVertexArray(garvageVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glGenVertexArrays(1, &lightCubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightvertices), &lightvertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load textures
    // -------------
    unsigned int gmdTexture = loadTexture("resources/textures/gmd.jpg");
    unsigned int gmdnormalTexture = loadTexture("resources/textures/gmdnormalmap.jpg");
    unsigned int maruTexture = loadTexture("resources/textures/maru.jpg");
    unsigned int floorTexture = loadTexture("resources/textures/marble.jpg");
    unsigned int wallTexture = loadTexture("resources/textures/wall.jpg");
    unsigned int wallnormalTexture = loadTexture("resources/textures/wallnormalmap.jpg");
    unsigned int tvTexture = loadTexture("resources/textures/tv.jpg");
    unsigned int tvTableTexture = loadTexture("resources/textures/tvtable.jpg");
    unsigned int gshelfTexture = loadTexture("resources/textures/gshelf.jpg");
    unsigned int doorTexture = loadTexture("resources/textures/door.jpg");
    unsigned int bmsjTexture = loadTexture("resources/textures/bmsj.jpg");
    unsigned int radiatorTexture = loadTexture("resources/textures/radiator.png");

    //  object locations
    // --------------------------------
    vector<glm::vec3> gmdObject // °ü¹°´ë 1
    {
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(4.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(-4.0f, 0.0f, 0.0f),
        glm::vec3(-5.0f, 0.0f, 0.0f)
    };

    vector<glm::vec3> gmd2Object // °ü¹°´ë 2
    {
        glm::vec3(5.0f, 0.0f, 15.0f),
        glm::vec3(4.0f, 0.0f, 15.0f),
        glm::vec3(3.0f, 0.0f, 15.0f),
        glm::vec3(2.0f, 0.0f, 15.0f),
        glm::vec3(1.0f, 0.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 15.0f),
        glm::vec3(-1.0f, 0.0f, 15.0f),
        glm::vec3(-2.0f, 0.0f, 15.0f),
        glm::vec3(-3.0f, 0.0f, 15.0f),
        glm::vec3(-4.0f, 0.0f, 15.0f),
        glm::vec3(-5.0f, 0.0f, 15.0f)
    };

    vector<glm::vec3> maruObject // ¸¶·ç 1
    {
       glm::vec3(0.0f, -2.0f, 2.0f)
    };

    vector<glm::vec3> maru2Object // ¸¶·ç 2
    {
       glm::vec3(0.0f, -2.0f, 13.0f)
    };

    vector<glm::vec3> wall11Object // wall 1-1
    {
       glm::vec3(-1.0f, 1.0f, -0.6f)
    };

    vector<glm::vec3> wall12Object // wall 1-2
    {
       glm::vec3(-1.0f, 1.0f, 15.6f)
    };

    vector<glm::vec3> wall21Object // wall 2-1
    {
       glm::vec3(-10.0f, 1.0f, 7.5f)
    };

    vector<glm::vec3> wall22Object // wall 2-2
    {
       glm::vec3(8.15f, 1.0f, 7.5f)
    };

    vector<glm::vec3> tvObject // TV
    {
       glm::vec3(7.0f, 0.3f, 7.5f)
    };
    
    vector<glm::vec3> tvTableObject //TV table
    {
       glm::vec3(7.3f, -1.5f, 7.5f)
    };
    
    vector<glm::vec3> gshelfObject // ÃÑ±âº¸°üÇÔ 1
    {
       glm::vec3(-8.5f, -0.95f, 3.0f),
       glm::vec3(-8.5f, -0.95f, 1.0f)
    };

    vector<glm::vec3> gshelf2Object // ÃÑ±âº¸°üÇÔ 2
    {
       glm::vec3(-8.5f, -0.95f, 14.0f),
       glm::vec3(-8.5f, -0.95f, 12.0f)
    };

    vector<glm::vec3> doorObject // ¹®
    {
        glm::vec3(-9.7f, -0.5f, 7.5f)
    };
   
    vector<glm::vec3> bmsjObject // º¹¹«½ÅÁ¶
    {
        glm::vec3(8.0f, 3.0f, 7.5f)
    };

    vector<glm::vec3> radiatorObject // ¶óµð¿¡ÀÌÅÍ1
    {
       glm::vec3(7.5f, -1.5f, 2.0f)
    };

    vector<glm::vec3> radiator2Object // ¶óµð¿¡ÀÌÅÍ2
    {
       glm::vec3(7.5f, -1.5f, 13.0f)
    };

    vector<glm::vec3> lightObject //ÃëÄ§µî
    {
        glm::vec3(0.0f, 5.0f, 7.5f)
    };

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setVec3("material.diffuse", 1.0f, 0.0f, 0.0f);
    lightingShader.setVec3("material.specular", 1.0f, 0.0f, 0.0f);


    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setInt("texture3", 2);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // lighting
        glm::vec3 lightPos(camera.Position);

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // material properties
        lightingShader.setFloat("material.shininess", 32.0f);

        //---------------------------------pointer light
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", camera.Position);
        lightingShader.setVec3("light.direction", camera.Front);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(11.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        lightingShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);

        lightingShader.setFloat("pointLight.constant", 1.0f);
        lightingShader.setVec3("pointLight.position", 0.0f, 5.0f, 7.5f);
        lightingShader.setFloat("pointLight.linear", 0.09f);
        lightingShader.setFloat("pointLight.quadratic", 0.032f);
        lightingShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("pointLight.specular", 0.25f, 0.25f, 0.25f);

     /*   lightingShader.setVec3("dirLight.position", 0.0f, 10.0f, 0.0f);
        lightingShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        lightingShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);*/



        // draw objects -------------------------------------------------------------------
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
      


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        lightingShader.setFloat("material.shininess", 64.0f);

        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        lightingShader.setMat4("model", model);

        // °ü¹°´ë 1
        glBindVertexArray(gmdVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gmdTexture);
        
        for (unsigned int i = 0; i < gmdObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, gmdObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // °ü¹°´ë 2
        glBindVertexArray(gmdVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gmdTexture);
        
        
        for (unsigned int i = 0; i < gmd2Object.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, gmd2Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ¸¶·ç
        glBindVertexArray(mrVAO);
        glBindTexture(GL_TEXTURE_2D, maruTexture);
        for (unsigned int i = 0; i < maruObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, maruObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ¸¶·ç 2
        glBindVertexArray(mrVAO);
        glBindTexture(GL_TEXTURE_2D, maruTexture);
        for (unsigned int i = 0; i < maruObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, maru2Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -2.0f, 7.5f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

       // wall 1-1
        glBindVertexArray(wall1VAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        for (unsigned int i = 0; i < wall11Object.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, wall11Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        } 

        // wall 1-2
        glBindVertexArray(wall1VAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        for (unsigned int i = 0; i < wall12Object.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, wall12Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // wall 2-1
        glBindVertexArray(wall2VAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        for (unsigned int i = 0; i < wall21Object.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, wall21Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // wall 2-1
        glBindVertexArray(wall2VAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        for (unsigned int i = 0; i < wall22Object.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, wall22Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // tv
        glBindVertexArray(tvVAO);
        glBindTexture(GL_TEXTURE_2D, tvTexture);
        for (unsigned int i = 0; i < tvObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, tvObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // tvtable
        glBindVertexArray(tvTVAO);
        glBindTexture(GL_TEXTURE_2D, tvTableTexture);
        for (unsigned int i = 0; i < tvTableObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, tvTableObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ÃÑ±âº¸°üÇÔ 1
        glBindVertexArray(gshelfVAO);
        glBindTexture(GL_TEXTURE_2D, gshelfTexture);
        for (unsigned int i = 0; i < gshelfObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, gshelfObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            for (unsigned int i = 0; i < gshelfObject.size(); i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, gshelfObject[i]);
                lightingShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        // ÃÑ±âº¸°üÇÔ 2
        glBindVertexArray(gshelfVAO);
        glBindTexture(GL_TEXTURE_2D, gshelfTexture);
        for (unsigned int i = 0; i < gshelf2Object.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, gshelf2Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            for (unsigned int i = 0; i < gshelf2Object.size(); i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, gshelf2Object[i]);
                lightingShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        // ¹®
        glBindVertexArray(doorVAO);
        glBindTexture(GL_TEXTURE_2D, doorTexture);
        for (unsigned int i = 0; i < doorObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, doorObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // º¹¹«½ÅÁ¶
        glBindVertexArray(bmsjVAO);
        glBindTexture(GL_TEXTURE_2D, bmsjTexture);
        for (unsigned int i = 0; i < bmsjObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, bmsjObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ¶óµð¿¡ÀÌÅÍ1
        glBindVertexArray(radiatorVAO);
        glBindTexture(GL_TEXTURE_2D, radiatorTexture);
        for (unsigned int i = 0; i < radiatorObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, radiatorObject[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ¶óµð¿¡ÀÌÅÍ2
        glBindVertexArray(radiatorVAO);
        glBindTexture(GL_TEXTURE_2D, radiatorTexture);
        for (unsigned int i = 0; i < radiator2Object.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, radiator2Object[i]);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.0f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(lightCubeVAO);
        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        for (unsigned int i = 0; i < lightObject.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightObject[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &gmdVAO);
    glDeleteVertexArrays(1, &mrVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &wall1VAO);
    glDeleteVertexArrays(1, &wall2VAO);
    glDeleteVertexArrays(1, &tvVAO);
    glDeleteVertexArrays(1, &tvTVAO);
    glDeleteVertexArrays(1, &gshelfVAO);
    glDeleteVertexArrays(1, &doorVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &bmsjVAO);
    glDeleteVertexArrays(1, &radiatorVAO);
    glDeleteBuffers(1, &gmdVBO);
    glDeleteBuffers(1, &mrVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &wall1VBO);
    glDeleteBuffers(1, &wall2VBO);
    glDeleteBuffers(1, &tvVBO);
    glDeleteBuffers(1, &tvTVBO);
    glDeleteBuffers(1, &gshelfVBO);
    glDeleteBuffers(1, &doorVBO);
    glDeleteBuffers(1, &lightCubeVBO);
    glDeleteBuffers(1, &bmsjVBO);
    glDeleteBuffers(1, &radiatorVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime/2);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime / 2);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime / 2);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime / 2);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
