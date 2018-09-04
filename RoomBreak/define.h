#pragma once
#define MAX_PATH 260
#define NUMOFGRP 20
#define PI 3.1415926f
#define MMM 2

//direction
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define JUMP 4

// 物品栏中的属性
#define BOX_DEFAULT 0
#define BOX_OBJECT 1
#define BOX_OBJECT_CLICK 2

//鉴别物品栏和密码框
#define ITEM 0
#define PASSWORD 1
#define PASSWORDCORRECT -1

//场景控制信号
#define GAMEBEGINTIP -2
#define GAMESTART -1
#define INITIAL 0
#define NOCLOCK 1
#define NOPICTURE 2
#define NOPICTURE_NOCLOCK 3
#define NOTARGET 4
#define NOTARGET_NOCLOCK 5
#define NOHAMMER 6
#define NOHAMMER_NOCLOCK 7
#define FINAL 8
#define FINALEND 9
#define FINALBEGIN 10
#define GHOST 11
#define PART 12
#define LAMP 13

//文字输出控制信号
#define LEFTEDGEX 30
#define LEFTEDGEY 400
#define CHARSIZE 15
#define CHARGAP 10
#define LINELENGTH 700
#define LINEHEIGHT 70
#define LINESIZE 135

#include <iostream>
using namespace std;

#define FREEGLUT_STATIC
#include "GL/freeglut.h"

#include <vector>
#include <fstream>
