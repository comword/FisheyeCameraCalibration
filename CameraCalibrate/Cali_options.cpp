// Cali_options.cpp : implementation file
//

#include "stdafx.h"
#include "CameraCalibrate.h"
#include "Cali_options.h"
#include "afxdialogex.h"
#include "ImageViewer.h"
#include "CameraCalibrateDlg.h"
#include "CV_calibrate.h"


// Cali_options dialog

IMPLEMENT_DYNAMIC(Cali_options, CDialogEx)

Cali_options::Cali_options(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALI_OPTIONS, pParent)
	, conf_dict(_T(""))
{

}

Cali_options::~Cali_options()
{
}

void Cali_options::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Board_height, board_height);
	DDX_Text(pDX, IDC_Board_height2, board_width);
	DDX_Control(pDX, IDC_PICSLIST, m_list);
	DDX_Text(pDX, IDC_SAVEPATH_DIS, conf_dict);
}


BEGIN_MESSAGE_MAP(Cali_options, CDialogEx)
	ON_BN_CLICKED(IDC_LOADPIC, &Cali_options::OnBnClickedLoadpic)
	ON_BN_CLICKED(IDC_REMOVEPICS, &Cali_options::OnBnClickedRemovepics)
	ON_BN_CLICKED(IDC_SETCORNERS, &Cali_options::OnBnClickedSetcorners)
	ON_BN_CLICKED(IDOK, &Cali_options::OnBnClickedOk)
	ON_BN_CLICKED(IDC_AUTOCORNERS, &Cali_options::OnBnClickedAutocorners)
	ON_BN_CLICKED(IDC_SAVEPATH, &Cali_options::OnBnClickedSavepath)
END_MESSAGE_MAP()


// Cali_options message handlers


void Cali_options::OnBnClickedLoadpic()
{
	CFileDialog dlg(TRUE, //TRUE for open
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
		_TEXT("JPG Files (*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		if (m_list.FindString(0, dlg.GetPathName()) == LB_ERR) {
			POSITION pos;
			pos = dlg.GetStartPosition();  
			while (pos != NULL){
				CString filename = dlg.GetNextPathName(pos);
				m_list.AddString(filename);
				}
			m_list.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
		}
		else
			AfxMessageBox(L"Image has already been opened.");
	}
	else
	{
		return;
	}
}


BOOL Cali_options::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CCameraCalibrateDlg *parent_dlg = (CCameraCalibrateDlg *)this->GetParent();
	board_height.Format(L"%d", parent_dlg->board_h);
	board_width.Format(L"%d", parent_dlg->board_w);
	conf_dict.Format(L"%s", parent_dlg->Config_directory);
	UpdateData(false);
	m_maps = parent_dlg->m_maps;
	for (auto &e : m_maps) {
		m_list.AddString(e.first);
	}
	m_list.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void Cali_options::OnBnClickedRemovepics()
{
	// TODO: Add your control notification handler code here
	int nCount = m_list.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_list.GetSel(i) > 0) {
			CString tmp;
			m_list.GetText(i, tmp);
			if (m_maps.find(tmp) != m_maps.end())
				m_maps.erase(tmp);
			m_list.DeleteString(i);
		}
	}
}


void Cali_options::OnBnClickedSetcorners()
{
	// TODO: Add your control notification handler code here
	std::vector<CString> file_list;
	int nCount = m_list.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_list.GetSel(i) > 0) {
			CString tmp;
			m_list.GetText(i, tmp);
			file_list.push_back(tmp);
		}
	}
	std::vector<CString>::iterator iter;
	iter = file_list.begin();
	for (; iter != file_list.end(); iter++)
	{
		ImageViewer viewer(this,*iter);
		if (m_maps.find(*iter) != m_maps.end())
			viewer.load_points(m_maps[*iter]);
		viewer.DoModal();
		m_maps[*iter] = viewer.m_points;
	}
}


void Cali_options::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if (_ttoi(board_height) <= 0 || _ttoi(board_width) <= 0) {
		AfxMessageBox(L"Please input board size correctly.");
		return;
	}
	if (conf_dict == L"") {
		AfxMessageBox(L"Please select directory to save configure files.");
		return;
	}
	if (m_maps.empty()) {
		AfxMessageBox(L"Please add at least one picture and set corners correctly.");
		return;
	}
	CDialogEx::OnOK();
}


void Cali_options::OnBnClickedAutocorners()
{
	// TODO: Add your control notification handler code here
	std::vector<CString> file_list;
	int nCount = m_list.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_list.GetSel(i) > 0) {
			CString tmp;
			m_list.GetText(i, tmp);
			file_list.push_back(tmp);
		}
	}
	std::vector<CString>::iterator iter;
	iter = file_list.begin();
	for (; iter != file_list.end(); iter++)
	{
		bool found;
		UpdateData(true);
		if (_ttoi(board_height) <= 0 || _ttoi(board_width) <= 0) {
			AfxMessageBox(L"Please input board size correctly.");
			return;
		}
		cali->boardSize.height = _ttoi(board_height);
		cali->boardSize.width = _ttoi(board_width);
		CString filename = *iter;
		int nLength = WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), NULL, 0, NULL, NULL);
		char *pBuffer = new char[nLength + 1];
		WideCharToMultiByte(CP_ACP, 0, filename, filename.GetLength(), pBuffer, nLength, NULL, NULL);
		pBuffer[nLength] = 0;
		vector<Point2f> points;
		cv::Mat view = cv::imread(pBuffer, 1);
		cv::Mat viewGray;
		cvtColor(view, viewGray, COLOR_BGR2GRAY);
		found = findChessboardCorners(view, cali->boardSize, points, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
		cornerSubPix(viewGray, points, Size(11, 11),
			Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
		if (found) {
			vector<CPoint> res;
			vector<Point2f>::iterator itera;
			itera = points.begin();
			for (; itera != points.end(); itera++)
			{
				res.push_back(CPoint(float((*itera).x),
					float((*itera).y)));
			}
			m_maps[*iter] = res;
		}
	}
}


void Cali_options::OnBnClickedSavepath()
{
	// TODO: Add your control notification handler code here
	wchar_t buffer[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = buffer;
	bi.lpszTitle = _T("Please select directory to save configure files:");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, buffer)){
		CString tmp;
		tmp.Format(L"%s", buffer);
		this->GetDlgItem(IDC_SAVEPATH_DIS)->SetWindowText(tmp);
	}
}
