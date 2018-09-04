#pragma once
#include"define.h"
#define MAX_CHAR 128
class write
{
private:
	int size;
	bool isChinese;
	string cfont,efont;
	float color[3];
	string content;
	int position[2];
	void selectFont(int size, int charset, const char * face)
	{
		HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
			charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
		HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
		DeleteObject(hOldFont);
	}
	void drawCNString(const char* str) {
		int len, i;
		wchar_t* wstring;
		HDC hDC = wglGetCurrentDC();
		GLuint list = glGenLists(1);

		// 计算字符的个数
		// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
		// 否则一个字节算一个字符
		len = 0;
		for (i = 0; str[i] != '\0'; ++i)
		{
			if (IsDBCSLeadByte(str[i]))
				++i;
			++len;
		}

		// 将混合字符转化为宽字符
		wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
		wstring[len] = L'\0';

		// 逐个输出字符
		for (i = 0; i<len; ++i)
		{
			wglUseFontBitmapsW(hDC, wstring[i], 1, list);
			glCallList(list);
		}

		// 回收所有临时资源
		free(wstring);
		glDeleteLists(list, 1);
	}
	void drawString(const char * str)
	{
		static int isFirstCall = 1;
		static GLuint lists;

		if (isFirstCall) {
			isFirstCall = 0;
			// 申请MAX_CHAR个连续的显示列表编号  
			lists = glGenLists(MAX_CHAR);
			// 把每个字符的绘制命令都装到对应的显示列表中  
			wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
		}
		// 调用每个字符对应的显示列表，绘制每个字符  
		for (; *str != '\0'; ++str) {
			glCallList(lists + *str);
		}
	}
public:
	write() 
	{
		size = 200; 
		isChinese = false;
		efont = "Comic Sans MS";
		cfont = "华文仿宋";
		color[0] = 0;
		color[1] = 1;
		color[2] = 0;
		content = " Hello world!";
		position[0] = 100;
		position[1] = 100;
	}
	void setSize(int s)
	{
		size = s;
	}
	void setIsChinese(bool flag) 
	{
		isChinese = flag;
	}
	void setFont(string font)
	{
		if (isChinese)
			cfont = font;
		else
			efont = font;
	}
	void setColor(float i, float j, float k) 
	{
		color[0] = i;
		color[1] = j;
		color[2] = k;
	}
	void setContent(string c)
	{
		content = c;
	}
	void setPosition(int x, int y)
	{
		position[0] = x;
		position[1] = y;
	}
	void printString()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		if (isChinese)
		{
			selectFont(size, DEFAULT_CHARSET, cfont.c_str());
			glColor3f(color[0], color[1], color[2]);
			glRasterPos2i(position[0], position[1]);
			drawCNString(content.c_str());
		}
		else
		{
			selectFont(size, ANSI_CHARSET, efont.c_str());
			glColor3f(color[0], color[1], color[2]);
			glRasterPos2i(position[0], position[1]);
			drawString(content.c_str());
		}
	}
	void printString(string content, int x, int y)
	{
		this->content = content;
		position[0] = x;
		position[1] = y;
		printString();
	}
};