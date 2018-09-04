#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cstdio>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <gl/glut.h>
#include "define.h"
#include "texture.h"
using namespace std;
typedef struct
{
	float x;
	float y;
	float z;
} VERTEXF;
typedef struct
{
	double x;
	double y;
	double z;
} VERTEXD;

class Person
{
	int boarderX, boarderY, boarderZ;	// 边缘抽象成长方体
	VERTEXF lookatPoint;					// 视点
	int nearZ, farZ;						// 视域
	int angle;							// 视角
};

class Material
{
public:
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	unsigned map;

	Material(float *a, float *d, float *s, float *e, unsigned m)
	{
		ambient[0] = *a;
		ambient[1] = *(a + 1);
		ambient[2] = *(a + 2);
		ambient[3] = 0;
		diffuse[0] = *d;
		diffuse[1] = *(d + 1);
		diffuse[2] = *(d + 2);
		diffuse[3] = 0;
		specular[0] = *s;
		specular[1] = *(s + 1);
		specular[2] = *(s + 2);
		specular[3] = 0;
		emission[0] = *e;
		emission[1] = *(e + 1);
		emission[2] = *(e + 2);
		emission[3] = 0;
		map = m;
	}
public:
	Material()
	{
		ambient[0] = 0;
		ambient[1] = 0;
		ambient[2] = 0;
		ambient[3] = 0;
		diffuse[0] = 1.0;
		diffuse[1] = 1.0;
		diffuse[2] = 1.0;
		diffuse[3] = 0;
		specular[0] = 1.0;
		specular[1] = 1.0;
		specular[2] = 1.0;
		specular[3] = 0;
		emission[0] = 0;
		emission[1] = 0;
		emission[2] = 0;
		emission[3] = 0;
		map = 0;
	}
};

class Object
{
	string objname;                         //对象名字
	string matname;						    // 材料名称
											// int row,col;							// 面的属性数量、面的顶点数量
	Material material;                      // 材料

public:
	typedef std::vector<VERTEXF>::iterator viter;
	typedef std::vector< std::pair<float, float> >::iterator titer;
	typedef std::vector<VERTEXF>::iterator niter;
	typedef std::vector<int>::iterator fiter;
	vector<VERTEXF> vertexs;                // 点
	vector<pair<float, float> > texcoords;	// 材质
	vector<VERTEXF> normals;					// 法向量
	vector<int> faces;						// 面
	int row, col;
	int vertexsPerFace;
	int attrPerFace;

public:
	Object(viter vbeg, viter vend, titer tbeg, titer tend, niter nbeg, niter nend, fiter fbeg, fiter fend, int r, int c, string mat)
		:vertexs(vbeg, vend), texcoords(tbeg, tend), normals(nbeg, nend), faces(fbeg, fend), row(r), col(c), matname(mat) {}
public:
	Object() {};
public:
	void setName(string str)
	{
		objname = str;
	}
public:
	string getMatName()
	{
		return matname;
	}
public:
	void setMaterial(Material mat)
	{
		material = mat;
	}
public:
	Material getMaterial()
	{
		return material;
	}
	//void draw(int x, int y, int z, int rx, int ry, int rz);
};

class Group
{
	vector<Object> object;
	map<string, Material> material;
	int boarderX, boarderY, boarderZ;	// 边缘抽象成长方体

public:
	Group(string &cd, string filepath)
	{
		ifstream in;
		vector<VERTEXF> vertexs;
		vector< pair<float, float> > texcoords;
		vector<VERTEXF> normals;
		vector<int> faces;
		int row = 0, col = 0;
		string line, word, goname, mtlname;
		char Buffer[MAX_PATH];

		if (filepath.find(":") != string::npos)
		{
			cd = string(filepath.begin(), filepath.begin() + filepath.rfind("\\"));
		}
		else if (startswith(filepath, string(".\\")))
		{
			GetCurrentDirectoryA(MAX_PATH, Buffer);
			cd = Buffer + string(filepath.begin() + 1, filepath.begin() + filepath.rfind("\\"));
		}
		else if (startswith(filepath, string("..\\")))
		{
			GetCurrentDirectoryA(MAX_PATH, Buffer);
			cd = Buffer;
			cd = string(cd.begin(), cd.begin() + cd.rfind("\\"));
			cd = cd + string(filepath.begin() + 2, filepath.begin() + filepath.rfind("\\"));
			cout << cd << endl;
		}
		else
		{
			GetCurrentDirectoryA(MAX_PATH, Buffer);
			if (filepath.rfind("\\") != string::npos)
			{
				cd = Buffer + string("\\") + string(filepath.begin(), filepath.begin() + filepath.rfind("\\"));
			}
			else
			{
				cd = Buffer;
			}
		}

		in.open(filepath.c_str());
		if (!in)
		{
			cout << "Read obj error !" << endl;
			exit(0);
		}
		while (getline(in, line))
		{
			if (line.size() == 0 || line[0] == '#') continue;
			istringstream is(line);
			is >> word;
			if (word == "v")
			{
				VERTEXF p;
				is >> p.x >> p.y >> p.z;
				vertexs.push_back(p);
			}
			else if (word == "vt")
			{
				pair<float, float> p;
				is >> p.first >> p.second;
				texcoords.push_back(p);
			}
			else if (word == "vn")
			{
				VERTEXF p;
				is >> p.x >> p.y >> p.z;
				normals.push_back(p);
			}
			else if (word == "o" || word == "g")
			{
				if (!goname.empty() && !faces.empty())
				{
					Object obj(vertexs.begin(), vertexs.end(), texcoords.begin(), texcoords.end(), normals.begin(), normals.end(), faces.begin(), faces.end(), row, col, mtlname);
					//while (n.count(goname) != 0)
					//{
					//	goname.push_back('0');
					//}
					//m.insert(make_pair(goname, obj));
					//n.insert(goname);
					obj.setName(goname);
					object.push_back(obj);
					faces.clear();
				}
				is >> goname;
			}
			else if (word == "f")
			{
				int r = 0, c = 0;
				while (is >> word)
				{
					c = count(word.begin(), word.end(), '/');
					if (c == 0)
					{
						faces.push_back(atoi(word.c_str()));
					}
					else if (c == 1)
					{
						faces.push_back(atoi(string(word.begin(), word.begin() + word.find("/")).c_str()));
						faces.push_back(atoi(string(word.begin() + word.find("/") + 1, word.end()).c_str()));
					}
					else if (c == 2)
					{
						int a = word.find("/");
						int b = word.find("/", a + 1);
						faces.push_back(atoi(string(word.begin(), word.begin() + a).c_str()));
						faces.push_back(atoi(string(word.begin() + a + 1, word.begin() + b).c_str()));
						faces.push_back(atoi(string(word.begin() + b + 1, word.end()).c_str()));
					}
					++r;
				}
				row = r;
				col = c + 1;
			}
			else if (word == "mtllib")
			{
				is >> word;
				ReadMtl(cd, word, material);
			}
			else if (word == "usemtl")
			{
				is >> mtlname;
			}
		}
		if (!goname.empty() && !faces.empty())
		{
			Object obj(vertexs.begin(), vertexs.end(), texcoords.begin(), texcoords.end(), normals.begin(), normals.end(), faces.begin(), faces.end(), row, col, mtlname);
			//while (n.count(goname) != 0)
			//{
			//	goname.push_back('0');
			//}
			//m.insert(make_pair(goname, obj));
			//n.insert(goname);
			obj.setName(goname);
			object.push_back(obj);
			faces.clear();
		}
		in.close();
		MatAddtoObj();
	}

	Group clone();

	// cd为当前目录
	// mtlfilepath为mtl文件名
	// mat为输出，一个材料名称对应一种材质
	void ReadMtl(string &cd, string mtlfilepath, map<string, Material> &mat)
	{
		// 打开文件
		ifstream in(cd + "\\" + mtlfilepath, ios::in);
		if (!in)
		{
			cout << "Read mtl error !" << endl;
			return;
		}


		string line, word, ptname, ntname, fname;
		unsigned map;
		bool hasmap = false;
		float ambient[3] = { 0.0,0.0,0.0 };		// 环境光强（颜色）
		float diffuse[3] = { 0.0,0.0,0.0 };		// 漫反射光强（颜色）
		float specular[3] = { 0.0,0.0,0.0 };	// 镜面反射光强（颜色）
		float emission[3] = { 0.0,0.0,0.0 };	// 材质本身颜色

		
		while (getline(in, line))
		{
			if (line.size() == 0 || line[0] == '#') continue;
			istringstream is(line);
			is >> word;
			if (word == "newmtl")
			{
				is >> ntname;
				if (!ptname.empty())
				{
					if (hasmap)
					{
						mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, map)));
					}
					else
					{
						mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, 0)));
					}
				}
				ptname = ntname;
				hasmap = false;
			}
			else if (word == "Ka")
			{
				is >> ambient[0] >> ambient[1] >> ambient[2];
			}
			else if (word == "Kd")
			{
				is >> diffuse[0] >> diffuse[1] >> diffuse[2];
			}
			else if (word == "Ks")
			{
				is >> specular[0] >> specular[1] >> specular[2];
			}
			else if (word == "Ke")
			{
				is >> emission[0] >> emission[1] >> emission[2];
			}
			else if (word == "map_Kd")
			{
				is >> fname;
				Texture texture = Texture(cd + "\\" + fname);
				map = texture.getID();
				hasmap = true;
			}
		}
		if (!ntname.empty())
		{
			if (hasmap)
			{
				mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, map)));
			}
			else
			{
				mat.insert(make_pair(ptname, Material(ambient, diffuse, specular, emission, 0)));
			}
		}
		in.close();
	}

	bool startswith(string &a, string &b)
	{
		if (a.size()<b.size())
		{
			return false;
		}
		return a.substr(0, b.size()) == b;
	}

	void MatAddtoObj()
	{
		for (vector<Object>::iterator it = object.begin(); it != object.end(); ++it)
			(*it).setMaterial(material[(*it).getMatName()]);
	}
	//int x, int y, int z, int rx, int ry, int rz, int scaleX, int scaleY, int scaleZ
	void Draw(float color[], float scale[], float trans[], float rotate[])
	{
		glPushMatrix();
		glColor3f(color[0], color[1], color[2]);
		glScalef(scale[0], scale[1], scale[2]);
		glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
		glTranslatef(trans[0], trans[1], trans[2]);
		DrawObj();
		glPopMatrix();
	}

	void DrawGhost(float color[], float scale[], float trans[], float rotate[])
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
		glTranslatef(1, 0, 5);
		//glLoadIdentity();
		glColor4f(color[0], color[1], color[2], 0.1);
		glTranslatef(trans[0], trans[1], trans[2]);
		glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
		glScalef(scale[0], scale[1], scale[2]);
		//glTranslatef(1, 0, 0);
		DrawObj();
		//glutSolidSphere(1, 50, 40)
		glDisable(GL_BLEND);
		glPopMatrix();
	}

	void DrawObj()
	{
		int signal = 0;
		// glBegin(GL_TRIANGLES);
		for (vector<Object>::iterator it = object.begin(); it != object.end(); ++it)
		{
			// Object temp = objmap[*it];
			// setMaterial(matname[temp.material]);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBindTexture(GL_TEXTURE_2D, (*it).getMaterial().map);
			if ((*it).row == 3)
				glBegin(GL_TRIANGLES);
			else
				glBegin(GL_QUADS);

			vector<int>::iterator iter = (*it).faces.begin();

			if ((*it).col == 1)
			{
				while (iter != (*it).faces.end())
				{
					glVertex3f((*it).vertexs[*iter - 1].x, (*it).vertexs[*iter - 1].y, (*it).vertexs[*iter - 1].z);
					++iter;
				}
			}
			else if ((*it).col == 2)
			{
				while (iter != (*it).faces.end())
				{
					glTexCoord2f((*it).texcoords[*(iter + 1) - 1].first, (*it).texcoords[*(iter + 1) - 1].second);
					glVertex3f((*it).vertexs[*iter - 1].x, (*it).vertexs[*iter - 1].y, (*it).vertexs[*iter - 1].z);
					iter += 2;
				}
			}
			else if ((*it).col == 3 && !(*it).texcoords.empty())
			{
				while (iter != (*it).faces.end())
				{
					if (!signal)
					{
						VERTEXD v;
						v.x = (*it).vertexs[*iter - 1].x;
						v.y = (*it).vertexs[*iter - 1].y;
						v.z = (*it).vertexs[*iter - 1].z;
						v.x += 4.0;
						v.y += 5.0;
						v.z += 5.5;

						//border.push_back(v);
						signal = 1;
					}


					//	cout << temp.vertexs[*iter - 1].x << " " << temp.vertexs[*iter - 1].y << " " << temp.vertexs[*iter - 1].z << endl;
					glNormal3f((*it).normals[*(iter + 2) - 1].x, (*it).normals[*(iter + 2) - 1].y, (*it).normals[*(iter + 2) - 1].z);
					glTexCoord2f((*it).texcoords[*(iter + 1) - 1].first, (*it).texcoords[*(iter + 1) - 1].second);
					glVertex3f((*it).vertexs[*iter - 1].x, (*it).vertexs[*iter - 1].y, (*it).vertexs[*iter - 1].z);
					iter += 3;
				}
			}
			else
			{
				while (iter != (*it).faces.end())
				{
					glNormal3f((*it).normals[*(iter + 2) - 1].x, (*it).normals[*(iter + 2) - 1].y, (*it).normals[*(iter + 2) - 1].z);
					glVertex3f((*it).vertexs[*iter - 1].x, (*it).vertexs[*iter - 1].y, (*it).vertexs[*iter - 1].z);
					iter += 3;
				}
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glEnd();
		}
		//glEnd();
	}
};

