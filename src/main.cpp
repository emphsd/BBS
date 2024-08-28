#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "opencv2\highgui\highgui.hpp" 
#include "opencv2\imgproc\imgproc.hpp" 
#include "../include/bbs.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv){

    // add exceptions to take care of sizing
    //size should be dividable by size patch

    string df="E:/nSm/tempMatchAlgo/BBS/";
    BBS bbs=BBS(3,2,df+"data/images/box.png",df+"data/templates/temp.png");
     bbs.perform();
    std::cout<<"col="<<bbs.getCoords()[0]<<" ,row="<<bbs.getCoords()[1]<<endl;
    return 0;
}