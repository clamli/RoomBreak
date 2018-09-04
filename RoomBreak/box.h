#pragma once

#include "define.h"
#include "texture.h"

extern int frameH;
extern int frameW;

class Box
{
private:
	double width;
	double height;
	double left;
	double bottom;
	GLuint textureID_default;
	GLuint textureID_object;
	GLuint textureID_object_click;
	GLuint textureID;
	bool enable;
	int type;

	GLuint setTexture(string texturePath)
	{
		Texture tex = Texture(texturePath);
		return tex.getID();
	}

public:
	Box() {}
	Box(double width, double height, double left, double bottom, string texturePath_default, string texturePath_object, string texturePath_object_click, bool o_enable)
	{
		this->width = width;
		this->height = height;
		this->left = left;
		this->bottom = bottom;
		this->enable = o_enable;
		textureID_default = setTexture(texturePath_default);
		if (texturePath_object_click != "")
			textureID_object = setTexture(texturePath_object);
		if (texturePath_object_click != "")
			textureID_object_click = setTexture(texturePath_object_click);
		textureID = textureID_default;
		this->type = BOX_DEFAULT;
	}

	int getType()
	{
		return type;
	}

	void setEnable()
	{
		enable = true;
	}

	void disEnable()
	{
		enable = false;
	}

	void setTextureID(string texturePath_default)
	{
		textureID_default = setTexture(texturePath_default);
		textureID = textureID_default;
	}

	void draw()
	{
		if (enable)
		{
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glBindTexture(GL_TEXTURE_2D, textureID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(1.0, 1.0);		glVertex2f(left + width, bottom + height);
				glTexCoord2f(1.0, 0.0);		glVertex2f(left + width, bottom);
				glTexCoord2f(0.0, 0.0);		glVertex2f(left, bottom);
				glTexCoord2f(0.0, 1.0);		glVertex2f(left, bottom + height);
			}
			glEnd();
		}
	}

	void setObject()
	{
		type = BOX_OBJECT;
		textureID = textureID_object;
	}

	void setDefault()
	{
		type = BOX_DEFAULT;
		textureID = textureID_default;
	}

	bool click(int x, int y, int state)
	{
		bool flag = false;
		if (state == ITEM)
		{
			if (1.0*x / frameW > left && 1.0*x / frameW < (left + width) && 1.0*(frameH - y) / frameH > bottom && 1.0*(frameH - y) / frameH < (bottom + height))
			{
				if (type == BOX_DEFAULT) return 1;
				else if (type == BOX_OBJECT)
				{
					type = BOX_OBJECT_CLICK;
					textureID = textureID_object_click;
				}
				else if (type == BOX_OBJECT_CLICK)
				{
					type = BOX_OBJECT;
					textureID = textureID_object;
				}
				flag = true;
			}
		}
		else if (state == PASSWORD)
		{
			if (1.0*x / frameW > left && 1.0*x / frameW < (left + width) && 1.0*(frameH - y) / frameH > bottom && 1.0*(frameH - y) / frameH < (bottom + height))
				flag = true;
		}
		return flag;
	}
};