
// CameraCalibrateDlg.h : header file
//

#pragma once

#include <vector>
#include <map>
using Corn_file = std::map<CString, std::vector<CPoint>>;
// CCameraCalibrateDlg dialog
class CCameraCalibrateDlg : public CDialogEx
{
// Construction
public:
	CCameraCalibrateDlg(CWnd* pParent = NULL);	// standard constructor
	CString ConfigPath, Config_directory = _T("");
	Corn_file m_maps;
	int board_h = 0 , board_w = 0;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERACALIBRATE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpennew();
	afx_msg void OnBnClickedDocalibrate();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedLoadConf();
	afx_msg void OnBnClickedProcPics();
};
