
// MFCApplication3Dlg.h : 標頭檔
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <cv.h>
#include <highgui.h>
#include "CCamcapture2.h"

// CMFCApplication3Dlg 對話方塊
class CMFCApplication3Dlg : public CDialogEx
{
// 建構
public:
	CMFCApplication3Dlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_Cam_Start;
	afx_msg void OnBnClickedCamStart();
	CButton m_Pause;
	CComboBox m_Method_Selection;
	typedef enum
	{
		START,
		PAUSE,
		STOP,
	}CaptrueState;
	CaptrueState Cam;
	afx_msg void OnBnClickedPause();

	int Get_Combobox_Index = 0;
	
	afx_msg void CBN_SELENDOK_Method_Selection();
	CStatic m_Variable1;
	CStatic m_Variable2;
	CStatic m_Variable3;
	CStatic m_Parameter1;
	CStatic m_Parameter2;
	CStatic m_Parameter3;
	CSliderCtrl m_SliderControl1;
	CSliderCtrl m_SliderControl2;
	CSliderCtrl m_SliderControl3;
	afx_msg void OnNMCustomdrawSlidercontrol1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlidercontrol2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlidercontrol3(NMHDR *pNMHDR, LRESULT *pResult);
	//cv::VideoCapture capture;

	
	
	


	void OnOpenThread();
	
	//void ShowImage(cv::Mat Image, CWnd* pWnd);

	CStatic m_Show_Image;
	
	CEvent g_event;

	

	CCamCapture2 ccamcapture2;

	static void  CallbackinDlg(cv::Mat& Mat);

	

	//class CCamCapture
	//{
	//public:
	//	CCamCapture();
	//	~CCamCapture();
	//	BOOL CreateCapture();
	//	void DestoryCapture();
	//	void StartCapture();
	//	void PauseCapture();
	//	void StopCapture();
	//	CaptrueState GetCaptureState() const;
	//	cv::Mat* GetSelectedROI() const;
	//	cv::Rect GetTargetRect() const;
	//	void SetTargetRect(cv::Rect Rect);
	//	//void SetCaptureCallback(pfnCaptureCallback2 pfnCaptureCB);
	//protected:
	//	virtual void doGrabLoop();
	//	virtual void onGrabLoop_cvInit();
	//	virtual void onGrabLoop_cvClose();
	//private:
	//};
	
private:

	CWinThread* m_pThread;
	static UINT MyThread(LPVOID pVar);
	static bool m_Continue;
	static cv::VideoCapture capture;
	static void ShowImage(cv::Mat Image, CWnd* pWnd);
	static cv::Mat frame;
	static cv::Mat image;

	static cv::Rect selection;
	static bool trackobject;
	static bool select_flag;
	
	static void onMouse(int event, int x, int y, int flags, void* param);
public:
	CButton m_IniCam;
	afx_msg void OnBnClickedInitcam();
};
