#include <glad.h>
#include <glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//GLSL
const char *mVertexShaderSource = "#version 430 core\n"
"layout (location = 0) in vec3 aPos;\n"           // the position variable has attribute position 0
"out vec4 vertexColor; "   // specify a color output to the fragment shader
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"  
"}\0";

const char *mVertexShaderSource_withColor = "#version 430 core\n"
"layout(location = 0) in vec3 aPos;"   // the position variable has attribute position 0
"layout(location = 1) in vec3 aColor;" // the color variable has attribute position 1

"out vec3 ourColor;"                  // output a color to the fragment shader
"void main()\n"
"{\n"
"  gl_Position = vec4(aPos, 1.0);\n"
"  ourColor = aColor;\n"             // set ourColor to the input color we got from the vertex data
"}\n\0";



const char *mFragmentShaderSource1 = "#version 430 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;"   // the input variable from the vertex shader (same name and same type)
"}\n\0";

const char *mFragmentShaderSource2 = "#version 430 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor; \n"     // we set this variable in the OpenGL code.
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

const char *mFragmentShaderSource_norm = "#version 430 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor; \n"     // we set this variable in the OpenGL code.
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

//element array mode
//float vertices[] = {
//  0.5f,  0.5f, 0.0f,  // top right
//  0.5f, -0.5f, 0.0f,  // bottom right
//  -0.5f, -0.5f, 0.0f,  // bottom left
//  -0.5f,  0.5f, 0.0f   // top left 
//
//};

float vertices[] = {
  // positions         // colors
  0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
  -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
  0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

//No EBO
float firstTriangle[] = {
  //first triangle
  0.5f,  0.5f, 0.0f,  // top right
  0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f,  0.5f, 0.0f,   // top left 
};
float secondTriangle[] = {
  //second triangle
  0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f, -0.5f, 0.0f,  // bottom left
  -0.5f,  0.5f, 0.0f   // top left 

};

//Same triangle data
float vertices_same[] = {
  //first triangle
  0.5f,  0.5f, 0.0f,  // top right
  0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f,  0.5f, 0.0f,   // top left 
  //second triangle
  0.5f,  0.5f, 0.0f,  // top right
  0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f,  0.5f, 0.0f,   // top left 
};

unsigned int indices[] = {  // note that we start from 0!
  0, 1, 3,   // first triangle
  1, 2, 3    // second triangle
};