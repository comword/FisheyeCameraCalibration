#pragma once
#include "afxwin.h"
#include <vector>
#include <map>
// Cali_options dialog
using Corn_file = std::map<CString, std::vector<CPoint>>;
class Cali_options : public CDialogEx
{
	DECLARE_DYNAMIC(Cali_options)

public:
	Cali_options(CWnd* pParent = NULL);   // standard constructor
	virtual ~Cali_options();
	Corn_file m_maps;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALI_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString board_height;
	CString board_width;
	afx_msg void OnBnClickedLoadpic();
	virtual BOOL OnInitDialog();
	CListBox m_list;
	afx_msg void OnBnClickedRemovepics();
	afx_msg void OnBnClickedSetcorners();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAutocorners();
	afx_msg void OnBnClickedSavepath();
	CString conf_dict;
};