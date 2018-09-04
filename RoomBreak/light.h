#pragma once

#include "define.h"

class Light
{
private:
	int id;
	GLfloat position[4];	// 位置
	GLfloat ambient[4];		// 环境光
	GLfloat diffuse[4];		// 漫反射
	GLfloat specular[4];	// 镜面反射

public:
	Light(int id)
	{
		if (id > 7 || id < 0) cout << "Wrong Light ID" << endl;
		this->id = id;
		position[0] = position[1] = position[2] = 0.0f; position[3] = 1.0f;
		ambient[0] = ambient[1] = ambient[2] = 0.0f; ambient[3] = 1.0f;
		diffuse[0] = diffuse[1] = diffuse[2] = 0.0f; diffuse[3] = 1.0f;
		specular[0] = specular[1] = specular[2] = 0.0f; specular[3] = 1.0f;
	}

	// type为0时，表示(x,y,z)方向的方向光源；type为1时，表示(x,y,z)方向的点光源
	void setPostion(GLfloat x, GLfloat y, GLfloat z, GLfloat type)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z; 
		position[3] = type;
	}
	void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		ambient[0] = r;
		ambient[1] = g;
		ambient[2] = b;
		ambient[3] = a;
	}
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		diffuse[0] = r;
		diffuse[1] = g;
		diffuse[2] = b;
		diffuse[3] = a;
	}
	void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		specular[0] = r;
		specular[1] = g;
		specular[2] = b;
		specular[3] = a;
	}
	
	void draw()
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0 + id);
		// GL_LIGHTx = 0x4000 + x = GL_LIGHT0 + x
		glLightfv(GL_LIGHT0 + id, GL_POSITION, position);
		glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0 + id, GL_SPECULAR, specular);
		
	}
};