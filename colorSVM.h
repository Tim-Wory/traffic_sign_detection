#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include"colormoment.h"
//���������ռ�
using namespace std;
using namespace cv;
//using namespace cv::ml;
#define s 10000
//!ѵ�����ݲ���
const int sample_num_perclass = 20;     //ѵ��ÿ��ͼƬ����
const int class_num = 3;                //ѵ������
//!����ͼƬ�ߴ��һ��
const int image_cols = 1000;              //����ͼƬ�ߴ�
const int image_rows = 1000;              //����ͼƬ�ߴ�
//!���ɵ�ѵ���ļ�����λ��
char SVMName[40] = "C://judgecolor//SVM.xml";              //��������ѵ�����ɵ�����,��ȡʱҲ�������������
#define RW 0//0Ϊ��ȡ���еķ�����,1��ʾ����ѵ��һ��������
Size size = Size(image_cols, image_rows);
int colorsvm(Mat src)
{
#if RW  
	//!��ȡѵ������
	Mat trainingData = Mat::zeros(sample_num_perclass*class_num, 9, CV_32FC1);          //����ͼ���7��Hu��

	Mat trainingLabel = Mat::zeros(sample_num_perclass*class_num, 1, CV_32SC1);
	vector<float>descriptors;//������� 
	char buf[50];                       //�ַ�������
	for (int i = 0; i<class_num; i++)        //��ͬ�����ѭ��
	{
		for (int j = 0; j<sample_num_perclass; j++)      //һ�����е�ͼƬ����
		{
			//!����ͼƬ��·��(��ͬ���ͼƬ�������˲�ͬ���ļ�����)
			sprintf(buf, "C://judgecolor//%d//%d.jpg", i, j + 1);
			//!��ȡ
			Mat src = imread(buf, 1);
			//!����ߴ磨��һ����
			Mat reImg;
			resize(src, reImg, size, CV_INTER_CUBIC);

			//!��Hu��
			float *descriptors;
			float *dstPoi = trainingData.ptr<float>(i*sample_num_perclass + j);  //ָ��Դ��ָ��
			descriptors = colorMom(reImg);
			for (int r = 0; r < 9; r++)
				dstPoi[r] = descriptors[r];
			//!��ӶԸ����ݵķ����ǩ
			int *labPoi = trainingLabel.ptr<int>(i*sample_num_perclass + j);
			labPoi[0] = i;
		}
	}
	imwrite("res.png", trainingData);
	//cout << descriptors.size() << endl;
	//!����SVM֧����������ѵ������
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setC(0.01);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(trainingData, ROW_SAMPLE, trainingLabel);
	svm->save(SVMName);
#else
	//��ȡxml�ļ�
	//Ptr<SVM> svm = SVM::load<SVM>(SVMName);
	CvSVM svm;
	svm.load(SVMName);
#endif
	//!��ȡһ��ͼƬ���в���
	Mat temp;
	temp = src;
	Mat dst;
	int Class;
	resize(temp, dst, size, CV_INTER_CUBIC);

	Mat pre(1, 9, CV_32FC1);
	float *descriptor;
	float *p = pre.ptr<float>(0);
	descriptor = colorMom(temp);
	for (int i = 0; i < 9; i++)
		p[i] = descriptor[i];
	int res = svm->predict(pre);
	//cout << res << endl;
	switch (res)
	{
	case 0:Class = 1; break;
	case 1:Class = 2; break;
	case 2:Class = 3; break;
	}
	return Class;
}