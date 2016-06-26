// Stero_cali_diag.cpp : implementation file
//

#include "stdafx.h"
#include "CameraCalibrate.h"
#include "Stero_cali_diag.h"
#include "afxdialogex.h"
#include "Transcode.h"


// Stero_cali_diag dialog

IMPLEMENT_DYNAMIC(Stero_cali_diag, CDialogEx)

Stero_cali_diag::Stero_cali_diag(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DUAL_CONF_SELECT, pParent)
	, left_conf(_T(""))
	, m_left_img_path(_T(""))
	, m_right_img_path(_T(""))
	, m_pairs(_T(""))
	, save_path(_T(""))
{

}

Stero_cali_diag::~Stero_cali_diag()
{
}

void Stero_cali_diag::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEFT, left_conf);
	DDX_Text(pDX, IDC_EDIT_LEFT_IMG, m_left_img_path);
	DDX_Text(pDX, IDC_EDIT_RIGHT_IMG, m_right_img_path);
	DDX_Text(pDX, IDC_EDIT2, m_pairs);
	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, save_path);
}


BEGIN_MESSAGE_MAP(Stero_cali_diag, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &Stero_cali_diag::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDOK, &Stero_cali_diag::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_IMG, &Stero_cali_diag::OnBnClickedButtonLeftImg)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_IMG, &Stero_cali_diag::OnBnClickedButtonRightImg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PATH, &Stero_cali_diag::OnBnClickedButtonSavePath)
END_MESSAGE_MAP()


void Stero_cali_diag::OnBnClickedButtonLeft()
{
	UpdateData(true);
	CFileDialog dlg(TRUE, //TRUE for open
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_TEXT("Configure Files (*.yaml)|*.yaml||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		left_conf = dlg.GetPathName();
		load_img_file_path(left_conf, m_left_img_path);
		UpdateData(false);
	}
	else
	{
		return;
	}
}

void Stero_cali_diag::OnBnClickedOk()
{
	UpdateData(true);
	if (m_pairs == L"")
	{
		AfxMessageBox(_T("You should input pairs number."));
		return;
	}
	if (left_conf.IsEmpty())
	{
		AfxMessageBox(_T("You should select calibration files first."));
		return;
	}
	if (m_left_img_path.IsEmpty() || m_right_img_path.IsEmpty())
	{
		AfxMessageBox(_T("You should select left or right calibration images first."));
		return;
	}
	if (save_path.IsEmpty())
	{
		AfxMessageBox(_T("You should specify a save path."));
		return;
	}
	CDialogEx::OnOK();
}


void Stero_cali_diag::OnBnClickedButtonLeftImg()
{
	UpdateData(true);
	CFileDialog dlg(TRUE, //TRUE for open
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_TEXT("JPG Files (*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_left_img_path = dlg.GetPathName();
		UpdateData(false);
	}
	else
	{
		return;
	}
}


void Stero_cali_diag::OnBnClickedButtonRightImg()
{
	UpdateData(true);
	CFileDialog dlg(TRUE, //TRUE for open
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_TEXT("JPG Files (*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		m_right_img_path = dlg.GetPathName();
		UpdateData(false);
	}
	else
	{
		return;
	}
}

void Stero_cali_diag::load_img_file_path(CString Conf_path, CString &img_path)
{
	int nLength = WideCharToMultiByte(CP_ACP, 0, Conf_path, Conf_path.GetLength(), NULL, 0, NULL, NULL);
	char *pBuffer = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, Conf_path, Conf_path.GetLength(), pBuffer, nLength, NULL, NULL);
	pBuffer[nLength] = 0;
	cv::FileStorage fs(pBuffer, cv::FileStorage::READ);
	delete pBuffer;
	string tmp_path;
	fs["img_path"] >> tmp_path;
	img_path = UTF8ToUnicode(tmp_path).c_str();
}


void Stero_cali_diag::OnBnClickedButtonSavePath()
{
	UpdateData(true);
	CFileDialog dlg(false, //TRUE for open
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_TEXT("Configure Files (*.yaml)|*.yaml||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		save_path = dlg.GetPathName();
		UpdateData(false);
	}
	else
	{
		return;
	}
}


BOOL Stero_cali_diag::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_pairs != L"") {
		this->GetDlgItem(IDC_EDIT2)->EnableWindow(false);
		this->GetDlgItem(IDC_BUTTON_SAVE_PATH)->EnableWindow(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
