#pragma once
#include "iostream"
#include <cv.h>
#include <highgui.h>
#include <minwindef.h>




typedef struct
{


} TempMatchParam;

typedef struct
{


} MeanShiftParam;

typedef struct
{
	

} CamShiftParam;


typedef struct
{
	TempMatchParam TM_Param;
	MeanShiftParam MS_Param;
	CamShiftParam CS_Param;

} VT_Params;


class CVisualTracker {
public:
	CVisualTracker();
	~CVisualTracker();
	void SetMethodType(VT_MethodType Type);


private:
	BOOL TrackingByTempMatching(cv::Mat &Frame, cv::Rect &TrackRect);
	VT_MethodType m_CurrType;
	VT_Params m_CurrParams;
	TempMatchParam& m_CurrTMMethod;
	cv::Mat* m_pROI;

};