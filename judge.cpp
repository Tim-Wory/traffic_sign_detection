#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<math.h>
#include"edgesmooth.h"
#include"circlejudge1.h"
#include"trianglejudge.h"
#include"colorSVM.h"
#include"judgeSVM1.h"
#include"judgeSVM2.h"
#include"circlejudge2.h"
using namespace std;
using namespace cv;
//int count = 0;
int main()
{
	int k;
	int classify;
	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	IplImage* img, *img0,*img1;
	Mat srcImagest, srcImageer, imageROI, imageROI1, midImage1, imageroi, srcImage, srcImage33;
	Mat srcImage11, srcImage22;
	Mat srcImage0, src, midImage,srcImage1,srcImage2;
	//����HSV��ͨ��
	Mat imageHchannel;
	Mat imageSchannel;
	Mat imageVchannel;
	VideoCapture cap("C://Users//yuanze//Desktop//9.mp4");
	//VideoCapture cap(0);
	VideoWriter writer;
	writer.open("C://Users//yuanze//Desktop//output.avi", CV_FOURCC_DEFAULT, 30, Size(srcImage.cols, srcImage.rows));
	while (1)
	{
		Mat srcImage1m;
		cap >> srcImage;
		if (!srcImage.data)
		{
			cout << "��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ��ͼƬ���ڣ�" << endl;
			return false;
		}
		//	imshow("��ͨ��־ԭʼͼƬ", srcImage);
		vector<Mat>hsv_vec;
		//��ֵ�˲�
		medianBlur(srcImage, srcImage, 7);
		srcImage.copyTo(srcImage1);
		srcImage.copyTo(imageROI1);
		srcImage.copyTo(srcImage11);
		srcImage.copyTo(srcImage1m);
		srcImage.copyTo(imageroi);
		srcImage.copyTo(srcImage33);
		//srcImage1 = srcImage;
			cvtColor(srcImage, midImage, CV_BGR2HSV);
			cvtColor(srcImage, midImage1, CV_BGR2HSV);
			cvtColor(srcImage, srcImage, CV_BGR2GRAY);
		//ֱ��ͼ���⻯
		equalizeHist(srcImage, srcImage);
		srcImage.copyTo(srcImage0);
		srcImage.copyTo(imageROI);
		//srcImage0 = srcImage;
		//ΪHSV��ͨ������ռ�
		imageHchannel.create(srcImage.size(), srcImage.type());
		imageSchannel.create(srcImage.size(), srcImage.type());
		imageVchannel.create(srcImage.size(), srcImage.type());
		split(midImage, hsv_vec);
		imageHchannel = hsv_vec[0];
		imageSchannel = hsv_vec[1];
		imageVchannel = hsv_vec[2];
		merge(hsv_vec, midImage);
		//imshow("HSVͨ��ͼƬ", midImage);
		//��ͨ��־��ֵ��
		for (int i = 0; i < srcImage.rows; i++)
		{
			const uchar*data_h = imageHchannel.ptr<uchar>(i);
			const uchar*data_s = imageSchannel.ptr<uchar>(i);
			const uchar*data_v = imageVchannel.ptr<uchar>(i);
			for (int j = 0; j < srcImage.cols*srcImage.channels(); j++)
			{
				if ((data_h[j] >= 100 && data_h[j] <= 124) && (data_s[j] >= 43 && data_s[j] <= 255) && (data_v[j] >= 46 && data_v[j] <= 255) || (((data_h[j] >= 0 && data_h[j] <= 10) || (data_h[j] >= 156 && data_h[j] <= 180)) && (data_s[j] >= 43 && data_s[j] <= 255) && (data_v[j] >= 46 && data_v[j] <= 255)) || ((data_h[j] >= 11 && data_h[j] <= 34) && (data_s[j] >= 43 && data_s[j] <= 255) && (data_v[j] >= 46 && data_v[j] <= 255)) || ((data_h[j] >= 0 && data_h[j] <= 180) && (data_s[j] >= 0 && data_s[j] <= 255) && (data_v[j] >= 0 && data_v[j] <= 46)))
					srcImage.at<uchar>(i, j) = 255;
				else
					srcImage.at<uchar>(i, j) = 0;
			}
		}
		//imshow("��ֵ����ͼ��", srcImage);
		srcImage.copyTo(srcImage2);
		//srcImage2 = srcImage;
		//��Եƽ��
		delete_jut(srcImage, srcImagest, 10, 10, 1);
		//imshow("�߽�ƽ��", srcImagest);
		Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
		erode(srcImage, srcImageer, element);
		//imshow("��ʴ���ͼ��", srcImageer);
		//�ҵ�Բ�ν�ͨ��־���ָ�
		k = findCircles(srcImageer, midImage,srcImage11, srcImage1,srcImage1m, imageROI, 50, 200, 3, 10000);
		imageROI.copyTo(imageroi);
		classify = colorsvm(srcImage1m);
		if (k == 0 || classify == 3)
		{
			//classify = 0;
			img = &IplImage(srcImage0);
			img0 = &IplImage(midImage1);
			img1 = &IplImage(srcImage11);
			split(midImage1, hsv_vec);
			imageHchannel = hsv_vec[0];
			imageSchannel = hsv_vec[1];
			imageVchannel = hsv_vec[2];
			merge(hsv_vec, midImage1);
			for (int i = 0; i < srcImage0.rows; i++)
			{
				const uchar*data_h = imageHchannel.ptr<uchar>(i);
				const uchar*data_s = imageSchannel.ptr<uchar>(i);
				const uchar*data_v = imageVchannel.ptr<uchar>(i);
				for (int j = 0; j < srcImage0.cols*srcImage0.channels(); j++)
				{
					if ((data_h[j] >= 100 && data_h[j] <= 124) && (data_s[j] >= 43 && data_s[j] <= 255) && (data_v[j] >= 46 && data_v[j] <= 255) || (((data_h[j] >= 0 && data_h[j] <= 10) || (data_h[j] >= 156 && data_h[j] <= 180)) && (data_s[j] >= 43 && data_s[j] <= 255) && (data_v[j] >= 46 && data_v[j] <= 255)) || ((data_h[j] >= 11 && data_h[j] <= 34) && (data_s[j] >= 43 && data_s[j] <= 255) && (data_v[j] >= 46 && data_v[j] <= 255)))
						srcImage0.at<uchar>(i, j) = 255;
					else
						srcImage0.at<uchar>(i, j) = 0;
				}
			}
			//imshow("��ֵ����ͼ��", srcImage0);
			//��Եƽ��
			//delete_jut(srcImage0, srcImage0, 10, 10, 1);
			//imshow("�߽�ƽ��", srcImage0);
			Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
			//erode(srcImage0, srcImage0, element);
			//	imshow("��ʴ���ͼ��", srcImage0);
			drawSquares(img0, img,img1, findSquares4(img, img0, storage, 5000, 0, 180), "�����μ��");
			srcImage22 = cvarrToMat(img1);
			imshow("myVideoPlayer", srcImage22);
			writer << srcImage22;
		}
		else
		{
			//classify = colorsvm(srcImage1);
			switch (classify)
			{
			case 1://imshow("HSVͨ��ͼƬ", midImage);
				//imshow("��ֵ����ͼ��", srcImage);
				//	imshow("�߽�ƽ��", srcImagest);
				//	imshow("��ʴ���ͼ��", srcImageer);
				//	imshow("�ָ��ͼƬ", midImage);
				imwrite("C://Users//yuanze//Desktop//40test3.jpg", imageroi);
				judgesvm1(imageroi, classify); 
				imshow("myVideoPlayer", srcImage11);
				writer << srcImage11; break;
			case 2:
				//	imshow("��ֵ����ͼ��", srcImage);
				//	imshow("HSVͨ��ͼƬ", midImage1);
				//delete_jut(srcImage2, srcImage2, 10, 10, 1);
				Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
				dilate(srcImage2, srcImage2, element);
				findCircles1(srcImage2, midImage1,srcImage33, imageROI1, 50, 200, 3, 10000);
				imwrite("C://Users//yuanze//Desktop//40test4.jpg", imageROI1);
				judgesvm2(imageROI1, classify);
				imshow("myVideoPlayer", srcImage33);
				writer << srcImage33; break;
				//	imshow("�ָ��ͼƬ", midImage1); break;
			}
		}


		waitKey(10);
	}
	return 0;
}