#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include "fmod.h"			// 音频库的头文件
#pragma comment(lib, "fmodvc.lib")	// 把音频库加入到链接器中


//此部分代码参考网上
FSOUND_STREAM *bgm;		// 背景音乐 *.mp3格式
FSOUND_STREAM *fetch;
FSOUND_STREAM *footstep;
FSOUND_STREAM *hit;
FSOUND_STREAM *openlock;
FSOUND_STREAM *passwordfail;
FSOUND_STREAM *passwordsuccess;
FSOUND_STREAM *jump;
FSOUND_STREAM *gear;
FSOUND_STREAM *gameend;

GLvoid InitFMOD(void)
{
	// 载入音频文件
	if (FSOUND_Init(44100, 32, 0))		// 把声音初始化为44khz
	{
		// 载入文件bgmusic.mp3
		bgm = FSOUND_Stream_OpenFile("music/bgm.mp3", FSOUND_LOOP_NORMAL, 0);
		fetch = FSOUND_Stream_OpenFile("music/fetch.mp3", FSOUND_LOOP_OFF, 0);
		footstep = FSOUND_Stream_OpenFile("music/footstep.mp3", FSOUND_LOOP_OFF, 0);
		hit = FSOUND_Stream_OpenFile("music/hit.mp3", FSOUND_LOOP_OFF, 0);
		openlock = FSOUND_Stream_OpenFile("music/openlock.mp3", FSOUND_LOOP_OFF, 0);
		passwordfail = FSOUND_Stream_OpenFile("music/passwordfail.mp3", FSOUND_LOOP_OFF, 0);
		passwordsuccess = FSOUND_Stream_OpenFile("music/passwordsuccess.mp3", FSOUND_LOOP_OFF, 0);
		jump = FSOUND_Stream_OpenFile("music/jump.mp3", FSOUND_LOOP_OFF, 0);
		gear = FSOUND_Stream_OpenFile("music/gear.mp3", FSOUND_LOOP_OFF, 0);
		gameend = FSOUND_Stream_OpenFile("music/gameend.mp3", FSOUND_LOOP_OFF, 0);
	}
}

GLvoid FreeFMOD(void)
{
	// 释放音频文件
	if (bgm != NULL)
	{
		FSOUND_Stream_Close(bgm);	// 释放文件bgmusic.mp3
	}
}

