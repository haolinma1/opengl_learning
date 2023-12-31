#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(700, 500, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "there are some error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    // add the texture coordiante for the texture to be draw
    // Texture coordinates specify the point in the texture image
    // that will correspond to the vertex you are specifying them for
    float position[] = {
         -0.5f, -0.5f, 0.0f, 0.0f, // 0
          0.5f, -0.5f, 1.0f, 0.0f,// 1
          0.5f,  0.5f, 1.0f,1.0f, // 2
          -0.5f,0.5f, 0.0f,1.0f// 3
    };

    // delete the duplicate points
    // index buffer, store the points of how the opengl will draw our digram
    // and add the index buffer
    // !! the type must be unsigned integer
    // look at the position, if we want to draw a square,
    // the first triangle should be points 0,1,2
    // the second triangle should be 2,3,1
    unsigned int index[] = {
        0,1,2,
        2,3,0
    };

   


        // create a vertex buffer
    {
        VertexBuffer vb(position, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        VertexArray va;
        // for vertex itself
        layout.Push<float>(2);
        // this is for texture layout
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);


        // create index buffer
        IndexBuffer ib(index, 6);

        Shader shader("res/shader/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        Texture texture("res/Texture/sample2.png");
        texture.Bind();
        // 0 stands for the slot, which have the same number as texture.Bind()
        shader.SetUniform1i("u_Texture", 0);
        


        //enable the vertext array
        glEnableVertexAttribArray(0);

        float r = 0.0f;
        float increment = 0.05f;
        Renderer render;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            render.Clear();

            // count means how many points we want to draw
            // must be GL_UNSIGNED_INT
          
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            render.Draw(ib, va, shader);

            if (r > 1.0)
            {
                increment = -0.05;
            }
            else if (r < 0.0)
            {
                increment = 0.05;
            }
            r += increment;


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    } // need to have this scope because after the glfwTerminate has been invoke, the opengl context will be destoryed
    // and when the program delete the buffer, our glDebugger will return a false message.
    glfwTerminate();
    return 0;
}