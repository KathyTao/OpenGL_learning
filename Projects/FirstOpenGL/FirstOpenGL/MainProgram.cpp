#include <glad.h>
#include <glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "DataFile.h"
#include "shader_s.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void bindingObjects(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO, float *vertices);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int width, height, nrChannels;
unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

// stores how much we're seeing of either texture
float mixValue = 0.2f;

int main()
{
	//调用glfwInit函数来初始化GLFW
	glfwInit();		
	//使用glfwWindowHint函数来配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
	//处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  //// vertex shader
  //int vertexShader = defineVertexShader(mVertexShaderSource);

  //// fragment shader
  //int fragmentShader1 = defineFragmentShader(mFragmentShaderSource1);
  //int fragmentShader2 = defineFragmentShader(mFragmentShaderSource2);
  //
  //// link shaders
  //int shaderProgram1 = createShaderProgram(vertexShader, fragmentShader1);
  //int shaderProgram2 = createShaderProgram(vertexShader, fragmentShader2);

  ////Define a vbo, vao
  //unsigned int VBO[2], VAO[2];
  ////unsigned int EBO;
  ////glGenBuffers(1, &EBO);
  //glGenBuffers(2, VBO);
  //glGenVertexArrays(2, VAO);

  //bindingObjects(VBO, VAO, firstTriangle);
  //bindingObjects(VBO + 1, VAO + 1, secondTriangle);

  ////Color vertex
  //int vertexShader = defineVertexShader(mVertexShaderSource_withColor);
  //int fragmentShader = defineFragmentShader(mFragmentShaderSource_norm);
  //int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

  Shader ourShader("../shader_3.vs", "../shader_3.fs");

  unsigned int VAO, VBO, EBO;
  bindingObjects(&VBO, &VAO, &EBO, vertices);

  // load and create a texture 
  // -------------------------
  unsigned int texture1, texture2;
  // texture 1
  // ---------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
                                          // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture1" << std::endl;
  }
  stbi_image_free(data);
  // texture 2
  // ---------
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture2" << std::endl;
  }
  stbi_image_free(data);

  // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
  // -------------------------------------------------------------------------------------------
  ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
                   // either set it manually like so:
  glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
  // or set it via the texture class
  ourShader.setInt("texture2", 1);
  glUniform1f(glGetUniformLocation(ourShader.ID, "alpha"), 0.3);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Part 1
    /*glUseProgram(shaderProgram1);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);*/

    //glUseProgram(shaderProgram2);

    //// update the uniform color
    //float timeValue = glfwGetTime();
    //float greenValue = sin(timeValue) / 2.0f + 0.5f;
    //int vertexColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
    //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    //glBindVertexArray(VAO[1]);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    
    //takes its indices from the EBO currently bound to the GL_ELEMENT_ARRAY_BUFFER target
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //part 2
    //int vertexPositionLocation = glGetUniformLocation(ourShader.ID, "shiftHorizontal");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    ourShader.use();
    //glUniform3f(vertexPositionLocation, 0.5f, 0.0f, 0.0f);
    glUniform1f(glGetUniformLocation(ourShader.ID, "alpha"), mixValue);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Part 1
  /*glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);*/
 
  //Part 2
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
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
    if (mixValue >= 1.0f)
      mixValue = 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
    if (mixValue <= 0.0f)
      mixValue = 0.0f;
  }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void bindingObjects(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO, float vertices[])
{
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);
  glGenVertexArrays(1, VAO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  // 2. copy our vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, 32*sizeof(float), vertices, GL_STATIC_DRAW);

  // 3. copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // 4. then set the vertex attributes pointers
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}
