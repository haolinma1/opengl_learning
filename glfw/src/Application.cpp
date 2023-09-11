#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

static void ClearErrors(){
    if (glGetError()!=GL_NO_ERROR)
    {

    }
}

// we can also print the function name, file and line
static bool GLGetError(const char* function,const char* file,unsigned int line) {
    while (GLenum error=glGetError())
    {
        std::cout << "[Error]: " << error <<" Function: "<<function<<" File: "<<file<<" Line: "<<line << std::endl;
        return false;
    }
    return true;
}

// to return two string from the function getShader
struct ShaderProgramSource {
    std::string vertex;
    std::string fragment;
};


// read the shader source from the path
static ShaderProgramSource getShader(const std::string& ShaderPath) {
    std::ifstream readfile(ShaderPath);
    std::string line;

    enum class Shadertype {
        none = -1, vertex=0,fragment=1
    };

    std::stringstream ss[2];
    Shadertype type = Shadertype::none;
    while (getline(readfile,line))
    {   
        // find the type of shader
        if (line.find("#shader")!=std::string::npos)
        {
            if (line.find("vertex")!=std::string::npos)
            {
                // find the vertex
                type = Shadertype::vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // find the fragment
                type = Shadertype::fragment;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    // return a struct
    return { ss[0].str(),ss[1].str() };
}



static unsigned int complieShader(unsigned int type, const std::string& source) {
    // glCreateShader creates an empty shader object and returns a non-zero value by which it can be referenced
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // count means that how many string that you want to pass to this function
    // src means the address of the array of the string
    // NULL means that we don't have the array, hence just one string
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);

    // TODO handle error
    int result;
    glGetShaderiv(id, GL_LINK_STATUS, &result);
    if (result==GL_FALSE)
    {
        // get the length of error message first
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Fail to complie: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }


    return id;


}

static unsigned int creatShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // create an empty program object and return a non-zero number that we can use it to reference
    unsigned int program = glCreateProgram();

    unsigned int vs = complieShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = complieShader(GL_FRAGMENT_SHADER,fragmentShader);

    // attach shader to our program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


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
    unsigned int IndexBuffer[] = {
        0,1,2,
        2,3,0
    };

    // vertex buffer, is used to store the vertex data in an array and render them
    unsigned int vbo;
    GLDebugger(glGenVertexArrays(1,&vbo));
    GLDebugger(glBindVertexArray(vbo);)
        

    unsigned int buffer;
    // generate buffer for gpu to work with
    // the buffer means the id that represent this buffer.
    // so we can just pass in the integet buffer that represent the buffer that we are going to generate
    glGenBuffers(1, &buffer);
    // select the buffer that we have created
    // this is the array of buffer, buffer is the id.
    // GL_ARRAY_BUFFER target means that binding the buffer for vertex attribute data
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // insert the data to this buffer.
    // opengl know which buffer it need to insert the data by the first argument, which is GL_ARRAY_BUFFER 
    glBufferData(GL_ARRAY_BUFFER, 4 *2* sizeof(float), position, GL_STATIC_DRAW);

    // so bascially, in each vertext, we have multiple attributes like position, color, texture
    // tell opengl what the layout of our data
    // index means the index of attribute in your buffer that you want to use 
    // let say, we have position in index 0, color in index 1, texture in index 2, so the index 0 means we 
    // get the position
    // the size, means how many float that we are providing, in the position, we want to make a 2d graphic
    // hence, the size will be 2
    // normalize means if you want to trasfer the number (like the color, from 0 to 255) to floating number
    // stride means how many bytes between each vertext (since, we have many things in one vertix, like color, texture, position)
    // pointer means, in a vertext, we have so many things right, so from position to color, (let say we have two floating number to represent the position, and one number represent the color)
    // we all know that in 32-bit system, 4 bytes represent a pointer, hence to get to the pointer of that color, we need 8
    // in this case, the position is the first attribute, hence the pointer would be (const void*)0
    // if we want to get to the color, the pointer would be (const void*)8
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    // get a new buffer for the index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    // GL_ELEMENT_ARRAY_BUFFER target is used for index data that defines the order in which vertices are rendered 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), IndexBuffer, GL_STATIC_DRAW);
    
    // initialze the two shader, vertex and fragment shader
    ShaderProgramSource shaderFile = getShader("res/shader/Basic.shader");
    
    unsigned int shader = creatShader(shaderFile.vertex, shaderFile.fragment);
    glUseProgram(shader);

    // Uniform:
    // must be after the glUserProgram
    // need to get the location of that uniform first
    GLDebugger(int location = glGetUniformLocation(shader, "u_Color"));
    // 4 stands for how many floating number
    // f stands for floating number
    GLDebugger(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));




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
        GLDebugger(glUniform4f(location, r, 0.3f, 0.8f, 1.0f))
        GLDebugger(glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL));
        
        if (r>1.0)
        {
            increment = -0.05;
        }
        else if (r<0.0)
        {
            increment = 0.05;
        }
        r += increment;


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}