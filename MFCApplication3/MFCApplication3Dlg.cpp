
// MFCApplication3Dlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊


struct THREAD_INFO
{
	int data;//設定執行續暫停時間
	bool * Continue;//是否繼續執行
	int CtrlID;//執行續控制的slider控制項的代號
	HWND hWnd;//產生執行續的視窗物件
	//bool * g_bThreadContinue;
}Thread_Info, *pThread_Info;//建立執行續


//我在家裡面加了這個測試看看。

cv::VideoCapture CMFCApplication3Dlg::capture;
bool CMFCApplication3Dlg::m_Continue;


cv::Rect CMFCApplication3Dlg::selection;

bool CMFCApplication3Dlg::trackobject = false;
bool CMFCApplication3Dlg::select_flag = false;

cv::Mat CMFCApplication3Dlg::frame;
cv::Mat CMFCApplication3Dlg::image;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援


// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 對話方塊



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Cam_Start, m_Cam_Start);
	DDX_Control(pDX, IDC_Pause, m_Pause);
	DDX_Control(pDX, IDC_Method_Selection, m_Method_Selection);
	DDX_Control(pDX, IDC_VARIABLE1, m_Variable1);
	DDX_Control(pDX, IDC_VARIABLE2, m_Variable2);
	DDX_Control(pDX, IDC_VARIABLE3, m_Variable3);
	DDX_Control(pDX, IDC_PARAMETER1, m_Parameter1);
	DDX_Control(pDX, IDC_PARAMETER2, m_Parameter2);
	DDX_Control(pDX, IDC_PARAMETER3, m_Parameter3);
	DDX_Control(pDX, IDC_SLIDERCONTROL1, m_SliderControl1);
	DDX_Control(pDX, IDC_SLIDERCONTROL2, m_SliderControl2);
	DDX_Control(pDX, IDC_SLIDERCONTROL3, m_SliderControl3);
	DDX_Control(pDX, IDC_SHOW_IMAGE, m_Show_Image);
	DDX_Control(pDX, IDC_InitCam, m_IniCam);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Cam_Start, &CMFCApplication3Dlg::OnBnClickedCamStart)
	ON_BN_CLICKED(IDC_Pause, &CMFCApplication3Dlg::OnBnClickedPause)
	ON_CBN_SELENDOK(IDC_Method_Selection, &CMFCApplication3Dlg::CBN_SELENDOK_Method_Selection)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERCONTROL1, &CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERCONTROL2, &CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERCONTROL3, &CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol3)
	ON_BN_CLICKED(IDC_InitCam, &CMFCApplication3Dlg::OnBnClickedInitcam)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 訊息處理常式

BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	m_Show_Image.SetWindowPos(&wndTop, 15, 15, 640, 480, SWP_SHOWWINDOW);
	
	cv::namedWindow("ShowImage");
	HWND hWnd = (HWND)cvGetWindowHandle("ShowImage");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_SHOW_IMAGE)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);


	
	//g_bThreadContinue = FALSE;

	m_Method_Selection.AddString(_T("TemplateMatch"));
	m_Method_Selection.AddString(_T("MeamShift"));
	m_Method_Selection.AddString(_T("CamShift"));
	m_Method_Selection.SetCurSel(0);

	m_Parameter1.SetWindowTextW(L"TM");
	m_Parameter2.SetWindowTextW(L"NULL");
	m_Parameter3.SetWindowTextW(L"NULL");

	m_SliderControl1.SetRange(0, 10);
	m_SliderControl1.SetPos(0);

	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl1.GetPos());

	m_Variable1.SetWindowTextW(buffer);


	m_Parameter2.EnableWindow(0);
	m_Parameter3.EnableWindow(0);

	m_SliderControl2.EnableWindow(0);
	m_SliderControl3.EnableWindow(0);

	m_Variable2.EnableWindow(0);
	m_Variable3.EnableWindow(0);

	m_Pause.EnableWindow(0);

	Cam = STOP;
	ccamcapture2.StopCapture();

	ccamcapture2.SetCaptureCallback(CallbackinDlg);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMFCApplication3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication3Dlg::OnBnClickedCamStart()
{
	

	Thread_Info.hWnd = m_hWnd;
	Thread_Info.Continue = &m_Continue;

	// TODO: 在此加入控制項告知處理常式程式碼
	if (ccamcapture2.GetCaptureState() != START)
	{
		m_Cam_Start.SetWindowTextW(L"Stop");
		m_Pause.EnableWindow(1);
		ccamcapture2.StartCapture();
		
		//capture.open(0);
		m_Continue = true;
		//cv::Mat image = cv::imread("scarlett_johansson_5-wide.jpg");
		//CWnd* pWnd_SHOW_IMAGE = GetDlgItem(IDC_SHOW_IMAGE);
		//ShowImage(image, pWnd_SHOW_IMAGE);
		cv::namedWindow("ShowImage");
		/*HWND hWnd = (HWND)cvGetWindowHandle("ShowImage");
		HWND hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem(IDC_SHOW_IMAGE)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);*/

		//m_pThread = AfxBeginThread(MyThread, (LPVOID)&Thread_Info);
		ccamcapture2.CreateCapture();
		
	}
	else if (ccamcapture2.GetCaptureState() == START)
	{
		m_Cam_Start.SetWindowTextW(L"Start");
		m_Pause.SetWindowTextW(L"Pause");
		m_Pause.EnableWindow(0);
		ccamcapture2.StopCapture();

		ccamcapture2.DestoryCapture();
		ccamcapture2.~CCamCapture2();
		//m_Continue = false;
		//WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		//cvDestroyWindow("ShowImage");
		/*DWORD dwCode;
		GetExitCodeThread(m_pThread->m_hThread, &dwCode);
		TerminateThread(m_pThread->m_hThread, dwCode);
		CloseHandle(m_pThread->m_hThread);*/

		//capture.release();
		//cv::destroyAllWindows();
		
	}
}



void CMFCApplication3Dlg::OnBnClickedPause()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (ccamcapture2.GetCaptureState() != START)
	{
		m_Pause.SetWindowTextW(L"Pause");
		
	}

	if (ccamcapture2.GetCaptureState() != PAUSE)
	{
		ccamcapture2.PauseCapture();
		
		m_Cam_Start.EnableWindow(0);
		m_Pause.SetWindowTextW(L"Continue");
	}
	else if (ccamcapture2.GetCaptureState() == PAUSE)
	{
		ccamcapture2.StartCapture();
		
		m_Cam_Start.EnableWindow(1);
		m_Pause.SetWindowTextW(L"Pause");
		
	}
	
}


void CMFCApplication3Dlg::CBN_SELENDOK_Method_Selection()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	Get_Combobox_Index = m_Method_Selection.GetCurSel();

	switch (Get_Combobox_Index)
	{
	case 0:

		m_Parameter1.SetWindowTextW(L"TM");
		m_Parameter2.SetWindowTextW(L"NULL");
		m_Parameter3.SetWindowTextW(L"NULL");
	
		m_SliderControl1.EnableWindow(1);
		m_SliderControl2.EnableWindow(0);
		m_SliderControl3.EnableWindow(0);

		m_Parameter1.EnableWindow(1);
		m_Parameter2.EnableWindow(0);
		m_Parameter3.EnableWindow(0);

		m_Variable1.EnableWindow(1);
		m_Variable2.EnableWindow(0);
		m_Variable3.EnableWindow(0);

		m_SliderControl1.SetRange(0, 10);
		m_SliderControl1.SetPos(0);

		break;

	case 1:

		m_Parameter1.SetWindowTextW(L"Vmin");
		m_Parameter2.SetWindowTextW(L"Vmax");
		m_Parameter3.SetWindowTextW(L"NULL");

		m_SliderControl1.EnableWindow(1);
		m_SliderControl2.EnableWindow(1);
		m_SliderControl3.EnableWindow(0);

		m_Parameter1.EnableWindow(1);
		m_Parameter2.EnableWindow(1);
		m_Parameter3.EnableWindow(0);

		m_Variable1.EnableWindow(1);
		m_Variable2.EnableWindow(1);
		m_Variable3.EnableWindow(0);

		m_SliderControl1.SetRange(0, 255);
		m_SliderControl1.SetPos(0);
		m_SliderControl2.SetRange(0, 255);
		m_SliderControl2.SetPos(0);


		break;

	case 2:
		m_Parameter1.SetWindowTextW(L"Vmin");
		m_Parameter2.SetWindowTextW(L"Vmax");
		m_Parameter3.SetWindowTextW(L"Smin");


		m_SliderControl1.EnableWindow(1);
		m_SliderControl2.EnableWindow(1);
		m_SliderControl3.EnableWindow(1);

		m_Parameter1.EnableWindow(1);
		m_Parameter2.EnableWindow(1);
		m_Parameter3.EnableWindow(1);

		m_Variable1.EnableWindow(1);
		m_Variable2.EnableWindow(1);
		m_Variable3.EnableWindow(1);

		m_SliderControl1.SetRange(0, 255);
		m_SliderControl1.SetPos(0);
		m_SliderControl2.SetRange(0, 255);
		m_SliderControl2.SetPos(0);
		m_SliderControl3.SetRange(0, 255);
		m_SliderControl3.SetPos(0);
		break;
	}



	
}


void CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼

	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl1.GetPos());
	m_Variable1.SetWindowTextW(buffer);

	*pResult = 0;
}


void CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl2.GetPos());
	m_Variable2.SetWindowTextW(buffer);
	*pResult = 0;
}


void CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl3.GetPos());
	m_Variable3.SetWindowTextW(buffer);
	*pResult = 0;
}

void CMFCApplication3Dlg::ShowImage(cv::Mat Image, CWnd* pWnd)
{
	//Windows中顯示圖像存在一個4位元組對齊的問題, 也就是每一行的位元組數必須是4的倍數.
	//而Mat的資料是連續存儲的.一般Mat的資料格式為BGR, 也就是一個圖元3個位元組, 假設我的圖片一行有5個圖元, 那一行就是15個位元組, 這不符合MFC的資料對齊方式,
	//如果我們直接把Mat的data加個資料頭再顯示出來就可能會出錯.
	//手動4位元組對齊, 就是計算每行的位元組是不是4的倍數, 不是的話, 在後面補0
	//但是我們把圖片轉成RGBA之後, 一個圖元就是4個位元組, 不管你一行幾個圖元, 一直都是對齊的.

	cv::Mat imgTmp;
	CRect rect;
	pWnd->GetClientRect(&rect);
	cv::resize(Image, imgTmp, cv::Size(rect.Width(), rect.Height()));

	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA);
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1);
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = pWnd->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	pWnd->ReleaseDC(pDC);
}


UINT CMFCApplication3Dlg::MyThread(LPVOID pVar)
{
	THREAD_INFO * Thread_Info = (THREAD_INFO *)pVar;
	cv::Rect trackwindow;
	bool isUseMouse = true;
	

	while (m_Continue)
	{
		CMFCApplication3Dlg *pDlg = (CMFCApplication3Dlg*)pVar;
		
			//cvSetMouseCallback("ShowImage", onMouse, 0);

			// 所要執行的程式碼放這裡
			capture >> image;
			image.copyTo(frame);
			//selection.x = selection.y = 0;
			//CMFCApplication3Dlg * hWnd = (CMFCApplication3Dlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
			//CWnd* pWnd_SHOW_IMAGE = (CWnd *)hWnd->GetDlgItem(IDC_SHOW_IMAGE);
			//ShowImage(image, pWnd_SHOW_IMAGE);
			//cvSetMouseCallback();
			cv::imshow("ShowImage", image);
			cvSetMouseCallback("ShowImage", onMouse, 0);
			//cvWaitKey(0);
		
	}
	capture.release();
	cvWaitKey(0);
	return 0;
}

void CMFCApplication3Dlg::OnOpenThread()		
{
	m_pThread = AfxBeginThread(MyThread, (LPVOID)&Thread_Info);

}

void CMFCApplication3Dlg::onMouse(int event, int x, int y, int flags, void* param)
{
	CvPoint p1, p2;
	if (select_flag == false && event == CV_EVENT_LBUTTONDOWN)
	{
		
		selection.x = x;
		selection.y = y;
		select_flag = true;

	}
	else if (select_flag == true && event == CV_EVENT_MOUSEMOVE)
	{
		
		p1 = cvPoint(selection.x, selection.y);
		p2 = cvPoint(x, y);
		rectangle(image, cv::Point(p1), cv::Point(p2), cv::Scalar(0, 255, 0), 2);
		cv::imshow("ShowImage", image);

	}
	else if (select_flag == true && event == CV_EVENT_LBUTTONUP)
	{
		cv::Rect roi = cv::Rect(cv::Point(selection.x, selection.y), cv::Point(x, y));
		if (roi.width && roi.height)  
		{   
			cv::Mat roiImg = frame(roi);
			cv::imshow("roi", roiImg);
			//waitKey(0);  
		}
		select_flag = false;
	}
}

//BOOL CMFCApplication3Dlg::CCamCapture::CreateCapture()
//{
//	cv::VideoCapture capture;
//	CWinThread* m_pThread;
//	capture.open(0);
//	if (capture.isOpened())
//	{
//		return TRUE;
//	}
//	else
//	{
//		if (g_bThreadContinue)
//		{
//			DWORD dwCode;
//			GetExitCodeThread(m_pThread->m_hThread, &dwCode);
//			TerminateThread(m_pThread->m_hThread, dwCode);
//			CloseHandle(m_pThread->m_hThread);
//		}
//		m_pThread = AfxBeginThread(MyThread, (LPVOID)&Thread_Info);
//	}
//	
//}

void CMFCApplication3Dlg::OnBnClickedInitcam()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	
}
void CMFCApplication3Dlg::CallbackinDlg(cv::Mat& Mat)
{
	cv::imshow("ShowImage2", Mat);
}