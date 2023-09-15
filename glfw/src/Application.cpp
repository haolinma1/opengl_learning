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
    float position[] = {
         -0.5f, -0.5f, // 0
          0.5f,  -0.5f, // 1
          0.5f,  0.5f,  // 2
          -0.5f,0.5f, // 3
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
        VertexBuffer vb(position, 4 * 2 * sizeof(float));

        // so bascially, in each vertext, we have multiple attributes like position, color, texture
        // tell opengl what the layout of our data
        // index means the index of attribute in your buffer that you want to use 
        // let say, we have position in index 0, color in index 1, texture in index 2, so the index 0 means we 
        // get the position
        // the size, means how many float that we are providing, in the position, we want to make a 2d graphic
        // hence, the size will be 2
        // normalize means if you want to trasfer the number (like the color, from 0 to 255) to floating number
        // stride means how many bytes between each vertex (since, we have many things in one vertix, like color, texture, position)
        // pointer means, in a vertext, we have so many things right, so from position to color, (let say we have two floating number to represent the position, and one number represent the color)
        // we all know that in 32-bit system, 4 bytes represent a pointer, hence to get to the pointer of that color(which is the next vertex), we need 8
        // in this case, the position is the first attribute, hence the pointer would be (const void*)0
        // if we want to get to the color, the pointer would be (const void*)8
        // for function glVertexAttribPointer

        VertexBufferLayout layout;
        VertexArray va;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);


        // create index buffer
        IndexBuffer ib(index, 6);

        Shader shader("res/shader/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);



        //enable the vertext array
        glEnableVertexAttribArray(0);

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // count means how many points we want to draw
            // must be GL_UNSIGNED_INT
            GLDebugger(shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f))
                GLDebugger(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

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