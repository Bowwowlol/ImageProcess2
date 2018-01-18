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

//typedef enum {TM_SQDIFF = CV_TM_SQDIFF, TM_SQDIFF_NORMED, TM_CCORR, TM_CCORR_NORMED,TM_CCOEFF, TM_CCOEFF_NORMED} TempMatchParam;

typedef enum  {TempMatch = 1, MeanShift, CAMShift} VT_MethodType;


class CVisualTracker {
public:
	CVisualTracker();
	~CVisualTracker();
	void SetMethodType(VT_MethodType Type);
	VT_MethodType GetMethodType() const;
	void SetVT_Params(VT_MethodType Type, VT_Params Param);
	VT_Params GetVT_Params() const;
	void SetROI(cv::Mat* pROI);
	cv::Mat* GetROI() const;
	BOOL Tracking(cv::Mat &Frame, cv::Rect &TrackRect);
	void ShowResult(cv::Mat &Frame, cv::Rect &TrackRect);



private:
	BOOL TrackingByTempMatching(cv::Mat &Frame, cv::Rect &TrackRect);
	VT_MethodType m_CurrType;
	VT_Params m_CurrParams;
	TempMatchParam& m_CurrTMMethod;
	cv::Mat* m_pROI;

};