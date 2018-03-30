#pragma once

extern int success;
extern char infoLog[512];
extern int defineVertexShader(const char *vertexShaderSource);
extern int defineFragmentShader(const char *fragmentShaderSource);
extern int createShaderProgram(int vertexShader, int fragmentShader);