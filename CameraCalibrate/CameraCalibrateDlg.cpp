
// CameraCalibrateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CameraCalibrate.h"
#include "CameraCalibrateDlg.h"
#include "afxdialogex.h"
#include "CV_calibrate.h"
#include "Cali_options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

cv_cali *cali;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CCameraCalibrateDlg dialog



CCameraCalibrateDlg::CCameraCalibrateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMERACALIBRATE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraCalibrateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCameraCalibrateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPENNEW, &CCameraCalibrateDlg::OnBnClickedOpennew)
	ON_BN_CLICKED(ID_DOCALIBRATE, &CCameraCalibrateDlg::OnBnClickedDocalibrate)
	ON_BN_CLICKED(ID_LOAD_CONF, &CCameraCalibrateDlg::OnBnClickedLoadConf)
	ON_BN_CLICKED(ID_PROC_PICS, &CCameraCalibrateDlg::OnBnClickedProcPics)
END_MESSAGE_MAP()


// CCameraCalibrateDlg message handlers

BOOL CCameraCalibrateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	cali = new cv_cali();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCameraCalibrateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCameraCalibrateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCameraCalibrateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCameraCalibrateDlg::OnBnClickedOpennew()
{
	// TODO: Add your control notification handler code here
	Cali_options opt;
	opt.DoModal();
	this->m_maps = opt.m_maps;
	this->board_h = _ttoi(opt.board_height);
	this->board_w = _ttoi(opt.board_width);
	this->Config_directory = opt.conf_dict;
	if (!(m_maps.empty() || board_h == 0 || board_w == 0))
		this->GetDlgItem(ID_DOCALIBRATE)->EnableWindow();
	else
		this->GetDlgItem(ID_DOCALIBRATE)->EnableWindow(false);
}

void CCameraCalibrateDlg::OnBnClickedDocalibrate()
{
	// TODO: Add your control notification handler code here
	int nframes = (int)m_maps.size();
	cv::namedWindow("Calibration", 1);
	for (auto &e : m_maps) {
		cali->imagePoints.push_back(cali->conv_cornors2f(e.second));
	}
	for (auto &e : m_maps) {
		CString filename = e.first;
		int nLength = WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), NULL, 0, NULL, NULL);
		char *pBuffer = new char[nLength + 1];
		WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), pBuffer, nLength, NULL, NULL);
		pBuffer[nLength] = 0;
		cv::Mat view = cv::imread(pBuffer, 1);
		if (view.empty())
			break;
		cali->imageSize = view.size();
		cali->boardSize.height = board_h;
		cali->boardSize.width = board_w;
		vector<Point2f> pointbuf = cali->conv_cornors2f(e.second);
		cv::drawChessboardCorners(view, cali->boardSize, Mat(pointbuf), true);
		string msg = "Press 's' to start";
		int baseLine = 0;
		Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
		Point textOrigin(view.cols - 2 * textSize.width - 10, view.rows - 2 * baseLine - 10);
		putText(view, msg, textOrigin, 1, 1,Scalar(0, 255, 255));
		imshow("Calibration", view);
		int key = 0xff & waitKey();
		if ((key & 255) == 27)
			break;
		if (key == 's') {
			CString strFileName = filename.Mid(filename.ReverseFind('\\') + 1);
			strFileName += L".yaml";
			CString outputFilename = Config_directory + L"\\" + strFileName;
			cali->runAndSave(outputFilename, cali->imagePoints, cali->imageSize,
				cali->boardSize, CHESSBOARD, cali->squareSize, cali->aspectRatio,
				cali->flags, cali->cameraMatrix, cali->distCoeffs,
				true, true);
		}
	}
	Mat view, rview, map1, map2;
	initUndistortRectifyMap(cali->cameraMatrix, cali->distCoeffs, Mat(),
		getOptimalNewCameraMatrix(cali->cameraMatrix, cali->distCoeffs, cali->imageSize, 1, cali->imageSize, 0),
		cali->imageSize, CV_16SC2, map1, map2);
	for (auto &e : m_maps) {
		CString filename = e.first;
		int nLength = WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), NULL, 0, NULL, NULL);
		char *pBuffer = new char[nLength + 1];
		WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), pBuffer, nLength, NULL, NULL);
		pBuffer[nLength] = 0;
		view = imread(pBuffer, 1);
		if (view.empty())
			continue;
		//undistort( view, rview, cameraMatrix, distCoeffs, cameraMatrix );
		remap(view, rview, map1, map2, INTER_LINEAR);
		imshow("Calibration", rview);
		int c = 0xff & waitKey();
		if ((c & 255) == 27 || c == 'q' || c == 'Q')
			break;
	}
}


BOOL CCameraCalibrateDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	delete cali;
	return CDialogEx::DestroyWindow();
}


void CCameraCalibrateDlg::OnBnClickedLoadConf()
{
	CFileDialog dlg(TRUE, //TRUE for open
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_TEXT("Configure Files (*.yaml)|*.yaml||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		ConfigPath = dlg.GetPathName();
	}
	else
	{
		return;
	}
	cali->loadCameraParams(ConfigPath,cali->cameraMatrix,cali->distCoeffs);
	this->GetDlgItem(ID_PROC_PICS)->EnableWindow();
	// TODO: Add your control notification handler code here
}


void CCameraCalibrateDlg::OnBnClickedProcPics()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, //TRUE for open
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_TEXT("JPG Files (*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString filename = dlg.GetPathName();
		int nLength = WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), NULL, 0, NULL, NULL);
		char *pBuffer = new char[nLength + 1];
		WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), pBuffer, nLength, NULL, NULL);
		pBuffer[nLength] = 0;
		cv::Mat view = cv::imread(pBuffer, 1);
		cali->imageSize = view.size();
		if (view.empty())
			return;
		Mat rview, map1, map2;
		initUndistortRectifyMap(cali->cameraMatrix, cali->distCoeffs, Mat(),
			getOptimalNewCameraMatrix(cali->cameraMatrix, cali->distCoeffs, cali->imageSize, 1, cali->imageSize, 0),
			cali->imageSize, CV_16SC2, map1, map2);
		remap(view, rview, map1, map2, INTER_LINEAR);
		imshow("Calibration", rview);
		int c = 0xff & waitKey();
		if ((c & 255) == 27 || c == 'q' || c == 'Q')
			return;
	}
	else
	{
		return;
	}
}
