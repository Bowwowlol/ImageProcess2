#pragma once
#include "iostream"
#include <cv.h>
#include <highgui.h>
#include <minwindef.h>

typedef enum
{
	START,
	PAUSE,
	STOP,
}CaptureState;

typedef void(__cdecl *pfnCaptureCallback2)(cv::Mat& Frame);

class CCamCapture2
{
public:
	CCamCapture2();
	~CCamCapture2();
	BOOL CreateCapture();
	void DestoryCapture();
	void StartCapture();
	void PauseCapture();
	void StopCapture();
	CaptureState GetCaptureState() const;
	cv::Mat* GetSelectedROI() const;
	cv::Rect GetTargetRect() const;
	void SetTargetRect(cv::Rect Rect);
	void SetCaptureCallback(pfnCaptureCallback2 pfnCaptureCB);

protected:
	static UINT threadGrabImage(LPVOID pParam);
	CWinThread* m_pMyThread;
	void doGrabLoop();
	void onGrabLoop_cvInit();
	void onGrabLoop_cvClose();

	//camera inited flag
	BOOL m_bCamInited;
	// Event indicating that the thread is about to exit.
	HANDLE m_heventThreadDone;
	// camera capture state
	CaptureState m_State;
	// target selection
	static BOOL m_bTargetObj;

private:
	static void onMouseCB2(int event, int x, int y, int flags, void* param);
	void onGrabLoop_DrawROI(cv::Mat& frame);
	static cv::Mat m_Image;
	static cv::Mat m_pROI;
	static cv::Rect m_TargetRect;
	static cv::Point m_Origin;

	cv::VideoCapture m_cap;
	pfnCaptureCallback2 m_pfnCustomCB;
};

