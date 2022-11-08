#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
using namespace cv;
using namespace std;
//사용자의 얼굴에 맞춰 머리카락 부분에 헤어스타일을 입히는 함수
//3채널 컬러 영상 src(얼굴 이미지)에 PNG파일의 투명한 4채널 영상 overlay(헤어 이미지)를 pos 위치에 합성하여 src에 저장
void overlayImage(Mat& src, const Mat& overlay, const Point& pos)
{
    CV_Assert(src.type() == CV_8UC3); //src는 하나의 픽셀이 unsigned 8bit * 3(R,G,B)로 표현되는 data //얼굴 이미지
    CV_Assert(overlay.type() == CV_8UC4); //overlay는 4채널 data //헤어 이미지
    int srcx = std::max(pos.x, 0); //pos.x와 0을 비교하여 큰 값을 반환 -> srcx
    int srcy = std::max(pos.y, 0); //pos.y와 0을 비교하여 큰 값을 반환 -> srcy
    int overlayx = std::min(pos.x + overlay.cols, src.cols); //pos.x + overlay.cols와 src.cols를 비교하여 작은 값 반환 -> overlayx
    int overlayy = std::min(pos.y + overlay.rows, src.rows); //pos.y + overlay.rows와 src.rows를 비교하여 작은 값 반환 -> overlayy
    //alpha blending an RGBA pixel with an RGB pixel //실제 합성이 이루어지는 영역을 계산
    for (int y = srcy; y < overlayy; y++) {
        int y2 = y - pos.y; //srcy - pos.y
        Vec3b* pSrc = src.ptr<Vec3b>(y); //Vec3b는 Vec<uchar,3>, src.ptr(y)은 특정 행 y좌표에 있는 픽셀의 B,G,R 데이터 값을 반환한다.
        const Vec4b* p0vr = overlay.ptr<Vec4b>(y2); //overlay.ptr(y2)는 특정 행 y2좌표에 있는 픽셀의 B,G,R 데이터 값을 반환한다.
        for (int x = srcx; x < overlayx; x++) {
            int x2 = x - pos.x; //srcx - pos.x
            float alpha = (float)p0vr[x2][3] / 255.f; //투명화의 정도
            if (alpha > 0.f) { //새로운 픽셀 R,G,B 값을 계산한다. //픽셀 결합
                pSrc[x][0] = saturate_cast<uchar>(pSrc[x][0] * (1.f - alpha) //알파블렌딩 공식
                    + p0vr[x2][0] * alpha);
                pSrc[x][1] = saturate_cast<uchar>(pSrc[x][1] * (1.f - alpha)
                    + p0vr[x2][1] * alpha);
                pSrc[x][2] = saturate_cast<uchar>(pSrc[x][2] * (1.f - alpha)
                    + p0vr[x2][2] * alpha); //2차원 배열로 한 장의 입력 영상에 여러 번 합성을 할 수 있는 구조
            }
        }
    }
}

void ShowImages(string title, int nArgs, ...) {
    int size;
    int i;
    int m, n;
    int x, y;
    int w, h;
    float scale;
    int max;

    if (nArgs <= 0) {
        printf("Number of arguments too small....\n");
        return;
    }
    else if (nArgs > 14) {
        printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
        return;
    }

    else if (nArgs == 1) {
        w = h = 1;
        size = 300;
    }
    else if (nArgs == 2) {
        w = 2; h = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4) {
        w = 2; h = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6) {
        w = 3; h = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8) {
        w = 4; h = 2;
        size = 200;
    }
    else {
        w = 4; h = 3;
        size = 150;
    }

    //3채널 이미지를 만들어준다.
    Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC3);
    va_list args;
    va_start(args, nArgs);
    // Loop for nArgs number of arguments
    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
        // Get the Pointer to the IplImage
        Mat img = va_arg(args, Mat);
        // 이미지의 width와 height를 구한다.
        x = img.cols;
        y = img.rows;
        // Find whether height or width is greater in order to resize the image
        max = (x > y) ? x : y;
        // Find the scaling factor to resize the image
        scale = (float)((float)max / size);
        // Used to Align the images
        if (i % w == 0 && m != 20) {
            m = 20;
            n += 20 + size;
        }
        // Set the image ROI to display the current image
        // 입력 이미지 사이즈를 변경하고, Single Big Image에 복사한다.
        Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
        Mat temp; resize(img, temp, Size(ROI.width, ROI.height));
        temp.copyTo(DispImage(ROI));
    }
    //새로운 윈도우를 생성하고, 여러 개의 이미지들이 합쳐진 화면을 띄워준다.
    namedWindow(title, 1);
    imshow(title, DispImage);
    imwrite("D:\\uploads\\final.jpg", DispImage);
    waitKey();
    va_end(args);
}


int main()
{
    //Mat src = imread("D:/uploads/skinbg2_.jpg", IMREAD_COLOR);도 가능하다.
    Mat src = imread("D:\\uploads\\skinbg2_.jpg", IMREAD_COLOR);
    Mat src2 = imread("D:\\uploads\\skinbg2_.jpg", IMREAD_COLOR);
    Mat src3 = imread("D:\\uploads\\skinbg2_.jpg", IMREAD_COLOR);
    Mat src4 = imread("D:\\uploads\\skinbg2_.jpg", IMREAD_COLOR);
    Mat src5 = imread("D:\\uploads\\skinbg2_.jpg", IMREAD_COLOR);
    Mat src6 = imread("D:\\uploads\\skinbg2_.jpg", IMREAD_COLOR);
    Mat h1 = imread("D:\\uploads\\hairstyles\\h2.png", IMREAD_UNCHANGED);
    Mat h2 = imread("D:\\uploads\\hairstyles\\h5.png", IMREAD_UNCHANGED);
    Mat h3 = imread("D:\\uploads\\hairstyles\\h11.png", IMREAD_UNCHANGED);
    Mat h4 = imread("D:\\uploads\\hairstyles\\h4.png", IMREAD_UNCHANGED);
    Mat h5 = imread("D:\\uploads\\hairstyles\\h9.png", IMREAD_UNCHANGED);
    Mat h6 = imread("D:\\uploads\\hairstyles\\h13.png", IMREAD_UNCHANGED);
    CascadeClassifier face_cascade("D:\\uploads\\haarcascade_frontalface_default.xml");
    vector<Rect> faces;
    face_cascade.detectMultiScale(src, faces); //얼굴 검출
    //검출된 얼굴 영역의 가로 크기에 맞게 헤어스타일 이미지의 크기를 축소한 후, 얼굴 위 머리쪽 위치에 헤어스타일을 합성시킴
    for (Rect face : faces) {
        float fx = float(face.width) / h1.cols;
        Mat tmp;
        resize(h1, tmp, Size(), fx * 3, fx * 2.9); //헤어스타일의 크기를 얼굴 사이즈에 맞게 조절
        Point pos(face.x - face.height, face.y - face.height / 3.2); //합성이 되어 놓일 머리의 위치를 파악
        overlayImage(src, tmp, pos);
        resize(h2, tmp, Size(), fx * 3, fx * 2.9);
        overlayImage(src2, tmp, pos);
        resize(h3, tmp, Size(), fx * 3, fx * 2.9);
        overlayImage(src3, tmp, pos);
        resize(h4, tmp, Size(), fx * 3, fx * 2.9);
        overlayImage(src4, tmp, pos);
        resize(h5, tmp, Size(), fx * 3, fx * 2.9);
        overlayImage(src5, tmp, pos);
        resize(h6, tmp, Size(), fx * 3, fx * 2.9);
        overlayImage(src6, tmp, pos);
    }
    ShowImages("Change your hairstyles by PersonalColor", 6, src, src2, src3, src4, src5, src6);
    return 0;
}
