#include "stdafx.h"
#include "CCamCapture2.h"


cv::Mat CCamCapture2::m_Image;
cv::Mat CCamCapture2::m_pROI;
cv::Rect CCamCapture2::m_TargetRect;
cv::Point CCamCapture2::m_Origin;
BOOL CCamCapture2::m_bTargetObj;


CCamCapture2::CCamCapture2()
{
	//onGrabLoop_cvInit();
	//m_Image = NULL;
	//m_pROI = NULL;
	//SetCaptureCallback(m_pfnCustomCB);
	
}

CCamCapture2::~CCamCapture2()
{
	onGrabLoop_cvClose();
}

BOOL CCamCapture2::CreateCapture()
{
	if (m_bCamInited)
	{
		return TRUE;
	}
	else
	{
		if (m_pMyThread)
		{
			m_bCamInited = false;
		}
		onGrabLoop_cvInit();
		m_pMyThread = AfxBeginThread(threadGrabImage, (LPVOID)this);
	}
}

void CCamCapture2::DestoryCapture()
{
	if (m_bCamInited)
	{
		StopCapture();
		WaitForSingleObject(m_pMyThread->m_hThread, INFINITE);
	}
}

void CCamCapture2::StartCapture()
{
	if (GetCaptureState() == PAUSE)
	{
		m_pMyThread->ResumeThread();
	}
	m_State = START;
}

void CCamCapture2::PauseCapture()
{
	m_State = PAUSE;
}

void CCamCapture2::StopCapture()
{
	m_State = STOP;
}

CaptureState CCamCapture2::GetCaptureState() const
{
	return m_State;
}

cv::Mat* CCamCapture2::GetSelectedROI() const
{
	return &m_pROI;
}

cv::Rect CCamCapture2::GetTargetRect() const
{

	return m_TargetRect;
}

void CCamCapture2::doGrabLoop()
{
	cv::Mat frame;
	while (m_bCamInited)
	{
		if ( GetCaptureState() == START)
		{
		

			m_cap >> frame;
			if (frame.empty())
			{
				break;
			}
			frame.copyTo(m_Image);
			cv::imshow("ShowImage", m_Image);


			if (m_pfnCustomCB)
			{
				m_pfnCustomCB(frame);
			}
			
		}

		else if (GetCaptureState() == PAUSE)
		{
			m_pMyThread->SuspendThread();
			
		}
		else
		{
			m_bCamInited = false;
		}
	}
}

void CCamCapture2::onGrabLoop_cvInit()
{
	m_cap.open(0);

	if (m_cap.isOpened())
	{
		m_bCamInited = TRUE;
		//PauseCapture();
		cv::namedWindow("ROI");
		cv::namedWindow("ShowImage2");
		cvSetMouseCallback("ShowImage", onMouseCB2, 0);
	}
	else
	{
		m_bCamInited = FALSE;
	}
}

void CCamCapture2::onGrabLoop_cvClose()
{
	if (m_cap.isOpened())
	{
		m_cap.release();
		cv::destroyWindow("ROI");
	}
}
void CCamCapture2::onMouseCB2(int event, int x, int y, int flags, void* param)
{
	CvPoint p1, p2;
	if (m_bTargetObj == FALSE && event == CV_EVENT_LBUTTONDOWN)
	{
		m_Origin.x = x;
		m_Origin.y = y;
		m_bTargetObj = TRUE;

	}
	else if (m_bTargetObj == TRUE && event == CV_EVENT_MOUSEMOVE)
	{

		p1 = cvPoint(m_Origin.x, m_Origin.y);
		p2 = cvPoint(x, y);
		rectangle(m_Image, cv::Point(p1), cv::Point(p2), cv::Scalar(0, 255, 0), 2);
		cv::imshow("ShowImage", m_Image);

	}
	else if (m_bTargetObj == TRUE && event == CV_EVENT_LBUTTONUP)
	{
		m_TargetRect = cv::Rect(cv::Point(m_Origin.x, m_Origin.y), cv::Point(x, y));
		
		if (m_TargetRect.width && m_TargetRect.height)
		{
			m_pROI = m_Image(m_TargetRect);
			
			cv::imshow("ROI", m_pROI);
			//waitKey(0);  
		}
		m_bTargetObj = FALSE;
	}
}

UINT CCamCapture2::threadGrabImage(LPVOID pParam)
{
	CCamCapture2* pCapture = ((CCamCapture2*)pParam);

	pCapture->doGrabLoop();

	return 0;
}

void CCamCapture2::onGrabLoop_DrawROI(cv::Mat& frame)
{
	if (m_bTargetObj == TRUE)
	{

	}
	else
	{
		cv::imshow("roi",m_pROI);
	}
}

void CCamCapture2::SetTargetRect(cv::Rect Rect)
{
	m_Image(Rect);
}

void CCamCapture2::SetCaptureCallback(pfnCaptureCallback2 pfnCaptureCB)
{

	m_pfnCustomCB = pfnCaptureCB;

}
