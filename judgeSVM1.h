//#-*-coding=utf-8
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <fstream>
#include "stdlib.h"
//���������ռ�
using namespace std;
using namespace cv;
using namespace cv::ml;
#define s 10000
//!ѵ�����ݲ���
const int sample_num_perclass1 = 33;     //ѵ��ÿ��ͼƬ����
const int class_num1 = 3;                //ѵ������
//!����ͼƬ�ߴ��һ��
const int image_cols1 = 800;              //����ͼƬ�ߴ�
const int image_rows1 = 800;              //����ͼƬ�ߴ�
//!���ɵ�ѵ���ļ�����λ��
char SVMName1[40] = "C://judge1//SVM1.xml";              //��������ѵ�����ɵ�����,��ȡʱҲ�������������
#define RW1 0//0Ϊ��ȡ���еķ�����,1��ʾ����ѵ��һ��������
//!�������
Size size1 = Size(image_cols1, image_rows1);
void judgesvm1(Mat &src, int classify)
{
if RW1   
	//!��ȡѵ������
	Mat trainingData = Mat::zeros(sample_num_perclass1*class_num1, s, CV_32FC1);          //����ͼ���7��Hu��

	Mat trainingLabel = Mat::zeros(sample_num_perclass1*class_num1, 1, CV_32SC1);
	vector<float>descriptors;//������� 
	char buf[50];                       //�ַ�������
	for (int i = 0; i<class_num1; i++)        //��ͬ�����ѭ��
	{
		for (int j = 0; j<sample_num_perclass1; j++)      //һ�����е�ͼƬ����
		{
			//!����ͼƬ��·��(��ͬ���ͼƬ�������˲�ͬ���ļ�����)
			sprintf(buf, "C://judge%d//%d//%d.jpg", classify, i, j + 1);
			//!��ȡ
			Mat src = imread(buf, -1);
			//!����ߴ磨��һ����
			Mat reImg;
			resize(src, reImg, size1, CV_INTER_CUBIC);
			Mat canny;
			Canny(reImg, canny, 50, 200, 3);
			//!��Hu��
			HOGDescriptor *hog = new HOGDescriptor(Size(image_cols1, image_rows1), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);  //������˼���ο�����1,2             
			hog->compute(reImg, descriptors, Size(1, 1), Size(0, 0));
			//!��Hu������ѵ�����ݼ���
			float *dstPoi = trainingData.ptr<float>(i*sample_num_perclass1 + j);  //ָ��Դ��ָ��
			for (int r = 0; r<s; r++)
				dstPoi[r] = (float)descriptors[r];
			//!��ӶԸ����ݵķ����ǩ
			int *labPoi = trainingLabel.ptr<int>(i*sample_num_perclass1 + j);
			labPoi[0] = i;
		}
	}
	imwrite("res.png", trainingData);
	//cout << descriptors.size() << endl;
	//!����SVM֧����������ѵ������
	Ptr<SVM> svm1 = SVM::create();
	svm1->setType(SVM::C_SVC);
	svm1->setC(0.01);
	svm1->setKernel(SVM::LINEAR);
	svm1->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm1->train(trainingData, ROW_SAMPLE, trainingLabel);
	svm1->save(SVMName1);
#else
	//��ȡxml�ļ�
	Ptr<SVM> svm1 = StatModel::load<SVM>(SVMName1);
#endif
	//!��ȡһ��ͼƬ���в���
	Mat temp1 = imread("C://Users//yuanze//Desktop//40test3.jpg");
	//cvtColor(temp1, temp1, CV_RGB2GRAY);
	//cout << temp1 << endl;
	//cout << temp.channels() << endl;
	/*Mat temp;
	temp = src;*/
	//cout << temp << endl;

	Mat dst;
	resize(temp1, dst, size1, CV_INTER_CUBIC);
	HOGDescriptor *hog = new HOGDescriptor(Size(image_cols1, image_rows1), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);  //������˼���ο�����1,2       
	vector<float>descriptor;//�������       
	hog->compute(dst, descriptor, Size(1, 1), Size(0, 0));
	Mat pre(1, s, CV_32FC1);
	float *p = pre.ptr<float>(0);
	for (int i = 0; i<s; i++)
		p[i] = descriptor[i];

	int res = svm1->predict(pre);
	//cout << res << endl;
	switch (res)
	{
	case 0:cout << "��ת" << endl; break;
	case 1:cout << "��ת" << endl; break;
	case 2:cout << "ֱ��" << endl; break;
	}
}