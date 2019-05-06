/****************************************************************************

 Copyright (C) 2002-2014 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.7.0.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

// #include <GL/glew.h>
// #include <GL/glut.h>
#include <gl.h>
#include <glu.h>
#include <glut.h>


#include "simpleViewer.h"
#include "textfile.h"


using namespace std;

GLuint vertexShader, fragmentShader, geoShader, shaderProgram;
float lpos[4] = {1,0.5,1,0};

bool wireframe = false;

GLint numSides = 3;  
GLint numControlPoints = 3;

float rootRadius = 0.2f;
float tipRadius = 0.2f;


const int numPoints = 2;
const GLfloat segments[numPoints][4] = {
    {  0.0,  0.0, 0.0, 1.0  }, 
    {  1.0,  0.0, 0.0, 1.0  }
    // {  2.0,  0.0, 0.0, 1.0  }, 
    // {  3.0,  0.0, 0.0, 1.0  } 
}; 

const GLfloat colors[numPoints][3] = {
    {  1.0,  0.0, 0.0  }, 
    {  0.0,  1.0, 0.0  }
    // {  0.0,  0.0, 1.0  }, 
    // {  0.0,  1.0, 1.0  } 
}; 

GLfloat lengths[numPoints] = {
      1.0 , 
      1.2 
    //   1.4 , 
    //   1.6 
}; 

const GLfloat original_lengths[numPoints] = { 
      1.0 , 
      1.2 
    //   1.4 , 
    //   1.6 
}; 



GLuint vao, vbo[3];

Viewer::Viewer(QWidget *parent) : QGLViewer(parent)
{
    

}

// Draws a spiral
void Viewer::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    GLfloat matrix[16]; 
    camera()->getModelViewProjectionMatrix(matrix);
    glUseProgram(shaderProgram);
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0]);

    GLuint numSidesLocation = glGetUniformLocation(shaderProgram, "numSides");
    glUniform1iv(numSidesLocation, 1, &numSides);

    GLuint numControlPointsLocation = glGetUniformLocation(shaderProgram, "numControlPoints");
    glUniform1iv(numControlPointsLocation, 1, &numControlPoints);

    GLuint rootRadiusLocation = glGetUniformLocation(shaderProgram, "rootRadius");
    glUniform1fv(rootRadiusLocation, 1, &rootRadius);

    GLuint tipRadiusLocation = glGetUniformLocation(shaderProgram, "tipRadius");
    glUniform1fv(tipRadiusLocation, 1, &tipRadius);

    glPointSize(10);  

    if(wireframe)    
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    drawAxis();
  
    glDrawArrays(GL_POINTS, 0, numPoints);      

	glutSwapBuffers();                
}

void Viewer::init() {
    // Restore previous viewer state.
    restoreStateFromFile();

    if (GL_ARB_vertex_shader && GL_ARB_fragment_shader && GL_EXT_geometry_shader4) 
    {
		printf("Ready for GLSL\n");
        printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    }
	else {
		printf("No GLSL support\n");
		exit(1);
	}

	setShaders();  
}

void Viewer::setShaders() 
{

	char *vertexScript = NULL, *fragmentScript = NULL, *geoScript = NULL;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    geoShader = glCreateShader(GL_GEOMETRY_SHADER);
 

	// vertexScript = textFileRead("/Users/sarac.schvartzman/Dropbox/Code/FurShader/shaders/minimal120.vert");
	// fragmentScript = textFileRead("/Users/sarac.schvartzman/Dropbox/Code/FurShader/shaders/minimal120.frag");
    // geoScript = textFileRead("/Users/sarac.schvartzman/Dropbox/Code/FurShader/shaders/minimal120.geom");

    vertexScript = textFileRead("/Users/sarac.schvartzman/Dropbox/Code/FurShader/shaders/minimal330.vert");
	fragmentScript = textFileRead("/Users/sarac.schvartzman/Dropbox/Code/FurShader/shaders/minimal330.frag");
    geoScript = textFileRead("/Users/sarac.schvartzman/Dropbox/Code/FurShader/shaders/minimal330.geom");

	glShaderSource(vertexShader, 1, &vertexScript, NULL);
	glShaderSource(fragmentShader, 1, &fragmentScript, NULL);
    glShaderSource(geoShader, 1, &geoScript, NULL);

	free(vertexScript); 
    free(fragmentScript); 
    free(geoScript);


    {
        int success;
        char infoLog[512];
        glCompileShader(vertexShader);
        // check for shader compile errors
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER::VERTEX::COMPILATION \n" << infoLog << std::endl;
    }

	{
        int success;
        char infoLog[512];
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "SHADER::FRAGMENT::COMPILATION \n" << infoLog << std::endl;
    }

    {
        int success;
        char infoLog[512];
        glCompileShader(geoShader);
        // check for shader compile errors
        glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
        std::cout << "SHADER::GEOMETRY::COMPILATION \n" << infoLog << std::endl;
    }

    
	shaderProgram = glCreateProgram();
    
	glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geoShader);
	glAttachShader(shaderProgram, fragmentShader);
    

    //
    {
        /* Allocate and assign a Vertex Array Object to our handle */
        glGenVertexArrays(1, &vao);

        /* Bind our Vertex Array Object as the current used object */
        glBindVertexArray(vao);

        /* Allocate and assign three Vertex Buffer Objects to our handle */
        glGenBuffers(3, vbo);

        // Positions
        {
            /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

            /* Copy the vertex data from diamond to our buffer */
            /* 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values */
            glBufferData(GL_ARRAY_BUFFER, numPoints*4 * sizeof(GLfloat), segments, GL_STATIC_DRAW);

            /* Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex */
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

            /* Enable attribute index 0 as being used */
            glEnableVertexAttribArray(0);
        }

        // Colors
        {
            /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

            /* Copy the color data from colors to our buffer */
            /* 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values */
            glBufferData(GL_ARRAY_BUFFER, numPoints*3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

            /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            /* Enable attribute index 1 as being used */
            glEnableVertexAttribArray(1);
        }

        // Lengths
        {
            /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
            glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

            /* Copy the length data from lengths to our buffer */
            /* 4 * sizeof(GLfloat) is the size of the lengths array, since it contains 4 GLfloat values */
            glBufferData(GL_ARRAY_BUFFER, numPoints * sizeof(GLfloat), lengths, GL_STREAM_DRAW);

            /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);

            /* Enable attribute index 1 as being used */
            glEnableVertexAttribArray(2);
        }

    }
 
    glLinkProgram(shaderProgram);

    {
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "SHADER::PROGRAM::LINKING \n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geoShader);

	glUseProgram(shaderProgram);

    update();
}

void Viewer::changeLength(int value)
{
    const float mult = (value * 2.f / 100);
    for (size_t i = 0; i < numPoints; i++)
    {
        lengths[i] = original_lengths[i] * mult;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numPoints*sizeof(GLfloat), lengths );

    update();
}

void Viewer::changeRootRadius(int value)
{
    rootRadius = (value * 0.5 / 100);
    update();
}

void Viewer::changeTipRadius(int value)
{
    tipRadius = (value * 0.5 / 100);
    update();
}

void Viewer::changeSides(int value)
{
    numSides = value;
    
    update();
}

void Viewer::changeNumberControlPoints(int value)
{
    numControlPoints = value;
    
    update();
}

void Viewer::setWireframe(bool value)
{
    wireframe = value;
    
    update();
}