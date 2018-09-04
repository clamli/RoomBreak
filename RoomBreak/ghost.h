#pragma once
#include <time.h>
class Ghost
{
private:
	int count;
	double delta;
	bool transOutRoom(float trans[])
	{
		if (trans[2] >= 3 &&
			trans[2] <= 11.5 &&
			trans[0] + 12 >= 0 &&
			trans[0] + 12 <= 3)
			return true;
		return false;
	}
public:
	Ghost()
	{
		count = 0;
		delta = 0;
	}
	void changePos(float color[], float scale[], float trans[], float rotateGhost[], bool isLightOn)
	{
		count++;
		if (count >= 10)
		{	
			count = 0;
			trans[0] = 5 * cos(delta);
			trans[2] = 5 * sin(delta);
			delta += 2 * PI / 360;
			if (delta >= 2*PI)
				delta -= 2 * PI;
		}
		//else if(count >= 10)
		//{
		//	count = 0;
		//	srand((unsigned)time(NULL));
		//	
		//	
		//	//printf("%f\n", trans[1]);
		//}		
		rotateGhost[0] += 0.2;	
		if (rotateGhost[0] >= 360)
			rotateGhost[0] -= 360;
	}
};
 