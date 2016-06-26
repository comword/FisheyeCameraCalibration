#pragma once
// Stero_cali_diag dialog
#include "CV_calibrate.h"
class Stero_cali_diag : public CDialogEx
{
	DECLARE_DYNAMIC(Stero_cali_diag)

public:
	Stero_cali_diag(CWnd* pParent = NULL);   // standard constructor
	virtual ~Stero_cali_diag();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUAL_CONF_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLeft();
	CString left_conf;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonLeftImg();
	afx_msg void OnBnClickedButtonRightImg();
	CString m_left_img_path;
	CString m_right_img_path;
protected:
	void load_img_file_path(CString Conf_path, CString & img_path);
public:
	CString m_pairs;
	afx_msg void OnBnClickedButtonSavePath();
	virtual BOOL OnInitDialog();
	CString save_path;
};
