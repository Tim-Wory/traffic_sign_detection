#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int findCircles(Mat src, Mat midImage,Mat &src11, Mat src1,Mat &src1m, Mat &imageroi, double dThreshold1, double dThreshold2, int iSize, int iMinArea)
{
	int count = 0;
	Mat imageROI, src0;
	Canny(src, src0, dThreshold1, dThreshold2, iSize);
	//������  
	vector<vector<Point>> q_vPointContours;
	findContours(src0, q_vPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	//drawContours(src0, q_vPointContours, -1, Scalar(0,255,0));//��ʾ����  
	//ɸѡĿ��������  
	vector<Point> vfindContours(0);
	size_t q_iAmountContours = q_vPointContours.size();
	size_t iIndex = 0;
	for (iIndex = 0; iIndex < q_iAmountContours; iIndex++)
	{
		//����Բ������ж��Ƿ�ΪĿ��Բ  
		double ddarea = contourArea(q_vPointContours[iIndex]);
		if (iMinArea >ddarea)
			continue;
		else
			count++;
		//cout << ddarea << endl;
		//�洢Ŀ��Բ��������  
		size_t findCount = q_vPointContours[iIndex].size();
		for (int i = 0; i < findCount; i++)
			vfindContours.push_back(q_vPointContours[iIndex][i]);

		//������Բ������õ�Բ  
		RotatedRect rectElli = fitEllipse(vfindContours);
		float fR = MIN(rectElli.size.width, rectElli.size.height);// �Ƿ�ΪԲ�����ԱȽ�������ֵ����ʮ�ֽӽ�����ȣ�����һ����Բ  
		//cout << "fitEllipse ����: " << rectElli.center.x << ", " << rectElli.center.y << "  �뾶:" << fR / 2 << endl;
		circle(midImage, Point(rectElli.center), fR / 2, Scalar(0, 0, 0), 2);//Բ��  
		circle(midImage, Point(rectElli.center), 5, Scalar(0, 0, 0), 3);//Բ��  
		vector<Rect>boundRect(q_vPointContours.size());
		int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
		boundRect[iIndex] = boundingRect((Mat)q_vPointContours[iIndex]);
		x0 = boundRect[iIndex].x;  //��õ�i����Ӿ��ε����Ͻǵ�x����
		y0 = boundRect[iIndex].y; //��õ�i����Ӿ��ε����Ͻǵ�y����
		w0 = boundRect[iIndex].width; //��õ�i����Ӿ��εĿ��
		h0 = boundRect[iIndex].height; //��õ�i����Ӿ��εĸ߶�
		rectangle(midImage, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8); //���Ƶ�i����Ӿ���
		rectangle(src1, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8); //���Ƶ�i����Ӿ���
		rectangle(src11, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8); //���Ƶ�i����Ӿ���
		//cout << w0 << endl;
		//cout << w0 << endl;
		//cout << h0 << endl;
		imageroi = src(Rect(x0, y0, w0, h0));
		//imageroi = imageROI;
	//	imshow("ROI", imageroi);
		src1m= src1(Rect(x0, y0, w0, h0));
	}
	return count;
}