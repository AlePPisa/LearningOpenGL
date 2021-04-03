//
// Created by Ale Pisa on 2021-04-02.
//

// Vertex Shader ----------------
unsigned int vertexShader; // Holds ID for vertex shader
vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates vertex behind the scenes and returns ID
glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr); // We pass the source code to the shader
glCompileShader(vertexShader); // Compile the shader

// Check if the shader compiled properly
int  success;
char infoLog[512]; // Var for error message, if any
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Queues to check for compile status

if(!success) { // If failed to compile we load error message and display it
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}

// Fragment shader -------------
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
glCompileShader(fragmentShader);

// Check to see if it compiled properly
glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // Reusing vars

if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
}

// Shader Program Object -------------
// All the smaller shaders linked into one large shader program.
unsigned int shaderProgram; // Will hold the ID for the shaderProgram
shaderProgram = glCreateProgram(); // Inits and returns ID

// Attach our shaders to the shader program
glAttachShader(shaderProgram, vertexShader); // Adds vertex shader
glAttachShader(shaderProgram, fragmentShader); // Adds fragment shader
glLinkProgram(shaderProgram); // Links all shaders together

// NOTE: We don't need to provide the other shaders as there are default versions of those

// Check for successful linking of shader program
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
}

// Set up the program and delete shaders since we don't need them anymore
glUseProgram(shaderProgram); // Set the program
glDeleteShader(vertexShader); // Delete vertex shader
glDeleteShader(fragmentShader); // Delete fragment shader
