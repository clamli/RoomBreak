#include "define.h"
#include "light.h"
#include "ghost.h"
#include "view.h"
#include "class.h"
#include "box.h"
#include "mouse.h"
#include "music.h"
#include "write.h"
#include "time.h"
#define WIDTH 600
#define HEIGHT 600

write writer;           //片头文字
//End endWrite;           //片尾文字
Position pt;
BOOLEAN button_down;
Line mouseLine;
View view;
int eventState = GAMESTART;
//int eventState = FINAL;
int frameH, frameW;
BOOLEAN turnLeft, turnRight;
int tempY;
clock_t start, stop;

//导入obj参数
Group* group[NUMOFGRP];
Ghost ghost;
 

// 开始界面
Box box_start = Box();
//结束界面
Box box_end = Box();

// 光照信息
Light light0 = Light(0);
Light light1 = Light(1);
Light light2 = Light(2);
Light light3 = Light(3);
bool isLightOn = false;

//鬼的信息
float colorGhost[3] = { 1.0, 0, 0 };
float scaleGhost[3] = { 3, 3, 3 };
float transGhost[3] = { -8, -1 ,-4 };
float rotateGhost[4] = { 0.5, 0, 1, 0 };

// 物品框
Box box_frame = Box();
Box box_key = Box();
Box box_ham = Box();
Box box_clock = Box();

//密码框
Box box_in[3];
Box box[9];
int countOfBox;
int inputNum[3];
string boxBackgroundPathPassword[9] =
{ "boxbmp\\1.bmp",
  "boxbmp\\2.bmp",
  "boxbmp\\3.bmp",
  "boxbmp\\4.bmp",
  "boxbmp\\5.bmp",
  "boxbmp\\6.bmp",
  "boxbmp\\7.bmp",
  "boxbmp\\8.bmp",
  "boxbmp\\9.bmp" };
string boxBackGroundPathIn = "boxbmp\\column.bmp";


// 鼠标管理器
MouseManager mouse = MouseManager();

// 阴影参数和矩阵
const GLfloat ground_parameter[4] = { 0, 1, 0, 11.9 };
const GLfloat wall_parameter[4] = { 1, 0, 0, 14.95 };
const GLfloat desk_parameter[4] = { 0, 1, 0, 5.9 };
GLfloat shadow_matrix_ground[4][4];
GLfloat shadow_matrix_wall[4][4];
GLfloat shadow_matrix_desk[4][4];
void generate_shadow_matrix(GLfloat matrix[4][4], const GLfloat ground[4], const GLfloat light[4])
{
	GLfloat dot = 0;
	int i, j;

	for (i = 0; i < 4; i++) {
		dot += ground[i] * light[i];
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == j) {
				matrix[i][j] = dot - ground[i] * light[j];
			}
			else {
				matrix[i][j] = -ground[i] * light[j];
			}
		}
	}
}

void setFloat3(GLfloat Float[3], double num1, double num2, double num3)//辅助函数，给三维数组赋值
{
	Float[0] = (float)num1;
	Float[1] = (float)num2;
	Float[2] = (float)num3;
}

void initialize(void)
{
	string cd;          //是不是可以定义在另外位置
	View mouserate;
	mouserate.setRate(5);//数值越大鼠标灵敏度约低
	Vector3 moverate;
	moverate.setRate(0.5);//数值越大移动距离越大

	group[INITIAL] = new Group(cd, ".\\room\\room.obj");
	group[NOCLOCK] = new Group(cd, ".\\room_clock_disp\\room_clock_disp.obj");
	group[NOPICTURE] = new Group(cd, ".\\room_password\\room_password.obj");
	group[NOPICTURE_NOCLOCK] = new Group(cd, ".\\room_password_clock_disp\\room_password_clock_disp.obj");
	group[NOTARGET] = new Group(cd, ".\\room_password_hammer\\room_password_hammer.obj");
	group[NOTARGET_NOCLOCK] = new Group(cd, ".\\room_password_hammer_clock_disp\\room_password_hammer_clock_disp.obj");
	group[NOHAMMER] = new Group(cd, ".\\room_password_hammer_disp\\room_password_hammer_disp.obj");
	group[NOHAMMER_NOCLOCK] = new Group(cd, ".\\room_password_hammer_disp_clock_disp\\room_password_hammer_disp_clock_disp.obj");
	group[FINAL] = new Group(cd, ".\\room_password_hammer_disp_clock_disp_door\\room_password_hammer_disp_clock_disp_door.obj");
	group[GHOST] = new Group(cd, ".\\ghost\\ghost.obj");
	group[PART] = new Group(cd, ".\\room_noalarmdoor\\room_noalarmdoor.obj");
	group[LAMP] = new Group(cd, ".\\lamp\\lamp.obj");

	// 开始界面初始化
	string startScreenPath = "boxbmp\\gamestart.bmp";
	box_start = Box(1.0, 1.0, 0.0, 0.0, startScreenPath, "", "", true);
	string endScreenPath = "boxbmp\\gameend.bmp";
	box_end = Box(1.0, 1.0, 0.0, 0.0, endScreenPath, "", "", true);

	// 光照初始化
	int lightX = -4.5;
	int lightY = 3.0;
	int lightZ = 7.5;
	int lightDX = 0.2;
	int lightDZ = 0.2;
	light0 = Light(0);
	light0.setPostion(lightX + lightDX, lightY, lightZ + lightDZ, 1.0f);
	light0.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	light0.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	light0.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	light1 = Light(1);
	light1.setPostion(lightX + lightDX, lightY, lightZ - lightDZ, 1.0f);
	light1.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	light1.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	light1.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	light2 = Light(2);
	light2.setPostion(lightX - lightDX, lightY, lightZ + lightDZ, 1.0f);
	light2.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	light2.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	light2.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	light3 = Light(3);
	light3.setPostion(lightX - lightDX, lightY, lightZ - lightDZ, 1.0f);
	light3.setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	light3.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	light3.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

	// 阴影初始化
	GLfloat light_pos[4] = { lightX + lightDX, lightY, lightZ + lightDZ, 1.0f };
	generate_shadow_matrix(shadow_matrix_ground, ground_parameter, light_pos);
	generate_shadow_matrix(shadow_matrix_wall, wall_parameter, light_pos);
	generate_shadow_matrix(shadow_matrix_desk, desk_parameter, light_pos);

	// 物品框初始化
	double width = 0.1;
	double widthgap = 0.0;
	double height = 0.1;
	double widthborder = 0.003;
	double heightborder = 0.01;
	string framePath = "boxbmp\\frame.bmp";
	box_frame = Box(3 * width + widthborder, height + heightborder, 1.0 - 3 * width - 2 * widthgap - widthborder, 1.0 - height - heightborder, framePath, "", "", true);
	string boxBackgroundPath = "boxbmp\\default.bmp";
	string keyPath = "boxbmp\\key.bmp", keyClickPath = "boxbmp\\key_click.bmp";
	box_key = Box(width, height, 1.0 - width, 1.0 - height, boxBackgroundPath, keyPath, keyClickPath, true);
	string hamPath = "boxbmp\\hammer.bmp", hamClickPath = "boxbmp\\hammer_click.bmp";
	box_ham = Box(width, height, 1.0 - 2 * width - widthgap, 1.0 - height, boxBackgroundPath, hamPath, hamClickPath, true);
	string clockPath = "boxbmp\\clock.bmp", clockClickPath = "boxbmp\\clock_click.bmp";
	box_clock = Box(width, height, 1.0 - 3 * width - 2 * widthgap, 1.0 - height, boxBackgroundPath, clockPath, clockClickPath, true);

	//密码框的初始化
	double widthPassword = 0.05;
	double heightPassword = 0.05;
	for (int i = 0; i < 3; i++)
		box_in[i] = Box(widthPassword, heightPassword, 0.425 + i*widthPassword, 0.5 + heightPassword, boxBackGroundPathIn, "", "", false);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			box[3 * i + j] = Box(widthPassword, heightPassword, 0.425 + j * widthPassword, 0.5 - i * heightPassword, boxBackgroundPathPassword[3 * i + j], "", "", false);
	}
}

void OnDraw(void)//绘制图像函数 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, (GLfloat)frameW / (GLfloat)frameH, 1.0, 50);
	glMatrixMode(GL_MODELVIEW);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);//背景颜色
	glLoadIdentity();
	gluLookAt(view.eye.x, view.eye.y, view.eye.z,
		view.center.x, view.center.y, view.center.z,
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0, 5, 50)，Y轴向上
	glEnable(GL_DEPTH_TEST);
	view.update();
	
	if (eventState == GAMESTART)
	{
		box_start.draw();
		mouse.showCount = 0;
		writer.setIsChinese(false);
		writer.setPosition(100, 300);
		writer.setSize(100);
		writer.setColor(1, 1, 1);
		writer.setContent("...");
	}
	else if(eventState == GAMEBEGINTIP)
	{
		writer.printString();
		write wt;
		wt.setIsChinese(false);
		wt.setPosition(280, 100);
		wt.setSize(100);
		wt.setColor(0.3, 0.3, 0.3);
		wt.setContent("Press to skip");
		wt.printString();
	}
	else if (eventState == FINAL)
	{
		start = clock(); 
		stop = clock();
		FSOUND_Stream_Close(bgm);	   // 释放文件bgmusic.mp3
		while (((double)(stop - start) / CLOCKS_PER_SEC) < 5){
			stop = clock();
		} 
		box_end.draw();
		FSOUND_Stream_Close(gear);    //齿轮转动声消失
		FSOUND_Stream_Play(FSOUND_FREE, gameend);
		eventState = FINALEND;
		//endWrite.Write();
	}
	else if (eventState == FINALEND)
	{
		//printf("bye\n");
		box_end.draw();
	}
	else
	{
		if (eventState == FINALBEGIN)
		{
			FSOUND_Stream_Play(FSOUND_FREE, gear);    //开始齿轮转动声
			eventState = FINAL;
		}
		// 打开光照
		light0.draw();
		if (isLightOn)
		{
			light1.draw();
			light2.draw();
			light3.draw();
		}
		else
		{
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
		}

		// 绘制场景
		float color[3] = { 0, 0, 0 };
		float scale[3] = { 3, 3, 3 };
		float rotate[4] = { 0, 0, 0, 1 };
		float trans[3] = { -5, -4, 0 };

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		group[eventState]->Draw(color, scale, trans, rotate);     //画整个场景
		if (!isLightOn)
		{
			group[GHOST]->DrawGhost(colorGhost, scaleGhost, transGhost, rotateGhost);          //画个鬼
			ghost.changePos(colorGhost, scaleGhost, transGhost, rotateGhost, isLightOn);             //改变鬼的坐标，大小，颜色
		}
		
		// 绘制阴影
		glColor3fv(color);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);

		glPushMatrix();
		glMultMatrixf((GLfloat*)shadow_matrix_ground);
		group[PART]->Draw(color, scale, trans, rotate);     // 画无门内部阴影场景
		glPopMatrix();

		glPushMatrix();
		glMultMatrixf((GLfloat*)shadow_matrix_wall);
		group[PART]->Draw(color, scale, trans, rotate);     // 画无门内部阴影场景
		glPopMatrix();

		glPushMatrix();
		glMultMatrixf((GLfloat*)shadow_matrix_desk);
		group[LAMP]->Draw(color, scale, trans, rotate);     // 画台灯阴影场景
		glPopMatrix();

		glDisable(GL_BLEND);

		// 绘制物品框
		box_key.draw();
		box_ham.draw();
		box_clock.draw();
		box_frame.draw();

		//绘制密码框
		for (int i = 0; i < 3; i++)
			box_in[i].draw();
		for (int i = 0; i < 9; i++)
			box[i].draw();

		// 恢复视角
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(75, (GLfloat)frameW / (GLfloat)frameH, 1.0, 50);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(view.eye.x, view.eye.y, view.eye.z,
			view.center.x, view.center.y, view.center.z,
			0, 1, 0);				// 场景（0，0，0）的视点中心 (0, 5, 50)，Y轴向上
		glEnable(GL_DEPTH_TEST);
		view.update();
	}
	
	glutSwapBuffers();
} 

void OnUpdate(void) 
{
	static int count = 0;
	if (count < 5)count++;
	else count = 0;
	if ((turnLeft || turnRight) && count==0)
	{
		Position new_pt, temp;
		if (turnLeft)
			temp.reset(19, tempY);
		else
			temp.reset(frameW - 19,tempY);
		new_pt = temp - pt;
		view.setView(-new_pt.x, -new_pt.y);
		if (turnLeft)
			temp.reset(20, tempY);
		else
			temp.reset(frameW - 20, tempY);
		pt = temp;
	}
	//printf("2\n");
	OnDraw();

}
void keyboard(unsigned char k, int x, int y)
{
	if (eventState == GAMESTART);
	else if (eventState == GAMEBEGINTIP)
	{
		if (k == ' ')
		{
			eventState = INITIAL; writer.setContent("...");
		}
	}
	else
	switch (k)
	{
	case 27: exit(0); break;
	case 'q': eventState = GAMESTART; box_clock.setDefault(); box_ham.setDefault(); box_key.setDefault(); isLightOn = false; break;
	case 'a': view.viewMove(LEFT); break;
	case 'd': view.viewMove(RIGHT); break;
	case 'w': view.viewMove(FORWARD); break;
	case 's': view.viewMove(BACKWARD); break;
	case ' ': view.viewMove(JUMP); break;
	default: break;
	}
}

void Mouse(int button, int state, int x, int y) //处理鼠标点击  
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		button_down = TRUE;                /*当l_button_down为true时才可改变相机位置*/
		pt.reset(x, y);                      /*记录旧坐标，以便移动发生时计算鼠标移动方向*/
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mouse.manage(x, y);
		button_down = FALSE;        /*拿起时不能改变摄像机位置*/
	}
}

void onMouseMove(int x, int y) //处理鼠标移动  
{
	Position new_pt, temp;
	tempY = y;
	if (x>20&&x<frameW-20)         /*如果鼠标没有按下则不改变摄像机位置*/
	{
		temp.reset(x, y);
		new_pt = temp - pt;
		view.setView(-new_pt.x, -new_pt.y);
		pt = temp;
		turnLeft = false;
		turnRight = false;
	}
	if (x < 20)
		turnLeft = true;
	if (x > frameW - 20)
		turnRight = true;
}

void reshape(int w, int h)
{
	frameH = h;
	frameW = w;
	pt.reset(w / 2, h / 2);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, (GLfloat)w / (GLfloat)h, 1.0, 50);
	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char*  argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(150, 50);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Room Break");
	glutFullScreen();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	initialize();
	glutDisplayFunc(&OnDraw);
	glutIdleFunc(&OnUpdate);
	//glutIdleFunc(&OnDraw);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(onMouseMove);
	glutKeyboardFunc(keyboard);
	InitFMOD();
	FSOUND_Stream_Play(FSOUND_FREE, bgm);
	glutMainLoop();
}