#include "stdafx.h"
#include "CVisualTracker.h"



CVisualTracker::~CVisualTracker()
{

}

BOOL CVisualTracker::TrackingByTempMatching(cv::Mat &Frame, cv::Rect &TrackRect)
{
	if (m_pROI == NULL)
	{
		return FALSE;
	}

	cv::Mat ResultMap;
	ResultMap.create(Frame.rows - m_pROI->rows + 1, Frame.cols - m_pROI->cols + 1, CV_32FC1);
	cv::matchTemplate(Frame, *m_pROI, ResultMap, CV_TM_SQDIFF);
	double minVal;
	cv::Point minLoc;
	minMaxLoc(ResultMap, &minVal, 0, &minLoc, 0);
}

cv::Mat* CVisualTracker::GetROI() const
{
	return m_pROI;
}

void CVisualTracker::SetMethodType(VT_MethodType Type)
{
	m_CurrType = Type;
}

VT_MethodType CVisualTracker::GetMethodType() const
{
	return m_CurrType;
}

void CVisualTracker::SetVT_Params(VT_MethodType Type, VT_Params Param)
{
	m_CurrType = Type;
	m_CurrParams = Param;
}

VT_Params CVisualTracker::GetVT_Params() const
{
	return m_CurrType, m_CurrParams;
}
