#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
// for debug
// make a macro of asser to wrap both of the function ClearErrors and GLGetError so that the program will be terminate

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLDebugger(x) ClearErrors();\
x;\
ASSERT(GLGetError(#x,__FILE__,__LINE__))

// #x, mean turn x into a string, __FILE__ get the file name, __LINE__ get the line number

// debug for opengl
// before a function (the function that we want to check)
// we use a while loop to get all the error before this function
// and then use the get error to check if this function has any error

 void ClearErrors();

// we can also print the function name, file and line
 bool GLGetError(const char* function, const char* file, unsigned int line);


 class Renderer
 {
 public:
	 Renderer();
	 ~Renderer();
	 // we need index buffer, vertex array and shader, we don't need to worry about vertex buffer
	 // because vertex buffer is binded to vertex array
	 void Draw(const IndexBuffer& ib,const VertexArray& va,const Shader& shader) const;
	 void Clear()const;

 private:

 };


