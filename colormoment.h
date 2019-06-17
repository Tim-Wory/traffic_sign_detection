#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

float calc3orderMom(Mat &channel)  //�������׾�
{
	uchar *p;
	float mom = 0;
	float m = mean(channel)[0];    //���㵥ͨ��ͼ��ľ�ֵ
	int nRows = channel.rows;
	int nCols = channel.cols;
	if (channel.isContinuous())     //�����洢����������ͼ��ɨ���ٶ�
	{
		nCols *= nRows;
		nRows = 1;
	}
	for (int i = 0; i < nRows; i++) //����������
	{
		p = channel.ptr<uchar>(i);
		for (int j = 0; j < nCols; j++)
			mom += pow((p[j] - m), 3);
	}
	float temp;
	temp = cvCbrt((float)(mom / (nRows*nCols)));    //���ֵ��������
	mom = temp;
	return mom;
}

//����9����ɫ�أ�3��ͨ����1��2��3�׾�
float *colorMom(Mat &img)
{
	float *Mom = new float[9];    //���9����ɫ��
	Mat hsvimg;
	if (img.channels() != 3)
		cout << "Error,input image must be a color image" << endl;
	Mat r(img.rows, img.cols, CV_8U);
	Mat g(img.rows, img.cols, CV_8U);
	Mat b(img.rows, img.cols, CV_8U);
	Mat channels[] = { b, g, r };
	split(img, channels);
	Mat tmp_m, tmp_sd;
	//����bͨ������ɫ��
	meanStdDev(b, tmp_m, tmp_sd);
	Mom[0] = (float)tmp_m.at<double>(0, 0);
	Mom[3] = (float)tmp_sd.at<double>(0, 0);
	Mom[6] = calc3orderMom(b);
	//  cout << Mom[0] << " " << Mom[1] << " " << Mom[2] << " " << endl;
	//����gͨ������ɫ��
	meanStdDev(g, tmp_m, tmp_sd);
	Mom[1] = (float)tmp_m.at<double>(0, 0);
	Mom[4] = (float)tmp_sd.at<double>(0, 0);
	Mom[7] = calc3orderMom(g);
	//  cout << Mom[3] << " " << Mom[4] << " " << Mom[5] << " " << endl;
	//����rͨ������ɫ��
	meanStdDev(r, tmp_m, tmp_sd);
	Mom[2] = (float)tmp_m.at<double>(0, 0);
	Mom[5] = (float)tmp_sd.at<double>(0, 0);
	Mom[8] = calc3orderMom(r);
	//  cout << Mom[6] << " " << Mom[7] << " " << Mom[8] << " " << endl;
	return Mom;//������ɫ������
}