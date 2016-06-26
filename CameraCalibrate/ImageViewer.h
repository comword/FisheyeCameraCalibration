#pragma once
#include <vector>

// ImageViewer dialog

class ImageViewer : public CDialogEx
{
	DECLARE_DYNAMIC(ImageViewer)

public:
	ImageViewer(CWnd* pParent,CString &Filepath);   // standard constructor
	virtual ~ImageViewer();
	void do_zoom(float zoomfactor, CPoint point);
	void do_move(int x,int y);
	std::vector<CPoint> m_points;
	CString pic_file;
	void load_points(std::vector<CPoint> points);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImageViewer };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	RECT get_desktop_size();
	RECT Draw_aera;
	RECT img_src;
	RECT w_rect;
	CImage image;
	bool moving = false;
	int pos_point = -1;
	CPoint rec_mouse;
	float zoom_factor = 1.0f;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
};
