#pragma once
#include "define.h"
#include "write.h"
#include <string>
#include <time.h>
using namespace std;

class End
{
	int index;
	double start_x;
	double start_y;
	double charGap;
	double lineLength;
	double lineHeight;
	string content1;
	string content2;
	string content3;
	string content4;
	write wte;
	
public:
	End() 
	{
		index = 0;
		start_x = LEFTEDGEX;
		start_y = LEFTEDGEY;
		charGap = CHARGAP;
		lineLength = LINELENGTH;
		lineHeight = LINEHEIGHT;
		content1 = "You think you have escaped from the";
		content2 = "room, but actually you are running";
		content3 = "into another endless nightmare, and,";
		content4 = "you will know it soon......";

		wte.setSize(CHARSIZE);
		wte.setIsChinese(false);
		//wte.setFont();
		wte.setColor(1, 1, 1);
	}

	void Delay() 
	{
		time_t start, stop;
		start = time(NULL);
		do {
			stop = time(NULL);
		} while (stop - start <= 1);
		printf("Use Time:%ld\n", (stop - start));
	}

	void Write()
	{
		wte.setContent(content1);
		wte.setPosition(start_x, start_y);
		wte.printString();
		wte.setContent(content2);
		wte.setPosition(start_x, start_y - LINEHEIGHT);
		wte.printString();
		wte.setContent(content3);
		wte.setPosition(start_x, start_y - 2 * LINEHEIGHT);
		wte.printString();
		wte.setContent(content4);
		wte.setPosition(start_x, start_y - 3 * LINEHEIGHT);
		wte.printString();
	}

	//	start_x = LEFTEDGEX;
	//	start_y = LEFTEDGEY;
	//	for (int i = 0; i <= index; i++)
	//	{
	//		char arr[2] = {content[i], '\0'};
	//		string ctx(arr);
	//		printf("%s\n", &ctx);
	//		wte.setContent(ctx);
	//		wte.setPosition(start_x, start_y);
	//		wte.printString();
	//		if(start_x + charGap > LINELENGTH)
	//		{
	//			start_x = LEFTEDGEX;
	//			start_y -= LINEHEIGHT;
	//		}
	//		else
	//		{
	//			start_x += charGap;
	//		}
	//	}
	//	if (index != LINESIZE - 1)
	//		index++;
	//			//todo
	//		//wte.setContent(content);
	//		//wte.setPosition(start_x, start_y);
	//		//wte.printString();
	//}
};