#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int findCircles1(Mat src, Mat midImage,Mat &src1, Mat &imageroi, double dThreshold1, double dThreshold2, int iSize, int iMinArea)
{
	int count = 0;
	Mat imageROI, src0;
	Canny(src, src0, dThreshold1, dThreshold2, iSize);
	//������  
	vector<vector<Point>> q_vPointContours1;
	findContours(src0, q_vPointContours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	//drawContours(src0, q_vPointContours, -1, Scalar(0,255,0));//��ʾ����  
	//ɸѡĿ��������  
	vector<Point> vfindContours1;
	size_t q_iAmountContours1 = q_vPointContours1.size();
	size_t iIndex = 0;
	for (iIndex = 0; iIndex < q_iAmountContours1; iIndex++)
	{
		//����Բ������ж��Ƿ�ΪĿ��Բ  
		double ddarea1 = contourArea(q_vPointContours1[iIndex]);
		if (iMinArea >ddarea1)
			continue;
		else
			count++;
		//cout << ddarea1 << endl;
		//�洢Ŀ��Բ��������  
		size_t findCount1 = q_vPointContours1[iIndex].size();
		for (int i = 0; i < findCount1; i++)
			vfindContours1.push_back(q_vPointContours1[iIndex][i]);

		//������Բ������õ�Բ  
		RotatedRect rectElli1 = fitEllipse(vfindContours1);
		float fR = MIN(rectElli1.size.width, rectElli1.size.height);// �Ƿ�ΪԲ�����ԱȽ�������ֵ����ʮ�ֽӽ�����ȣ�����һ����Բ  
		//cout << "fitEllipse ����: " << rectElli1.center.x << ", " << rectElli1.center.y << "  �뾶:" << fR / 2 << endl;
		circle(midImage, Point(rectElli1.center), fR / 2, Scalar(0, 0, 0), 2);//Բ��  
		circle(midImage, Point(rectElli1.center), 5, Scalar(0, 0, 0), 3);//Բ��  
		vector<Rect>boundRect1(q_vPointContours1.size());
		int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
		boundRect1[iIndex] = boundingRect((Mat)q_vPointContours1[iIndex]);
		x0 = boundRect1[iIndex].x;  //��õ�i����Ӿ��ε����Ͻǵ�x����
		y0 = boundRect1[iIndex].y; //��õ�i����Ӿ��ε����Ͻǵ�y����
		w0 = boundRect1[iIndex].width; //��õ�i����Ӿ��εĿ��
		h0 = boundRect1[iIndex].height; //��õ�i����Ӿ��εĸ߶�
		rectangle(midImage, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8); //���Ƶ�i����Ӿ���
		rectangle(src1, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8); //���Ƶ�i����Ӿ���

		//cout << w0 << endl;
		//cout << h0 << endl;
		imageroi= src(Rect(x0, y0, w0, h0));
		
	//	imshow("ROI", imageROI);
	}
	return count;
}