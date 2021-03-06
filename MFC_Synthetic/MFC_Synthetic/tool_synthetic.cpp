#include "stdafx.h"
#include "MFC_Synthetic.h"
#include "MFC_SyntheticDlg.h"
#include "afxdialogex.h"
#include <opencv2\opencv.hpp>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <stdint.h>

Mat Syn_Background_Foreground(Mat background, Mat firstForegroundImage, Mat secondForegroundImage, int rows, int cols) {
	Mat result(rows, cols, CV_8UC1);
	// 합성한 프레임 만들기
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) { // 배경과 전경이 겹치는 부분(전경값이 0이 아닌 부분)
			if (firstForegroundImage.data[i * cols + j] > 20 || secondForegroundImage.data[i * cols + j] > 20) {	//전경이미지에서 전경 부분
				result.data[i * cols + j] =
					(firstForegroundImage.data[i * cols + j] + secondForegroundImage.data[i * cols + j]) / 2;
			}
			else { // 전경이 아닌 부분
				result.data[i * cols + j] = background.data[i * cols + j];
			}
		}
	}
	return result;

}
Mat printObjOnBG(Mat background, segment obj_info, int* labelMap, string loadedFrameFileName) {
	// 세그먼트 jpg파일 불러내 오기
	Mat object = loadJPGObjectFile(obj_info, loadedFrameFileName);
	uint8_t* objPixelPtr = (uint8_t*)object.data;
	int cn = object.channels();

	uint8_t* bgPixelPtr = (uint8_t*)background.data;

	for (int i = obj_info.top; i < obj_info.bottom; i++){
		for (int j = obj_info.left; j < obj_info.right; j++){

			if (labelMap[i * background.cols + j] == 1) {	//이전 객체가 이미 있을 경우
				//Blending
				bgPixelPtr[i*background.cols*cn + j*cn + 0] =
					(bgPixelPtr[i*background.cols*cn + j*cn + 0] +
					objPixelPtr[(i - obj_info.top)*object.cols*cn + (j - obj_info.left)*cn + 0]) / 2;
				bgPixelPtr[i*background.cols*cn + j*cn + 1] =
					(bgPixelPtr[i*background.cols*cn + j*cn + 1] +
					objPixelPtr[(i - obj_info.top)*object.cols*cn + (j - obj_info.left)*cn + 1]) / 2;
				bgPixelPtr[i*background.cols*cn + j*cn + 2] =
					(bgPixelPtr[i*background.cols*cn + j*cn + 2] +
					objPixelPtr[(i - obj_info.top)*object.cols*cn + (j - obj_info.left)*cn + 2]) / 2;
			}
			else {
				labelMap[i * background.cols + j] = 1;		//객체를 그렸다고 표시
				bgPixelPtr[i*background.cols*cn + j*cn + 0] =
					objPixelPtr[(i - obj_info.top)*object.cols*cn + (j - obj_info.left)*cn + 0];
				bgPixelPtr[i*background.cols*cn + j*cn + 1] =
					objPixelPtr[(i - obj_info.top)*object.cols*cn + (j - obj_info.left)*cn + 1];
				bgPixelPtr[i*background.cols*cn + j*cn + 2] =
					objPixelPtr[(i - obj_info.top)*object.cols*cn + (j - obj_info.left)*cn + 2];

			}

		}
	}

	//free(objPixelPtr);
	//free(bgPixelPtr);
	// 블렌딩 프로세스
	//for (int i = obj.top; i < obj.bottom; i++) {
	//	for (int j = obj.left; j < obj.right; j++) {
	//		Vec3b colorB = background.at<Vec3b>(i,j);
	//		Vec3b colorO = frame.at<Vec3b>(i - obj.top,j - obj.left );

	//		if (labelMap[i * background.cols + j] == 1) {	//이전 객체가 이미 있을 경우
	//			colorB[0] = (colorB[0] + colorO[0]) / 2;
	//			colorB[1] = (colorB[1] + colorO[1]) / 2;
	//			colorB[2] = (colorB[2] + colorO[2]) / 2;
	//			background.at<Vec3b>(i,j) = colorB;
	//		} //Blending
	//		else {
	//			labelMap[i * background.cols + j] = 1;		//객체를 그렸다고 표시
	//			colorB[0] = colorO[0];
	//			colorB[1] = colorO[1];
	//			colorB[2] = colorO[2];
	//			background.at<Vec3b>(i, j) = colorB;

	//		}

	//	}
	//}
	return background;
}