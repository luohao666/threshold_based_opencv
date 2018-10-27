#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

Mat img_clone,img_cp;
int pt_x,pt_y;


void cvMouseCallback(int mouseEvent,int x,int y,int flags,void* param)
{
  pt_x=x;
  pt_y=y;
  return;
}

double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void findSquares(Mat src, vector<vector<Point> > &squares)
{
    Mat pyr, timg, gray0(src.size(), CV_8U), Src_HSV(src.size(), CV_8U), gray;

    pyrDown(src, pyr, Size(src.cols / 2, src.rows / 2));
    pyrUp(pyr, timg, src.size());
    vector<vector<Point> > contours;

    int counts = 0, counts_2 = 0, counts_3 = 0;
    vector<Point> cand_1, cand_2, cand_3;
    vector<vector<Point> > rect_2;
    vector<vector<Point> > rect_3;
    vector<vector<Point> > out;

    //cout<<"find squares src.chan="<<src.channels()<<endl;
    cvtColor(src, gray0, COLOR_BGR2GRAY);

    Canny(gray0, gray, 50, 200, 5);

    imshow("Canny", gray);

    //dilate(gray, gray, Mat(), Point(-1,-1));
    findContours(gray, contours, RETR_LIST, CHAIN_APPROX_NONE);

    vector<Point> approx;

    for (size_t i = 0; i < contours.size(); i++)
    {
        if (contours[i].size() < 200 || contours[i].size() > 1000)
        {
            continue;
        }
        //cout<<"rectangle detected 1111"<<endl;

        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.01, true);
        cout<<contours[i].size() <<"  "<<approx.size()<<" "<<fabs(contourArea(Mat(approx)))<<endl;
       

        if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 2000 && isContourConvex(Mat(approx)))
        {
            //cout<<contours[i].size() <<"  "<<approx.size()<<" "<< fabs(contourArea(Mat(approx))) << " " <<  isContourConvex(Mat(approx)) <<endl;
            //cout<<"rectangle detected  2222"<<endl;
            double maxCosine = 0;
            for (int j = 2; j < 5; j++)
            {
                double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }
            if (maxCosine < 0.3)
            {
                //cout<<"squares detected!"<<endl;
                squares.push_back(approx);
               
            }
        }
    }
}

void drawSquares( Mat image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();

        polylines(image, &p, &n, 1, true, Scalar(0,255,0), 1, LINE_AA);
    }
    imshow("Square Detection Demo", image);
}
int main(int argc,char* argv[])
{
	Mat img=imread(argv[1]);
	Mat img_hsv;
	cvtColor(img,img_hsv,COLOR_BGR2HSV);
	namedWindow("img");
	setMouseCallback("img",cvMouseCallback);
	unsigned char *p=img.data;
	unsigned char *phsv=img_hsv.data;
	int r,g,b;
	int h,s,v;
    
    vector<vector<Point> > squares;
	findSquares(img,squares);
	cout<<squares.size()<<endl;
    /*
                int center_x=0,center_y=0;
                for(int n=0;n<4;n++)
                {
                    center_x+=approx[n].x;
                    center_y+=approx[n].y;
                }
                center_x=center_x/4;
                center_y=center_y/4;
    */
    img.copyTo(img_cp);
    drawSquares(img_cp,squares);

	while(1)
	{
		img.copyTo(img_clone);
        
		
		b=(int)p[(img.cols*pt_y+pt_x)*3];
		g=(int)p[(img.cols*pt_y+pt_x)*3+1];
		r=(int)p[(img.cols*pt_y+pt_x)*3+2];
		
		h=(int)phsv[(img.cols*pt_y+pt_x)*3];
		s=(int)phsv[(img.cols*pt_y+pt_x)*3+1];
		v=(int)phsv[(img.cols*pt_y+pt_x)*3+2];
		
		char str1[100],str2[100],str3[100],str4[100];
		sprintf(str1,"X:%d  Y:%d",pt_x,pt_y);
		sprintf(str2,"R:%d  G:%d  B:%d",r,g,b);
		sprintf(str3,"H:%d  S:%d  V:%d",h,s,v);
        sprintf(str4,"r/b:%f  g/b:%f, r-g:%f",r*1.0/b,g*1.0/b,(r-g)*1.0);
        //sprintf(str4,"r/b:%f  g/b:%f, r-g:%f",r*1.0/b,g*1.0/b,(r+g)*1.0/b);
        putText(img_clone,str1,Point(50,50),FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,255),1);
        putText(img_clone,str2,Point(50,90),FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,255),1);
        putText(img_clone,str3,Point(50,130),FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,255),1);
        putText(img_clone,str4,Point(50,170),FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,255),1);
		imshow("img",img_clone);
		waitKey(100);
	}
	
}
