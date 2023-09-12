#include <GL/glew.h>
#include <iostream>
#include "Renderer.h"


// #x, mean turn x into a string, __FILE__ get the file name, __LINE__ get the line number

// debug for opengl
// before a function (the function that we want to check)
// we use a while loop to get all the error before this function
// and then use the get error to check if this function has any error

void ClearErrors() {
    if (glGetError() != GL_NO_ERROR)
    {

    }
}

// we can also print the function name, file and line
bool GLGetError(const char* function, const char* file, unsigned int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[Error]: " << error << " Function: " << function << " File: " << file << " Line: " << line << std::endl;
        return false;
    }
    return true;
}