// FaceDetection.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "cv.h"
#include "highgui.h"
#include <stdio.h>

static CvHaarClassifierCascade* cascade = 0;
static CvMemStorage* storage = 0;

void detect_and_draw( IplImage* image );
const char* cascade_name ="haarcascade_frontalface_alt.xml";                //�������Ҫ�õ��ķ�����

int main(int argc, char* argv[])
{
	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );   //��������������õķ�����
	if( !cascade )
	{
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return -1;
	}
	storage = cvCreateMemStorage(0);                                       //��̬�洢�ṹ�������洢������ͼ���е�λ��
	cvNamedWindow( "result", 1 );
	const char* filename = "./mouse.jpg";                                     //�����ͼ��
	IplImage* image = cvLoadImage( filename, 1 );                          //����ͼ��
	detect_and_draw( image );                                              //�Լ��ص�ͼ����м��
	cvWaitKey(0);
	cvReleaseImage( &image );
	cvDestroyWindow("result");
	return 0;
}

void detect_and_draw( IplImage* img )
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
	
    double scale = 1.3;
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
		cvRound (img->height/scale)), 8, 1 );
	
    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );
	
    if( cascade )
    {
        /*����cvHaarDetectObjects���ͼ���е�Ŀ�꣬��OpenCV�ṩ��*/
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0 ,
			cvSize(30, 30) );
        for( int i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }
	
    cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}