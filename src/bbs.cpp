#pragma once 
#include <iostream> 
#include <opencv2/opencv.hpp>
#include "opencv2\highgui\highgui.hpp" 
#include "opencv2\imgproc\imgproc.hpp" 
#include "..\include\bbs.hpp"


BBS::BBS(int patchSize,float lambda,string pathImg,string pathTemplateImg){
    this->patchSize=patchSize;
    this->lambda=lambda;

    std::cout<<"hi"<<std::endl;
    // READING IMAGES
    templateImg=imread(pathTemplateImg);
    image=imread(pathImg);

    
}

BBS::BBS(int patchSize,float lambda,string pairN){
    this->patchSize=patchSize;
    this->lambda=lambda;

    // READING IMAGES


    // templateImg=imread(pathTemplateImg);
    // image=imread(pathImg);
}




void BBS::locateMatch(){

    maxValue=0;
    coords.resize(2);
    for(int i=0;i<(image.cols-templateImg.cols)/patchSize+1;i++){
        for(int j=0;j<(image.rows-templateImg.rows)/patchSize+1;j++){
            // std::cout<<Bbs[i][j]<<"row"<<i*patchSize<<"col"<<j*patchSize<<std::endl;
            if (Bbs[i][j]>=maxValue){
                maxValue=Bbs[i][j];
                coords[0]=j*patchSize;
                coords[1]=i*patchSize;

            }
        }
    }
}


void BBS::perform(){
    std::cout<<"hi2"<<std::endl;
    // std::cout<<image.depth()<<templateImg.type()<<endl;
    image.convertTo(image,CV_32FC3, 1.0/255.0);
    templateImg.convertTo(templateImg,CV_32FC3, 1.0/255.0);

    // cvtColor(image,image,COLOR_RGB2HSV);
    // cvtColor(templateImg,templateImg,COLOR_RGB2HSV);

    // //gonna add driver supp
    TMat=img2Col(templateImg,patchSize);
    IMat=img2Col(image,patchSize);

    computeBBS();

    locateMatch();

    

    

}


void BBS::interpolate(){


}


void BBS::computeBBS(){
    //Gaussian lowpass filter
    float Gaussian[]{ 0.0277, 0.1110, 0.0277, 0.1110, 0.4452, 0.1110, 0.0277, 0.1110, 0.0277 };
    // float Gaussian[]{ 1,1,1,1,1,1,1,1,1 };

    // Mat gauss=getGaussianKernel(patchSize*patchSize,0.6, CV_32FC1);
    // std::cout<<"computing bbs"<<(float)gauss.data[0]<<std::endl;

    // auto Gaussian=gauss.data;

    // for(int i=0;i<patchSize*patchSize;i++){
    //     Gaussian[i]=(float)Gaussian[i];
    //     Gaussian[i]=Gaussian[i]/100;
    //     cout<<Gaussian[i]<<endl;
    // }

    std::cout<<(image.cols-templateImg.cols)/patchSize+1<<std::endl;
    std::cout<<(image.rows-templateImg.rows)/patchSize+1<<std::endl;
    int N=TMat.cols;

    Bbs.resize((image.cols-templateImg.cols)/patchSize+1);
    for(int i=0;i<(image.cols-templateImg.cols)/patchSize+1;i++){
        Bbs.at(i).resize((image.rows-templateImg.rows)/patchSize+1);
    }


    Drgb.resize(N);
    //?
    Drgb_buffer.resize((image.rows-templateImg.rows)/patchSize+1);
    for(int i=0;i<N;i++){
        Drgb.at(i).resize(N);
    }

    for (int i=0;i<(image.rows-templateImg.rows)/patchSize+1;i++){
    Drgb_buffer.at(i).resize(N);
    for (int j=0;j<N;j++){
        Drgb_buffer.at(i).at(j).resize(N);
    }}
    
    IndMat.resize(image.cols/patchSize);

    for (int i=0;i<image.cols/patchSize;i++){
        IndMat.at(i).resize(image.rows/patchSize);
        for(int j=0;j<image.rows/patchSize;j++){
            IndMat[i][j]=i+j*image.cols/patchSize;
        }
    }

    Dxy.resize(N);
    xx.resize(N);
    yy.resize(N);

    float x=0,y=0;

  
  int tt=0;
for(int j=0;j<templateImg.rows/patchSize;j++){
        x=(float)j*patchSize*0.0039;
        for(int i=0;i<templateImg.cols/patchSize;i++){
            tt++;
            y=(float)i*patchSize*0.0039;
            // cout<<"h"<<i<<" , "<<j<<endl;
            xx[i+j*templateImg.cols/patchSize]=x;
            yy[i+j*templateImg.cols/patchSize]=y;
           
        }
    }
     

  std::cout<<tt<<" he "<<N<<" re "<<(image.rows-templateImg.rows)/patchSize+1<<std::endl;
    for(int i=0;i<N;i++){
        Dxy.at(i).resize(N);
        for(int j=0;j<N;j++){
            Dxy[i][j]=(float)(pow(xx[i]-xx[j],2)+pow(yy[i]-yy[j],2));
        }
    }

    std::cout<<"here2"<<std::endl;

    // vector<int>w;

    // vector<int>indices;

    float r=0,g=0,b=0; 
    for(int col=0;col<(image.cols-templateImg.cols)/patchSize+1;col++){
        for(int row=0;row<(image.rows-templateImg.rows)/patchSize+1;row++){
            
            // std::cout<<"computing bbs"<<row<<"|"<<col<<(image.rows-templateImg.rows)/patchSize+1<<"|"<<(image.cols-templateImg.cols)/patchSize+1<<std::endl;
           
            if (row==0 && col==0){
                for (int j=0;j<templateImg.cols/patchSize;j++){
                    for(int i=0;i<templateImg.rows/patchSize;i++){
                        // indices.push_back(IndMat[j][i]);
                        // w.push_back(l);
                        // l++;
                        for(int z=0;z<N;z++){
                        r=0;g=0;b=0;
                        for(int k=0;k<patchSize*patchSize;k++){
                            r+=pow((TMat.at<Vec3f>(k,z)[0]-IMat.at<Vec3f>(k,IndMat[j][i])[0])*Gaussian[k],2);
                            g+=pow((TMat.at<Vec3f>(k,z)[1]-IMat.at<Vec3f>(k,IndMat[j][i])[1])*Gaussian[k],2);
                            b+=pow((TMat.at<Vec3f>(k,z)[2]-IMat.at<Vec3f>(k,IndMat[j][i])[2])*Gaussian[k],2);
                        }
                        Drgb[i*templateImg.cols/patchSize+j][z]=(r+g+b);
 
                        
                    }
                    
                    }
                }



            }

            else if (row!=0 && col==0){

                DrgbPre=Drgb;
                // for (int i=row;i<row+templateImg.rows/patchSize;i++)
                r=0;g=0;b=0;

                // templateImg.cols/patchSize
                for(int i=templateImg.cols/patchSize;i<N;i++){
                    Drgb[i-templateImg.cols/patchSize]=DrgbPre[i];
                }


                
                 for(int j=0;j<templateImg.cols/patchSize;j++)
                {
                    // indices.push_back(IndMat[j][row]);
                    for (int i=0;i<N;i++){
                        r=0;g=0;b=0;
                        for(int k=0;k<patchSize*patchSize;k++){
                            r+=pow((TMat.at<Vec3f>(k,i)[0]-IMat.at<Vec3f>(k,IndMat[j][row+templateImg.rows/patchSize-1])[0])*Gaussian[k],2);
                            g+=pow((TMat.at<Vec3f>(k,i)[1]-IMat.at<Vec3f>(k,IndMat[j][row+templateImg.rows/patchSize-1])[1])*Gaussian[k],2);
                            b+=pow((TMat.at<Vec3f>(k,i)[2]-IMat.at<Vec3f>(k,IndMat[j][row+templateImg.rows/patchSize-1])[2])*Gaussian[k],2);
                        }
                        // Drgb[(row%(templateImg.rows/patchSize))*templateImg.cols/patchSize+j][i]=r+g+b;
                       
                        Drgb[(templateImg.rows/patchSize-1)*templateImg.cols/patchSize+j][i]=r+g+b;



                    }
                }



                


     
            }

            else if (row==0 && col!=0){
                
                DrgbPre=Drgb_buffer[0];
               r=0;g=0;b=0;

                
                for(int i=0;i<templateImg.rows/patchSize;i++){
                     for(int j=1;j<templateImg.cols/patchSize;j++){
                        // std::cout<<"computing bbs2"<<i*templateImg.cols/patchSize+j-1<<"|"<<N<<std::endl;
                        Drgb[i*templateImg.cols/patchSize+j-1]=DrgbPre[i*templateImg.cols/patchSize+j];
                    }   
                }

            
            

                for(int j=0;j<templateImg.rows/patchSize;j++){

                     for (int i=0;i<N;i++){
                        // std::cout<<"co "<<j*templateImg.cols/patchSize+templateImg.cols/patchSize-1<<"|"<<N<<std::endl;
                        r=0;g=0;b=0;
                        for(int k=0;k<patchSize*patchSize;k++){
                            r+=pow((TMat.at<Vec3f>(k,i)[0]-IMat.at<Vec3f>(k,IndMat[col+templateImg.cols/patchSize-1][j])[0])*Gaussian[k],2);
                            g+=pow((TMat.at<Vec3f>(k,i)[1]-IMat.at<Vec3f>(k,IndMat[col+templateImg.cols/patchSize-1][j])[1])*Gaussian[k],2);
                            b+=pow((TMat.at<Vec3f>(k,i)[2]-IMat.at<Vec3f>(k,IndMat[col+templateImg.cols/patchSize-1][j])[2])*Gaussian[k],2);
                        }
                        Drgb[j*templateImg.cols/patchSize+templateImg.cols/patchSize-1][i]=r+g+b;
                        // Drgb[j*templateImg.cols/patchSize+col%(templateImg.cols/patchSize)][i]=r+g+b;
                    }

                }



            }


            else {

                DrgbPre=Drgb;

                for(int i=templateImg.cols/patchSize;i<N;i++){
                    Drgb[i-templateImg.cols/patchSize]=DrgbPre[i];
                }

                //       for(int i=1;i<templateImg.rows/patchSize;i++){
                //     for(int j=0;j<templateImg.cols/patchSize;j++)
                //     {Drgb[(i-1)*templateImg.cols/patchSize+j]=DrgbPre[(i)*templateImg.cols/patchSize+j];}
                // }

                DrgbPre=Drgb_buffer[row];

                for(int j=1;j<templateImg.cols/patchSize;j++){
                    for(int i=0;i<templateImg.rows/patchSize;i++){
                        Drgb[i*templateImg.cols/patchSize+j-1]=DrgbPre[i*templateImg.cols/patchSize+j];
                    }   
                }

            //    new pixel
            for (int i=0;i<N;i++){
                r=0;g=0;b=0;
                // if(row==100 && col==100)std::cout<<"h nl"<<endl;
                  for(int k=0;k<patchSize*patchSize;k++){
                
                            r+=pow((TMat.at<Vec3f>(k,i)[0]-IMat.at<Vec3f>(k,IndMat[col+templateImg.cols/patchSize-1][row+templateImg.rows/patchSize-1])[0])*Gaussian[k],2);
                            g+=pow((TMat.at<Vec3f>(k,i)[1]-IMat.at<Vec3f>(k,IndMat[col+templateImg.cols/patchSize-1][row+templateImg.rows/patchSize-1])[1])*Gaussian[k],2);
                            b+=pow((TMat.at<Vec3f>(k,i)[2]-IMat.at<Vec3f>(k,IndMat[col+templateImg.cols/patchSize-1][row+templateImg.rows/patchSize-1])[2])*Gaussian[k],2);


                      
            }
                     Drgb[N-1][i]=r+g+b;
      


            }}

            Drgb_buffer[row]=Drgb;
            // D=Drgb+Dxy;
            

            
            D=Drgb;
            Dreversed=D;

            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    D[i][j]=Drgb[i][j]+lambda*Dxy[i][j];
                    
                    // if (row==100 && col==100 && Dxy[i][j]==0)
                    // cout<<D[i][j]<<" o "<<Dxy[i][j]<<" o "<<Drgb[i][j]<<endl;
                    if (D[i][j]<1e-6) D[i][j]=0;
                    Dreversed[j][i]=D[i][j];
                }
            }



            vector<float>min1,min2;
            vector<int>min3,min4;
            min1.resize(N);
            min2.resize(N);

            min3.resize(N);
            min4.resize(N);
      

            int count=0;

            for(int i=0;i<N;i++){
                min1[i]=*min_element(D[i].begin(),D[i].end());
                min2[i]=*min_element(Dreversed[i].begin(),Dreversed[i].end());
                // std::cout<<min1[i]<<endl;


                }

            for(int i=0;i<N;i++){
                for (int j=0;j<N;j++){
                    if (D[i][j]==min1[i] && min2[j]==D[i][j])
                        {
                        count++;}
                }}
            

                float sum=0;


              for(int i=0;i<N;i++){
                  sum+=D[i][i];
                    }

               

    // std::cout<<"finished bbs"<<std::endl;
                
                if(sum<score) {
                score=sum;
                rowI=row*patchSize;
                colI=col*patchSize;
                }
                

            

           

            min1.clear();
            min2.clear();



            Bbs[col][row]=(float)count/N;


            

            
            

            

        }
       
    }


}


//patch Size |
Mat BBS::img2Col(Mat img,int patchSize){

    Mat CImg(patchSize*patchSize,img.rows/patchSize*img.cols/patchSize,CV_32FC3,Scalar(0.0, 0.0, 0.0));

    int col=0;

    std::cout<<"img"<<img.cols<<" r "<<img.rows<<endl;

    // // 0 3 6 9
    for(int j=0;j<img.rows/patchSize;j++){
       for (int i=0;i<img.cols/patchSize;i++){
            for (int k1=0;k1<patchSize;k1++){
                for (int k2=0;k2<patchSize;k2++){
                    // std::cout<<j+k1<<"|"<<i+k2<<"|"<<img.cols<<"|"<<img.rows<<std::endl;
                    // std::cout<<img.rows/patchSize<<"|"<<img.cols/patchSize<<std::endl;

                    // std::cout<<(float)img.at<Vec3f>(j*patchSize+k2,i*patchSize+k1)[0]<<std::endl;
                    // std::cout<<(float)img.at<Vec3f>(j*patchSize+k2,i*patchSize+k1)[1]<<std::endl;
                    // std::cout<<(float)img.at<Vec3f>(j*patchSize+k2,i*patchSize+k1)[2]<<std::endl;
                    

                CImg.at<Vec3f>(k1*patchSize+k2,col)[0]=(float)img.at<Vec3f>(j*patchSize+k1,i*patchSize+k2)[0];
                CImg.at<Vec3f>(k1*patchSize+k2,col)[1]=(float)img.at<Vec3f>(j*patchSize+k1,i*patchSize+k2)[1];
                CImg.at<Vec3f>(k1*patchSize+k2,col)[2]=(float)img.at<Vec3f>(j*patchSize+k1,i*patchSize+k2)[2];

                //  std::cout<<CImg.at<Vec3f>(k1*patchSize+k2,col)[0]<<std::endl;
                //     std::cout<<CImg.at<Vec3f>(k1*patchSize+k2,col)[1]<<std::endl;
                //     std::cout<<CImg.at<Vec3f>(k1*patchSize+k2,col)[2]<<std::endl;

                }
                
            }
            col++;

            // cout<<col<<'|'<<j/patchSize+i/patchSize*img.rows/patchSize<<endl;
            
            
        }
    }
    std::cout<<"img "<<col<<" r "<<img.rows/patchSize*img.cols/patchSize<<endl;
    
    return CImg;
}

