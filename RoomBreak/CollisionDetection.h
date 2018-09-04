#ifndef __COLLISION__
#define __COLLISION__
#define CHAIRHEIGHT 0.6
#define TABLEHEIGHT 1.9
#define BOTTOM -3
#include "vector.h"
#include <iostream>
using namespace std;

class isLegalToMove
{
private:
	float curHigh;
	bool isOnTable;
	bool isOnChair;
	bool isJump;
	int MapChair[16][16] = { 0 };
	int MapTable[16][16] = { 0 };
	int MapShelf[16][16] = { 0 };
public:
/*------------------------常规的碰撞检测---------------------------*/
	isLegalToMove()
	{
		curHigh = 0;
		isOnTable = false;
		isOnChair = false;
		isJump = false;

		MapChair[15][0] = 1;     //椅子
		MapChair[15][1] = 1;
		MapChair[15][2] = 1;
		MapChair[15][3] = 1;
		MapChair[15][4] = 1;
		MapChair[14][5] = 1;
		MapChair[14][4] = 1;
		MapChair[14][3] = 1;
		MapChair[14][2] = 1;
		MapChair[14][1] = 1;
		MapChair[14][0] = 1;
		MapChair[13][4] = 1;
		MapChair[13][1] = 1;
		MapChair[13][2] = 1;
		MapChair[13][3] = 1;
		MapChair[12][1] = 1;
		MapChair[12][2] = 1;
		MapChair[12][4] = 1;
		MapChair[12][3] = 1;
		MapChair[11][3] = 1;
		MapChair[11][2] = 1;
		MapChair[11][1] = 1;
		MapChair[11][0] = 1;

		MapShelf[0][1] = 1;      //书架和柜子
		MapShelf[0][2] = 1;
		MapShelf[1][2] = 1;
		MapShelf[2][1] = 1;
		MapShelf[2][2] = 1;
		MapShelf[1][3] = 1;
		MapShelf[2][3] = 1;
		MapShelf[4][3] = 1;
		MapShelf[3][1] = 1;
		MapShelf[4][1] = 1;
		MapShelf[5][1] = 1;
		MapShelf[6][0] = 1;
		MapShelf[6][1] = 1;
		MapShelf[6][2] = 1;
		MapShelf[7][0] = 1;
		MapShelf[8][0] = 1;
		MapShelf[7][1] = 1;
	}
	~isLegalToMove() {}

	void setJump()
	{
		isJump = true;
	}

	void noCollision(Vector3 *tmpeye, Vector3 *tmpcenter, Vector3 eye, Vector3 center)     //房间边界碰撞检测
	{
		if ((*tmpeye).x > 3)
		{
			(*tmpcenter).x = center.x + 3 - eye.x;
			(*tmpeye).x = 3;
		}
		else if ((*tmpeye).x < -12)
		{
			(*tmpcenter).x = center.x + (-12) - eye.x;
			(*tmpeye).x = -12;
		}
		if ((*tmpeye).z < 0)
		{
			(*tmpcenter).z = center.z + (0) - eye.z;
			(*tmpeye).z = 0;
		}
		else if ((*tmpeye).z > 15)
		{
			(*tmpcenter).z = center.z + (15) - eye.z;
			(*tmpeye).z = 15;
		}
	}
	bool noMapInShelf(Vector3 cureye)    //书架位置碰撞检测
	{
		bool flag = MapShelf[(int)(cureye.x) + 12][(int)(cureye.z)];
		if (flag)
			return false;
		else
			return true;
	}
	bool noMapInTableAndChair(Vector3 cureye)    //在高度为地面高度的时候的桌椅碰撞检测
	{
		bool flag = ((cureye.z >= 3 &&
			cureye.z <= 11.5 &&
			cureye.x+12 >= 0 &&
			cureye.x+12 <= 3) ||
			(MapChair[(int)(cureye.x) + 12][(int)(cureye.z)]));
		if(flag)
			return false;
		else
			return true;
	}
/*------------------------常规的碰撞检测---------------------------*/

///*-----------------------起跳以后的碰撞检测-----------------------*/
//	bool isOkToLandOnOrJumpFromTable(Vector3 eye)
//	{
//		if (eye.z >= 3 &&
//			eye.z <= 11.5 &&
//			eye.x + 12 >= 0 &&
//			eye.x + 12 <= 3)
//		{
//			if (eye.y <= TABLEHEIGHT)        //在下降到桌子的时候需要判断是否在桌子和椅子的范围内
//			{
//				printf("in\n");
//				isOnTable = true;
//				isJump = false;    
//				curHigh = eye.y;           
//				return true;                           //速度？
//			}
//		}
//		else
//		{
//			if(eye.y < BOTTOM)
//			{
//				isOnTable = false;
//				isJump = false;
//				curHigh = BOTTOM;
//				return true;
//			}
//		}
//		return false;
//	}
//	bool isOkToLandOnOrJumpFromChair(Vector3 eye)
//	{
//		if (MapChair[(int)(eye.x) + 12][(int)(eye.z)])
//		{
//			if (eye.y <= CHAIRHEIGHT)        //在下降到椅子的时候需要判断是否在桌子和椅子的范围内
//			{
//				isOnChair = true;
//				isJump = false;
//				curHigh = eye.y;
//				return true;
//			}
//		}
//		else
//		{
//			if(eye.y < BOTTOM)
//			{
//				isOnChair = false;
//				isJump = false;
//				curHigh = BOTTOM;
//				return true;
//			}
//		}
//		return false;
//	}
///*-----------------------起跳以后的碰撞检测-----------------------*/
//
///*-------------------从桌子或是椅子上下来-------------------------*/
//	bool isOkToMoveFromTable(Vector3 eye)
//	{
//		if (!(eye.z >= 3 &&
//			eye.z <= 11.5 &&
//			eye.x + 12 >= 0 &&
//			eye.x + 12 <= 3))
//		{
//			if (eye.y < BOTTOM)        //在下降到桌子的时候需要判断是否在桌子和范围内
//			{
//				isOnTable = false;
//				curHigh = BOTTOM;           
//				return false;                     
//			}
//		}
//		return true;
//	}
//	bool isOkToMoveFromFromChair(Vector3 eye)
//	{
//		if (MapChair[(int)(eye.x) + 12][(int)(eye.z)])
//		{
//			if (eye.y < BOTTOM)        //在下降到椅子的时候需要判断是否在椅子的范围内
//			{
//				isOnTable = false;
//				curHigh = BOTTOM;           
//				return false;                     
//			}
//		}
//		return true;
//	}
///*-------------------从桌子或是椅子上下来-------------------------*/

/*--------------------------判断是否能移动------------------------*/
	bool judgeIsOkToMove(Vector3 *neweye, Vector3 *newcenter, Vector3 eye, Vector3 center)
	{
		//bool flag = true;
		noCollision(neweye, newcenter, eye, center);
		if (neweye->z >= 3 &&
			neweye->z <= 11.5 &&
			neweye->x + 12 >= 0 &&
			neweye->x + 12 <= 3)//如果在桌子上，且高度小于桌子
		{
			//printf("in table\n");
			if ((*neweye).y <= TABLEHEIGHT)
				return false;
			else
				return true;
		}
		if (MapChair[(int)(neweye->x) + 12][(int)(neweye->z)] )
		{
			//printf("in chair\n");
			if ((*neweye).y < CHAIRHEIGHT)
				return false;
			else
				return true;
		}
			//return false;
		if (!(noMapInShelf(*neweye) && noMapInTableAndChair(*neweye) && ((*neweye).y >= BOTTOM)))
			return false;
		
		
		return true;
		/*
		if(!isJump && !isOnTable && !isOnChair)
			flag = noMapInShelf(*neweye) && noMapInTableAndChair(*neweye) && ((*neweye).y>=BOTTOM);
		else if(!isJump && (isOnTable || isOnChair))
		{
			if(!(isOkToMoveFromTable(*neweye) && isOkToMoveFromFromChair(*neweye)))
				flag = false;
			else
				flag = true;
		}
		else
		{
			if(isOkToLandOnOrJumpFromTable(*neweye) || isOkToLandOnOrJumpFromChair(*neweye))
				flag = false;
			else
				flag = true;
		}
		return flag;
		*/
	}
};
#endif