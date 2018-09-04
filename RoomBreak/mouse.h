#pragma once

#include "define.h"
#include "box.h"
#include "music.h"
#include "write.h"
#include "time.h"

extern Box box_key;
extern Box box_ham;
extern Box box_clock;

extern Line mouseLine;
extern View view;

//密码框
extern Box box_in[3];
extern Box box[9];
extern int countOfBox;
extern int inputNum[3];
extern string boxBackgroundPathPassword[9];
extern string boxBackGroundPathIn;
extern int eventState;
extern write writer;

//控制变量
bool enablePassword = false;
int countOfTime;
extern bool isLightOn;
extern clock_t start;

class MouseManager
{
private:
	// 门把手事件
	void knob_exec() 
	{
		FSOUND_Stream_Play(FSOUND_FREE, openlock);
		if (box_key.getType() == BOX_OBJECT_CLICK)
		{
			eventState = FINALBEGIN;
			box_key.setDefault();
			return;
		}

		if (isLightOn) isLightOn = false;
		else isLightOn = true;
	}
	
	// 钟表事件
	void clock_exec() 
	{
		switch (eventState)
		{
			case INITIAL:eventState = NOCLOCK; FSOUND_Stream_Play(FSOUND_FREE, fetch); break;
			case NOPICTURE:eventState = NOPICTURE_NOCLOCK; FSOUND_Stream_Play(FSOUND_FREE, fetch); break;
			case NOTARGET:eventState = NOTARGET_NOCLOCK; FSOUND_Stream_Play(FSOUND_FREE, fetch); break;
			case NOHAMMER:eventState = NOHAMMER_NOCLOCK; FSOUND_Stream_Play(FSOUND_FREE, fetch); break;
			default:break;
		}
		box_clock.setObject();
	}

	// 画事件
	void paint_exec() 
	{
		switch (eventState)
		{		
			case INITIAL:eventState = NOPICTURE; FSOUND_Stream_Play(FSOUND_FREE, fetch); break;
			case NOCLOCK:eventState = NOPICTURE_NOCLOCK; FSOUND_Stream_Play(FSOUND_FREE, fetch); break;
			default:break;
		}
	}

	// 靶心事件
	void target_exec() 
	{
		int en=0;
		switch (eventState)
		{
		case NOPICTURE:if (countOfTime == PASSWORDCORRECT) { eventState = NOTARGET; en = 1; FSOUND_Stream_Play(FSOUND_FREE, passwordsuccess);
		} break;
		case NOPICTURE_NOCLOCK:if (countOfTime == PASSWORDCORRECT) { eventState = NOTARGET_NOCLOCK; en = 1; FSOUND_Stream_Play(FSOUND_FREE, passwordsuccess);
		} break;
		case NOTARGET:eventState = NOHAMMER; FSOUND_Stream_Play(FSOUND_FREE, fetch); box_ham.setObject(); break;
		case NOTARGET_NOCLOCK:eventState = NOHAMMER_NOCLOCK; FSOUND_Stream_Play(FSOUND_FREE, fetch); box_ham.setObject(); break;
		default:break;
		}
		if (eventState >= NOTARGET && en == 0);
		else {
			en = 0;
			if (!enablePassword)
			{
				countOfTime = 0;
				enablePassword = true;
				for (int i = 0; i < 3; i++)
					box_in[i].setEnable();
				for (int i = 0; i < 9; i++)
					box[i].setEnable();
			}
			else
			{
				enablePassword = false;
				countOfTime = 0;
				for (int i = 0; i < 3; i++)
					box_in[i].disEnable();
				for (int i = 0; i < 9; i++)
					box[i].disEnable();
			}
		}
	}

public:
	int showCount;
	MouseManager() {}

	void manage(int x, int y)
	{
		if (eventState == GAMESTART)
		{
			//view.reset();
			if (1.0*x / frameW >= 1.0 * 915 / 1366 && 1.0*x / frameW <= 1.0 * 1155 / 1366
				&& 1.0*y / frameH >= 1.0 * 525 / 768 && 1.0*y / frameH <= 1.0 * 585 / 768)
				eventState = GAMEBEGINTIP;
		}
		else if (eventState == GAMEBEGINTIP)
		{
			switch (showCount)
			{
				case 0:writer.setContent("I drank too much last night..."); break;
				case 1:writer.setContent("What time is it now?"); break;
				case 2:writer.setContent("I feel thirsty of throat..."); break;
				case 3:writer.setContent("Is this a hotel?"); break;
				case 4:writer.setContent("No, it can't be a hotel..."); break;
				case 5:writer.setContent("Maybe I am trapped in?"); break;
				case 6:writer.setContent("I have to escape!"); break;
				case 7:writer.setContent("I have to escape..."); break;
				case 8:eventState = INITIAL; writer.setContent("..."); break;
				default:showCount = 0; break;
			}
			showCount++;
		}
		else if (eventState == FINALEND)
		{
			if (1.0*x / frameW >= 0.27 && 1.0*x / frameW <= 0.43
				&& 1.0*y / frameH >= 0.75 && 1.0*y / frameH <= 0.81)
			{
				eventState = GAMESTART; 
				FSOUND_Stream_Close(gameend);
				InitFMOD();
				FSOUND_Stream_Play(FSOUND_FREE, bgm);
				box_clock.setDefault(); 
				box_ham.setDefault(); 
				box_key.setDefault(); 
				isLightOn = false;
			}
			else if (1.0*x / frameW >= 0.58 && 1.0*x / frameW <= 0.68
				&& 1.0*y / frameH >= 0.75 && 1.0*y / frameH <= 0.81)
			{
				exit(0);
			}
		}
		else
		{
			int ret_box = 0;
			if (box_key.click(x, y, ITEM)) ret_box = 1;
			if (box_ham.click(x, y, ITEM)) ret_box = 1;
			if (box_clock.click(x, y, ITEM)) ret_box = 1;
			if (box_ham.getType() == BOX_OBJECT_CLICK && box_clock.getType() == BOX_OBJECT_CLICK)
			{
				box_ham.setDefault();
				box_clock.setDefault();
				box_key.setObject();
				FSOUND_Stream_Play(FSOUND_FREE, hit);
			}
			if (ret_box) return;

			if (enablePassword && countOfTime < 3)              //如果密码框被激活
			{
				if (countOfBox == 3)         //如果次数已到3次则最上面三个方格密码清空
				{
					if (!(inputNum[0] == 2 && inputNum[1] == 3 && inputNum[2] == 3))    //如果密码输入错误
					{
						FSOUND_Stream_Play(FSOUND_FREE, passwordfail);
						countOfBox = 0;               //清空重新输入
						for (int i = 0; i < 3; i++)
						{
							box_in[i].setTextureID(boxBackGroundPathIn);
							inputNum[i] = 0;
						}
						countOfTime++;             //输入次数自增1
					}
					else
					{
						countOfTime = PASSWORDCORRECT;     //否则输入次数自动到-1,代表已经输入正确，停止输入
						countOfBox = 0;                    //清空输入,为下一次做准备
						for (int i = 0; i < 3; i++)
						{
							box_in[i].setTextureID(boxBackGroundPathIn);
							inputNum[i] = 0;
						}
					}
				}
				else                            //如果次数没到3次则继续输入
				{
					for (int i = 0; i < 9; i++)
					{
						bool flag = false;
						if ((flag = box[i].click(x, y, PASSWORD)) == true)
						{
							box_in[countOfBox].setTextureID(boxBackgroundPathPassword[i]);
							inputNum[countOfBox] = i + 1;
							countOfBox++;
							break;
						}
					}
				}
			}

			mouseLine.getLine(x, y);
			Vector3 v;
			v.reset(-15.1, -4.4, 12.5);
			if (mouseLine.isEvent(view.eye, v, 3.0)) knob_exec();
			v.reset(-14.5, -2.7, 8.5);
			if (mouseLine.isEvent(view.eye, v, 2.0)) clock_exec();
			v.reset(-8.7, -4.2, -0.1);
			if (mouseLine.isEvent(view.eye, v, 2.0)) paint_exec();
			v.reset(-9.1, 1.9, 0.3);
			if ((countOfTime == PASSWORDCORRECT) || (countOfTime == 3) || ((mouseLine.isEvent(view.eye, v, 2.0) && (!enablePassword))))
				target_exec();
		}
	}
};


