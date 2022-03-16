#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "math.h"
#include "primitives/Shader.h"
#include "stb_image/stb_image.h"


#define SCREEN_RES_MULTIPLIER 1

// Window width and height
const int windowWidth = 800;
const int windowHeight = 600;

// This method allows for GLFW to resize the window whenever the user drags the corners
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Set viewport again
}

// This method processes input, specifically the esc key being pressed
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // Makes it so window closes with esc
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {

    // Initialize GLFW for it to properly work
    glfwInit();

    /* The glfwWindowHint function allows us to set up different settings for the window object before creating it.
     * the first parameter is the option we would like to change, and the second one is the value we are setting it to. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We are setting major version to 3 (GLFW 3.0)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Setting minor version to 3 (so now GLFW 3.3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Applying core profile for more functionality

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS necessary line.
#undef SCREEN_RES_MULTIPLIER
#define SCREEN_RES_MULTIPLIER 2
#endif

    // Create the window object
    GLFWwindow* window = glfwCreateWindow(windowWidth,windowHeight,"Psuedo Window",nullptr,nullptr);
    if (window == nullptr) { // If init fails terminate glfw and finish program with exit code -1
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // This method makes the window the current context, otherwise the window is not used.
    glfwMakeContextCurrent(window);

    // Now we initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set actual viewport dimension
    // Takes 4 parameters, left-x, bottom-y, right-x, top-y. y=0,x=0 is the bottom left corner of the viewport.
    glViewport(0, 0, windowWidth*SCREEN_RES_MULTIPLIER, windowHeight*SCREEN_RES_MULTIPLIER); // Used for mapping from -1 to 1 to the actual render size.

    // Tell GLFW that we have a viewportResize callback function, in case the event occurs it can resize it.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* IMPORTANT: Any other callback function that needs to be registered has to happen between the window creation
     * and before the render loop */

    // Vertex Data For Object
    // =========================================================
//    float vertices[] = { // This are the 3D coordinates for a triangle in NDC (Normalized Device Coordinates -1 to 1)
//            -0.5f, -0.5f, 0.0f, //x1, y1, z1
//            0.5f, -0.5f, 0.0f,  //x2, y2, z2
//            0.0f,  0.5f, 0.0f   //x3, y3, z3
//    };

    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3
    };

    // This creates a vertex buffer object
    unsigned int VBO, VAO, EBO; // Vertex Buffer Object, Vertex Array Object, Element Buffer Object
    glGenVertexArrays(1, &VAO); // Generate a Vertex Array Object to store all those settings below for this particular VBO
    glGenBuffers(1, &VBO); // This methods binds our var to the VBO created behind the scenes
    glGenBuffers(1, &EBO); // Creates EBO and returns ID

    // Bind VAO so it registers EBO and VBO when doing changes
    glBindVertexArray(VAO); // Its important to first bind the VAO so VBO registers in it

    // Bind VBO and fill it with data
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds it to GL_ARRAY_BUFFER, so any further operations on it are on VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Fills VBO with our data

    // Bind EBO and fill it with data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Binds the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Fills the buffer with the corresponding data

    // Tell openGL gow to interpret our VBO ------------

    // First argument specifies the index (location=0) in our shader so we use 0
    // Second argument is the length of the value location is a vec3 so 3
    // Third is the type of the data
    // Fourth is a setting that clamps values between -1 and 1 or 0 and 1 for unsigned.
    // Fifth: Stride, the length between each distinct value (x i i x i i x i i) <- for that it would be 3
    // Sixth: Starting offset for the value (i i x) <- offset would be 2 as it starts later
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Set interpretation of VBO
    glEnableVertexAttribArray(0); // Enables the attribute number (Defined in the vertex shader as (location=0)

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Loading an Image
    // =========================================================
    // Read image data using stb library
    int imgWidth, imgHeight, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../../OpenGL/resources/images/PseudoElephant.png", &imgWidth, &imgHeight, &nChannels, 0);

    // Create texture with opengl, store its ID
    unsigned int texture;
    glGenTextures(1, &texture);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) { // Error check
        // Generate the texture using the data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Free image data from the stb library
    stbi_image_free(data);

    // Render Loop
    // =========================================================
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set wireframe mode

    Shader basicShader("../../OpenGL/resources/shaders/Default.shader");
    basicShader.use();

    while(!glfwWindowShouldClose(window)) // Checks if the window has been instructed to close, if true loop terminates.
    {
        // Manage input
        processInput(window);

        // Render commands ...
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f); // Paints it red
        glClear(GL_COLOR_BUFFER_BIT);

        basicShader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Manage events and swap buffers
        glfwSwapBuffers(window); // Applies a double buffer for a crisp image.
        glfwPollEvents(); // Checks for events that may occur during runtime.
    }

    // Clean additional resources
    // =========================================================
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    //Once loop is done we want to properly terminate and remove resources (clean memory and shit).
    glfwTerminate(); // This function does exactly that ^^^

    return 0;
}