#include "Threshold.h"

int main(int argc,char** argv)
{
    Threshold _threshold;
//    Mat src = imread(argv[1],-1);
    Mat src = imread("../test_img/test.jpg",-1);
    Mat dst;
    dst = _threshold.combine_thresh(src);

    imshow("src",src);
    imshow("dst",dst);
    waitKey(0);
	return 0;
}
