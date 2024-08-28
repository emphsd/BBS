#pragma once
#ifndef BBS_HPP
#define BBS_HPP
#include <opencv2/opencv.hpp>
#include "opencv2\highgui\highgui.hpp" 
#include "opencv2\imgproc\imgproc.hpp" 
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <streambuf> 
#include <fstream> 
#include <sstream>

using namespace std;
using namespace cv;

class BBS{
    private:
        int patchSize;
        float lambda;
        Mat image;
        Mat templateImg;
        vector<float> coords;
        vector<vector<float>> Drgb;
        vector<vector<float>> DrgbPre;
        vector<vector<float>> Dxy;
        vector<float> xx;
        vector<float> yy;
        vector<vector<float>> D;
        vector<vector<float>> Dreversed;
        vector<vector<vector<float>>> Drgb_buffer;
        vector<vector<int>> IndMat;
        vector<vector<float>> Bbs;
        float maxValue=0;
        Mat TMat;
        Mat IMat;
        Mat map;
        float score=900000;
        int rowI=0;
        int colI=0;


    public:
        BBS(int patchSize,float lambda,string pathImg,string pathTemplateImg);
        BBS(int patchSize,float lambda,string pairN);
        void computeBBS();
        void perform();
        void locateMatch();
        void interpolate();
        static Mat img2Col(Mat img,int pathSize);
        Mat getImage(){return image;}
        Mat getTemplateImage(){return templateImg;}
        int getPatchSize(){return patchSize;}
        float getLambda(){return lambda ;}
        float getMaxValue(){return maxValue;}
        vector<float> getCoords(){return coords;}
        int getRow(){return rowI;}
        int getCol(){return colI;}
        float getScore(){return score;}

};


#endif