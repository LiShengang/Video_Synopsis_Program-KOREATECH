#include "stdafx.h"
#include "MFC_Synthetic.h"
#include "MFC_SyntheticDlg.h"
#include "afxdialogex.h"
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>
#include <opencv\cv.h>

Mat Syn_Background_Foreground(Mat background, Mat firstForegroundImage, Mat secondForegroundImage, int rows, int cols) {
	Mat result(rows, cols, CV_8UC1);
	// �ռ��� ������ �����
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) { // ���� ������ ��ġ�� �κ�(���氪�� 0�� �ƴ� �κ�)
			if (firstForegroundImage.data[i * cols + j] > 20 || secondForegroundImage.data[i * cols + j] > 20) {	//�����̹������� ���� �κ�
				result.data[i * cols + j] =
					(firstForegroundImage.data[i * cols + j] + secondForegroundImage.data[i * cols + j]) / 2;
			}
			else { // ������ �ƴ� �κ�
				result.data[i * cols + j] = background.data[i * cols + j];
			}
		}
	}
	return result;
}

Mat printObjOnBG(Mat background, segment obj, int* labelMap){
	stringstream ss;
	String fullPath;
	ss << "segment" << "/" << obj.fileName.c_str();
	fullPath = ss.str();
	ss.str("");
	Mat frame = imread(fullPath);

	for (int i = obj.top; i < obj.bottom; i++){
		for (int j = obj.left; j < obj.right; j++){
			if (j == obj.left) {}
			else {
				Vec3b colorB = background.at<Vec3b>(Point(j, i));
				Vec3b colorO = frame.at<Vec3b>(Point(j - obj.left, i - obj.top));

				if (labelMap[i * background.cols + j] == 1) {	//���� ��ü�� �̹� ���� ���
					//Blending
					colorB[0] = (colorB[0] + colorO[0]) / 2;
					colorB[1] = (colorB[1] + colorO[1]) / 2;
					colorB[2] = (colorB[2] + colorO[2]) / 2;
					background.at<Vec3b>(Point(j, i)) = colorB;
				}
				else {
					labelMap[i * background.cols + j] = 1;		//��ü�� �׷ȴٰ� ǥ��
					colorB[0] = colorO[0];
					colorB[1] = colorO[1];
					colorB[2] = colorO[2];
					background.at<Vec3b>(Point(j, i)) = colorB;
				}
			}
		}
	}

	return background;
}