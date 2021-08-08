#ifndef HELPFILE_H

#define HELPFILE_H

#include <SFML/OpenGL.hpp>
float PI = 3.141592;
 int mas[256][256][256];
float size = 20.0f;


void createBox(GLuint box[], float size) {

	glBindTexture(GL_TEXTURE_2D, box[0]);
	glBegin(GL_QUADS);
	{
		//back
		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
		glTexCoord2f(1, 1); glVertex3f(size, size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size, size, -size);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[1]);
	glBegin(GL_QUADS);
	{
		//front
		glTexCoord2f(0, 0); glVertex3f(size, -size, size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
		glTexCoord2f(1, 1); glVertex3f(-size, size, size);
		glTexCoord2f(0, 1); glVertex3f(size, size, size);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[2]);
	glBegin(GL_QUADS);
	{
		//left
		glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size, size, size);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[3]);
	glBegin(GL_QUADS);
	{
		//right
		glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
		glTexCoord2f(1, 0); glVertex3f(size, -size, size);
		glTexCoord2f(1, 1); glVertex3f(size, size, size);
		glTexCoord2f(0, 1); glVertex3f(size, size, -size);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[4]);
	glBegin(GL_QUADS);
	{
		//bottom
		glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
		glTexCoord2f(1, 0); glVertex3f(size, -size, size);
		glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, box[5]);
	glBegin(GL_QUADS);
	{
		//top
		glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 0); glVertex3f(size, size, -size);
		glTexCoord2f(1, 1); glVertex3f(size, size, size);
		glTexCoord2f(0, 1); glVertex3f(-size, size, size);
	}
	glEnd();
}



bool check(int x, int y, int z) {
	if ((x < 0) || (x >= 1000) || (y < 0) || (y >= 1000) || (z < 0) || (z >= 1000)) {
		return false;
	}
	return mas[x][y][z];
}


#endif HELPFILE_H