#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
using namespace cv;
using namespace std;
//������� �󱼿� ���� �Ӹ�ī�� �κп� ��Ÿ���� ������ �Լ�
//3ä�� �÷� ���� src(�� �̹���)�� PNG������ ������ 4ä�� ���� overlay(��� �̹���)�� pos ��ġ�� �ռ��Ͽ� src�� ����
void overlayImage(Mat& src, const Mat& overlay, const Point& pos)
{
    CV_Assert(src.type() == CV_8UC3); //src�� �ϳ��� �ȼ��� unsigned 8bit * 3(R,G,B)�� ǥ���Ǵ� data //�� �̹���
    CV_Assert(overlay.type() == CV_8UC4); //overlay�� 4ä�� data //��� �̹���
    int srcx = std::max(pos.x, 0); //pos.x�� 0�� ���Ͽ� ū ���� ��ȯ -> srcx
    int srcy = std::max(pos.y, 0); //pos.y�� 0�� ���Ͽ� ū ���� ��ȯ -> srcy
    int overlayx = std::min(pos.x + overlay.cols, src.cols); //pos.x + overlay.cols�� src.cols�� ���Ͽ� ���� �� ��ȯ -> overlayx
    int overlayy = std::min(pos.y + overlay.rows, src.rows); //pos.y + overlay.rows�� src.rows�� ���Ͽ� ���� �� ��ȯ -> overlayy
    //alpha blending an RGBA pixel with an RGB pixel //���� �ռ��� �̷������ ������ ���
    for (int y = srcy; y < overlayy; y++) {
        int y2 = y - pos.y; //srcy - pos.y
        Vec3b* pSrc = src.ptr<Vec3b>(y); //Vec3b�� Vec<uchar,3>, src.ptr(y)�� Ư�� �� y��ǥ�� �ִ� �ȼ��� B,G,R ������ ���� ��ȯ�Ѵ�.
        const Vec4b* p0vr = overlay.ptr<Vec4b>(y2); //overlay.ptr(y2)�� Ư�� �� y2��ǥ�� �ִ� �ȼ��� B,G,R ������ ���� ��ȯ�Ѵ�.
        for (int x = srcx; x < overlayx; x++) {
            int x2 = x - pos.x; //srcx - pos.x
            float alpha = (float)p0vr[x2][3] / 255.f; //����ȭ�� ����
            if (alpha > 0.f) { //���ο� �ȼ� R,G,B ���� ����Ѵ�. //�ȼ� ����
                pSrc[x][0] = saturate_cast<uchar>(pSrc[x][0] * (1.f - alpha) //���ĺ��� ����
                    + p0vr[x2][0] * alpha);
                pSrc[x][1] = saturate_cast<uchar>(pSrc[x][1] * (1.f - alpha)
                    + p0vr[x2][1] * alpha);
                pSrc[x][2] = saturate_cast<uchar>(pSrc[x][2] * (1.f - alpha)
                    + p0vr[x2][2] * alpha); //2���� �迭�� �� ���� �Է� ���� ���� �� �ռ��� �� �� �ִ� ����
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

    //3ä�� �̹����� ������ش�.
    Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC3);
    va_list args;
    va_start(args, nArgs);
    // Loop for nArgs number of arguments
    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
        // Get the Pointer to the IplImage
        Mat img = va_arg(args, Mat);
        // �̹����� width�� height�� ���Ѵ�.
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
        // �Է� �̹��� ����� �����ϰ�, Single Big Image�� �����Ѵ�.
        Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
        Mat temp; resize(img, temp, Size(ROI.width, ROI.height));
        temp.copyTo(DispImage(ROI));
    }
    //���ο� �����츦 �����ϰ�, ���� ���� �̹������� ������ ȭ���� ����ش�.
    namedWindow(title, 1);
    imshow(title, DispImage);
    imwrite("D:\\uploads\\final.jpg", DispImage);
    waitKey();
    va_end(args);
}


int main()
{
    //Mat src = imread("D:/uploads/skinbg2_.jpg", IMREAD_COLOR);�� �����ϴ�.
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
    face_cascade.detectMultiScale(src, faces); //�� ����
    //����� �� ������ ���� ũ�⿡ �°� ��Ÿ�� �̹����� ũ�⸦ ����� ��, �� �� �Ӹ��� ��ġ�� ��Ÿ���� �ռ���Ŵ
    for (Rect face : faces) {
        float fx = float(face.width) / h1.cols;
        Mat tmp;
        resize(h1, tmp, Size(), fx * 3, fx * 2.9); //��Ÿ���� ũ�⸦ �� ����� �°� ����
        Point pos(face.x - face.height, face.y - face.height / 3.2); //�ռ��� �Ǿ� ���� �Ӹ��� ��ġ�� �ľ�
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
