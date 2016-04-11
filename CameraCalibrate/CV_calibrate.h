#pragma once
#pragma comment(lib, "opencv_world310d.lib")
#include "stdafx.h"
//#include <opencv2/core.hpp>
//#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;
enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
class cv_cali
{
public:
	cv_cali();
	virtual ~cv_cali();
	void loadCameraParams(CString FilePath, Mat &cameraMatrix, Mat &distCoeffs);
	double computeReprojectionErrors(
		const vector<vector<Point3f> >& objectPoints,
		const vector<vector<Point2f> >& imagePoints,
		const vector<Mat>& rvecs, const vector<Mat>& tvecs,
		const Mat& cameraMatrix, const Mat& distCoeffs,
		vector<float>& perViewErrors);
	bool do_cali(vector<vector<Point2f> > imagePoints,
		Size imageSize, Size boardSize, Pattern patternType,
		float squareSize, float aspectRatio,
		int flags, Mat& cameraMatrix, Mat& distCoeffs,
		vector<Mat>& rvecs, vector<Mat>& tvecs,
		vector<float>& reprojErrs,
		double& totalAvgErr);
	void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners, Pattern patternType);
	void saveCameraParams(CString filename,
		Size imageSize, Size boardSize,
		float squareSize, float aspectRatio, int flags,
		const Mat& cameraMatrix, const Mat& distCoeffs,
		const vector<Mat>& rvecs, const vector<Mat>& tvecs,
		const vector<float>& reprojErrs,
		const vector<vector<Point2f> >& imagePoints,
		double totalAvgErr);
	bool runAndSave(CString outputFilename,
		const vector<vector<Point2f> >& imagePoints,
		Size imageSize, Size boardSize, Pattern patternType, float squareSize,
		float aspectRatio, int flags, Mat& cameraMatrix,
		Mat& distCoeffs, bool writeExtrinsics, bool writePoints);
	vector<Point3f> conv_cornors3f(vector<CPoint> points);
	vector<Point2f> conv_cornors2f(vector<CPoint> points);
	Size boardSize, imageSize;
	float squareSize=1.0f, aspectRatio=1.0f;
	Mat cameraMatrix, distCoeffs;
	vector<vector<Point2f> > imagePoints;
	int flags = 0;
};
//only one calibration class
extern cv_cali *cali;