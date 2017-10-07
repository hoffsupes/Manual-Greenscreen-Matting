#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat apply_alpha(Mat I, Mat alpha)
{
   
    Mat al;
    Mat newal[] = {alpha,alpha,alpha};
    merge(newal,3,al);
    
    Mat R = I.mul(al);
    
 return R;   
}

float al1,al2,al3;

Mat get_composite(string nam, string nam2, string nam3,string n)            // extra function created for easy compositing, when manually needed to do so, requires only    
                                                                            // location of the files
{
    Mat R;
    
    Mat alpha = imread(nam.data(),0);
    alpha.convertTo(alpha,CV_32F);
    normalize(alpha,alpha,0,1,NORM_MINMAX);
    
    
    Mat I = imread(nam3.data());
    I.convertTo(I,CV_32F);
    normalize(I,I,0,1,NORM_MINMAX);
    
    
    Mat T = imread(nam2.data());
    T.convertTo(T,CV_32F);
    normalize(T,T,0,1,NORM_MINMAX);
    
    resize(T,T,I.size());
    
    R = apply_alpha(I,alpha) + apply_alpha(T,1-alpha);
    normalize(R,R,0,255,NORM_MINMAX);
    R.convertTo(R,CV_8U);
    
    resize(R,R,R.size()/3);
    
    imwrite(n.data(),R);
    
    return R;
}

string get_file_name(string img, string spc,float a1, float a2,float a3)
{
        
    
    string a_1 = to_string(a1);
    string a_2 = to_string(a2);
    string a_3 = to_string(a3);
    
    string nam(img.data());
    nam.append(spc.data());
    nam.append("_");
    nam.append(a_1);
    nam.append("_");
    nam.append(a_2);
    nam.append("_");
    nam.append(a_3);
    nam.append(".jpeg");

    return nam;
    
}

string get_file_name_vid(string img, string spc,int idx)
{
        
    string idd = to_string(idx);
    
    string nam(img.data());
    nam.append(spc.data());
    nam.append("_");
    nam.append(idd);
    nam.append("_.jpeg");

    return nam;
    
}


void frame_pross(Mat fram,Mat T,int idx)
{
     
    Mat Ib,Ig,Ir;
    vector<Mat> v;
    split(fram,v);
    
    Ib = v[0];                                          // OpenCV follows the BGR scheme instead of a RGB scheme
    Ig = v[1];
    Ir = v[2];
   
    Mat alpha = 1 - (5.962)*(Ig - (0.879)*(Ib));                // The matting equation
    normalize(alpha,alpha,0,1,NORM_MINMAX);             // normalize between 0 and 1

    Mat mal,m2,m3;                                      //
    m2 = Mat::zeros(alpha.size(),CV_32F);               //
    alpha.copyTo(mal);                                  //
    mal.convertTo(mal,CV_8U);                           //
    threshold(mal,mal,255,1,THRESH_OTSU);               // determine mask to threshold only the FG region in alpha
    
    alpha.copyTo(m2,mal); 
    
    dilate(m2,m2,Mat::ones(200,200,CV_8U),Point(-1,-1),5);
    erode(m2,m2,Mat::ones(200,200,CV_8U),Point(-1,-1),5);
    
    pow(m2,(0.261),m2);                 
    m2.copyTo(alpha,mal);                               // gamma correct only portion specified by mask
    
    Mat aly;
    resize(alpha,aly,alpha.size()/3);

    Mat Y =  apply_alpha(T, 1-alpha) + apply_alpha(fram, alpha);       // apply alpha, multiply alpha to each channel of the given image
    
    normalize(Y,Y,0,255,NORM_MINMAX);                               // normalize resultant to 0 to 255 and save as an 8 bit image
    Y.convertTo(Y,CV_8U); 
    Mat yul;
    resize(Y,yul,fram.size()/3);
       
    string alnam = get_file_name_vid("/home/hulio/RPI/CVFX/HW_2/rain_frames_alpha/alpha","Kung_Fu",idx);
    string comnam = get_file_name_vid("/home/hulio/RPI/CVFX/HW_2/rain_composite/composite","Kung_Fu",idx);
    string orinam = get_file_name_vid("/home/hulio/RPI/CVFX/HW_2/rain_original/original","Kung_Fu",idx);
    
    normalize(alpha,alpha,0,255,NORM_MINMAX);
    alpha.convertTo(alpha,CV_8U); 
    imwrite(comnam.data(),Y);                                       // write files
    imwrite(alnam.data(),alpha);
    imwrite(orinam.data(),fram);
    
    
}

vector<int> get_fram_list2()
{
    vector<int> flish;
    int i = 0;
    for(i = 0; i < 50; i++){ flish.push_back(i); }
    for(i = 49; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 36; i--){ flish.push_back(i); }
    for(i =37; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 37; i--){ flish.push_back(i); }
    for(i =38; i <= 50; i++){ flish.push_back(i); }
    for(i = 49; i >= 44; i--){ flish.push_back(i); }
    for(i =45; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }    
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 28; i--){ flish.push_back(i); }        
    return flish;
}


vector<int> get_fram_list()
{
    vector<int> flish;
    int i = 0;
    for(i = 0; i < 50; i++){ flish.push_back(i); }
    for(i = 49; i >= 28; i--){ flish.push_back(i); }
    for(i = 29; i <= 46; i++){ flish.push_back(i); }
    for(i = 45; i >= 36; i--){ flish.push_back(i); }
    for(i =37; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 37; i--){ flish.push_back(i); }
    for(i =38; i <= 50; i++){ flish.push_back(i); }
    for(i = 49; i >= 44; i--){ flish.push_back(i); }
    for(i =45; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }
    for(i = 46; i >= 40; i--){ flish.push_back(i); }
    for(i =41; i <= 47; i++){ flish.push_back(i); }

    return flish;
}

void get_I_alpha(Mat&I, Mat&alpha,int i)
{
    
            string al_n = get_file_name_vid("/home/hulio/RPI/CVFX/HW_2/rain_frames_alpha/alpha","Kung_Fu",i);
            string ori_n = get_file_name_vid("/home/hulio/RPI/CVFX/HW_2/rain_original/original","Kung_Fu",i);
            
            I = imread(ori_n.data());
            I.convertTo(I,CV_32FC3);
            normalize(I,I,0,1,NORM_MINMAX);                             // normalize to [0,1]
            
            alpha = imread(al_n.data(),0);
            alpha.convertTo(alpha,CV_32FC1);
            normalize(alpha,alpha,0,1,NORM_MINMAX);                             // normalize to [0,1]
    
}

int main(int argc, char *argv[])
{
    
   /*
    if(argc != 2)
    {
        cout << "\nFATAL ERROR: Incorrect number of parameters!\n";
        exit(1);
    }*/
    
//     VideoCapture cap;
//     cap.open(argv[1]);
    
    VideoWriter wir;
    int codec = CV_FOURCC('M','J','P','G');
    wir.open("Kung_Fu_Showdown.avi",codec,25,Size(1500 ,1080));
    
//     cap.set(CV_CAP_PROP_POS_MSEC,4000);

    vector<int> flist = get_fram_list();
    vector<int> flist2 = get_fram_list2();
    
    int fsize = min(flist.size(),flist2.size());
    int i = 0;
    
    Mat T = imread("shaolin.jpg");
    T.convertTo(T,CV_32FC3);
    normalize(T,T,0,1,NORM_MINMAX);                             // normalize to [0,1]
//     cout << "\n Flist Size:: "<< flist.size() << "\n";
    
    while(i < fsize ) // && (cap.get(CV_CAP_PROP_POS_FRAMES) < cap.get(CV_CAP_PROP_FRAME_COUNT)))   
    {
        
//             Mat I;
//             cap >> I;
            
            Mat I,alpha,I2,alpha2,If,alphaf;
            get_I_alpha(I,alpha,flist[i]);
            get_I_alpha(I2,alpha2,flist2[i]);

            if(i == 0)
            {
                resize(T,T,I.size());  
            }
            
            flip(I2,If,1);
            flip(alpha2,alphaf,1);
            
            
//             Mat T;
//             cap >> T;
/*            T.convertTo(T,CV_32FC3);
            normalize(T,T,0,1,NORM_MINMAX);                             // normalize to [0,1]
            resize(T,T,I.size());   */                                    // resize target to size of original image
        
            Mat newT = apply_alpha(I,alpha) + apply_alpha(T, 1-alpha);
            Mat newnewT = apply_alpha(If,alphaf) + apply_alpha(newT, 1-alphaf);
            normalize(newnewT,newnewT,0,255,NORM_MINMAX);
            newnewT.convertTo(newnewT,CV_8UC3);
            wir << newnewT;
            
//             string orinam = get_file_name_vid("/home/hulio/RPI/CVFX/HW_2/rain_original/original","Kung_Fu",i);
//             imwrite(orinam.data(),I);
//             frame_pross(I,T,i);
            i++;
//             cout << "\n Processing Frame::: " << i << "\n";
    }

//     cap.release();
    wir.release();
    
//     namedWindow("Matte", 1);
//     createTrackbar("a1", "Matte", &a1, am, track_1, new int(0) );       // a1
//     createTrackbar("a2", "Matte", &a2, am, track_1, new int(1) );       // a2   
//     createTrackbar("a3", "Matte", &a3, am, track_1, new int(2) );       // gamma
//     waitKey();

    
return 1;
}
