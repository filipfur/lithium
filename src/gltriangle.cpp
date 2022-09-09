#include "gltriangle.h"
#include <glm/gtc/type_ptr.hpp>

mygl::Triangle::Triangle(mygl::ShaderProgram* shaderProgram) : _shaderProgram{shaderProgram}
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
	   -1.0f, -1.0f, 0.0f,
	   1.0f, -1.0f, 0.0f,
	   0.0f,  1.0f, 0.0f,
	};

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  */
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &_VBO);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

mygl::Triangle::~Triangle() noexcept
{

}

void mygl::Triangle::draw(mygl::Camera* camera)
{
	_shaderProgram->use();
    glUniformMatrix4fv(glGetUniformLocation(_shaderProgram->id(), "u_camera"), 1, GL_FALSE, glm::value_ptr(camera->matrix()));
    // 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}