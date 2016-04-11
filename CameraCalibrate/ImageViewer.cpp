// ImageViewer.cpp : implementation file
//

#include "stdafx.h"
#include "CameraCalibrate.h"
#include "CameraCalibrateDlg.h"
#include "ImageViewer.h"
#include "afxdialogex.h"


// ImageViewer dialog

IMPLEMENT_DYNAMIC(ImageViewer, CDialogEx)

ImageViewer::ImageViewer(CWnd* pParent, CString &Filepath)
	: CDialogEx(IDD_ImageViewer, pParent)
{
	pic_file = Filepath;
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImageViewer, CDialogEx)
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


// ImageViewer message handlers
RECT ImageViewer::get_desktop_size()
{
	HWND hwnd = ::GetDesktopWindow();
	RECT rect;
	::GetClientRect(hwnd, &rect);
	return rect;
}

BOOL ImageViewer::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	CCameraCalibrateDlg* menu = (CCameraCalibrateDlg*)this->GetParent();
	image.Load(pic_file);
	assert(!image.IsNull());
	RECT deskSize = get_desktop_size();
	img_src.top = 0;
	img_src.bottom = image.GetHeight();
	img_src.left = 0;
	img_src.right = image.GetWidth();
	int width = deskSize.right;
	int hight = deskSize.bottom;
	Draw_aera.top = 0;
	Draw_aera.left = 0;
	if (img_src.bottom > hight / 2 || img_src.right > width / 2)
	{
		//Set to 3/4 screen size
		SetWindowPos(NULL, width / 2 - width / 3, hight / 2 - hight / 3, int(width / 1.5f), int(hight / 1.5f), SWP_NOOWNERZORDER | SWP_NOZORDER);
		//Draw_aera.bottom = int(hight / 1.5f);
		//Draw_aera.right = int(width / 1.5f);
		Draw_aera.top = hight / 1.5f / 2 - img_src.bottom / 2;
		Draw_aera.left = width / 1.5f / 2 - img_src.right/2;
		Draw_aera.bottom = img_src.bottom + Draw_aera.top;
		Draw_aera.right = img_src.right + Draw_aera.left;
	}
	else
	{
		SetWindowPos(NULL, width / 2 - img_src.right / 2 , hight / 2 - img_src.bottom / 2, img_src.right+20, img_src.bottom+70, SWP_NOOWNERZORDER | SWP_NOZORDER);
		Draw_aera.top = 0;
		Draw_aera.left = 0;
		Draw_aera.bottom = img_src.bottom + Draw_aera.top;
		Draw_aera.right = img_src.right + Draw_aera.left;
	}
	GetWindowRect(&w_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ImageViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	dc.SetStretchBltMode(HALFTONE);
	image.Draw(dc,Draw_aera);
	std::vector<CPoint>::iterator iter;
	iter = m_points.begin();
	CPen Pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen *oldPen = dc.SelectObject(&Pen);
	for (; iter != m_points.end(); iter++)
	{
		int x = (*iter).x * zoom_factor + Draw_aera.left;
		int y = (*iter).y * zoom_factor + Draw_aera.top;
		dc.MoveTo(x - 10,y);
		dc.LineTo(x + 10,y);
		dc.MoveTo(x, y - 10);
		dc.LineTo(x, y + 10);
	}
	dc.SelectObject(oldPen);
}

void ImageViewer::do_zoom(float zoomfactor,CPoint point)
{
	zoom_factor = zoomfactor;
	Draw_aera.top = (w_rect.bottom - w_rect.top) / 2 - img_src.bottom * zoom_factor / 2;
	Draw_aera.left = (w_rect.right - w_rect.left) / 2 - img_src.right * zoom_factor / 2;
	Draw_aera.bottom = img_src.bottom * zoom_factor + Draw_aera.top;
	Draw_aera.right = img_src.right * zoom_factor + Draw_aera.left;
	Invalidate();
}

BOOL ImageViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta < 0)
	{
		if ((zoom_factor + zDelta * 0.001f) < 1.0f)
			do_zoom(1.0f,pt);
		else
			do_zoom(zoom_factor + zDelta * 0.001f,pt);
	}
	else
		do_zoom(zoom_factor + zDelta * 0.001f,pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL ImageViewer::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	if (Draw_aera.top>0 || Draw_aera.left>0 || Draw_aera.bottom < (w_rect.bottom- w_rect.top) || Draw_aera.right < (w_rect.right - w_rect.left))
		return CDialogEx::OnEraseBkgnd(pDC);
	else
		return TRUE;
}


void ImageViewer::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	moving = true;
	rec_mouse = point;
	CDialogEx::OnRButtonDown(nFlags, point);
}


void ImageViewer::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	moving = false;
	CDialogEx::OnRButtonUp(nFlags, point);
}


void ImageViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int x = (point.x - Draw_aera.left) / zoom_factor;
	int y = (point.y - Draw_aera.top) / zoom_factor;
	m_points.push_back(CPoint(x, y));
	/*
	CString test;
	test.Format(_T("%d,%d"), x, y);
	AfxMessageBox(test);
	*/
	Invalidate();
	CDialogEx::OnLButtonDown(nFlags, point);
}


void ImageViewer::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (moving)
	{
		do_move(point.x- rec_mouse.x, point.y - rec_mouse.y);
		rec_mouse = point;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void ImageViewer::do_move(int x, int y)
{
	Draw_aera.top = Draw_aera.top + y;
	Draw_aera.left = Draw_aera.left + x;
	Draw_aera.bottom = Draw_aera.bottom + y;
	Draw_aera.right = Draw_aera.right + x;
	Invalidate();
}

void ImageViewer::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	std::vector<CPoint>::iterator iter;
	iter = m_points.begin();
	for (; iter != m_points.end();)
	{
		int x = (*iter).x * zoom_factor + Draw_aera.left;
		int y = (*iter).y * zoom_factor + Draw_aera.top;
		if (abs(x - point.x) <= 10 && abs(y - point.y) <= 10)
		{
			iter = m_points.erase(iter);
			Invalidate();
		}
		else
			iter++;
	}
	CDialogEx::OnMButtonDown(nFlags, point);
}

void ImageViewer::load_points(std::vector<CPoint> points) {
	this->m_points = points;
}