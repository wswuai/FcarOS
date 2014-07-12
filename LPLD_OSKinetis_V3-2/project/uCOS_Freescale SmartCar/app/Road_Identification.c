
#include "stdafx.h"
#include <stdio.h>

#define HEIGHT 240
#define WIDTH 320



typedef enum
{
	crossroads,
	left_quarter,
	right_quarter,
	left_turn,
	right_turn,
	straightaway,
	unknown_track
}TrackType;

typedef struct
{
	int bound[HEIGHT][2];
	TrackType finalType;
	int startLeft;
	int startRight;
	int endLeft;
	int endRight;
}Track;


typedef struct
{
	unsigned char *pic;
	int *bound;	//返回
	int searchCenter;
	int searchRange;
	int threshold;		//返回
	int leftOrRight;	//0 left,1 right
}RowSearchInfo;

typedef struct
{
	int searchStart;
	int searchMiddle;
	int searchEnd;
	int start;
	int end;
}LineSearchInfo;



extern "C"  __declspec(dllexport) void search(unsigned char pic[HEIGHT][WIDTH],Track *track);

static void rowSearch(RowSearchInfo *info);
static void lineSearch(unsigned char pic[][WIDTH],int bound[][2],int leftOrRight,LineSearchInfo *linfo);




void search(unsigned char pic[][WIDTH],Track *track)
{
	LineSearchInfo info;

	info.searchStart=238;
	info.searchMiddle=210;
	info.searchEnd=10;
	
	lineSearch(pic,track->bound,0,&info);
	if (info.start)
	{
		track->startLeft=info.start;
		track->endLeft=info.end;
	}
	else
		track->endLeft=track->startLeft=238;

	lineSearch(pic,track->bound,1,&info);
	if (info.start)
	{
		track->startRight=info.start;
		track->endRight=info.end;
	}
	else
		track->endRight=track->startRight=238;
}



static void lineSearch(unsigned char pic[][WIDTH],int bound[][2],int leftOrRight,LineSearchInfo *linfo)
{
	int i;
	RowSearchInfo info;
	int steady;

	info.leftOrRight=leftOrRight;		
	info.searchCenter=leftOrRight?240:80;
	info.searchRange=80;

	for (i = linfo->searchStart; i >= linfo->searchMiddle; i--)
	{
		info.threshold=50;	//不能省略
		info.bound=bound[i];	
		info.pic=pic[i];
		rowSearch(&info);
		if(bound[i][leftOrRight]) break;
	}

	if (i==linfo->searchMiddle-1)
	{
		linfo->start=0;
		return;
	}
	linfo->start=steady=i;

	for (i--; i >=linfo->searchEnd; i--)
	{
		info.threshold=50;	//不能省略
		info.searchCenter=bound[steady][leftOrRight];		
		info.searchRange=5+(steady-i-1)*3/2;
		info.bound=bound[i];	
		info.pic=pic[i];
		rowSearch(&info);
		if(bound[i][leftOrRight])
			steady=i;
		else if (steady-i>10)
			break;
	}

	linfo->end=i;
}


//在左右指定范围内查找边界，范围可以是常量或使用前一行的结果
static void rowSearch(RowSearchInfo *info)
{
	int i,diff;
	int start,end;


	info->bound[info->leftOrRight]=0;

	start=max(info->searchCenter-info->searchRange,5);
	end=min(info->searchCenter+info->searchRange,WIDTH-5);

	for (i=start; i <= end ;i++)
	{
		diff=info->leftOrRight? (info->pic[i-1] - info->pic[i]) : (info->pic[i] - info->pic[i-1]);
		if (diff > info->threshold)
		{
			info->bound[info->leftOrRight]=i;
			info->threshold=diff;
		}
	}

}



