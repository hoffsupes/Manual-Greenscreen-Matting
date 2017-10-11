#include <iostream>
#include <string>
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

int a1,a2,a3,a4;
int k = 10;                                         // Values between +/- k needed
int am = 2*k*1000;                                  // 2k values considered, then values scaled down to smaller ones i.e. if n lies between 0 to 2k then it will be considered 
                                                    // as (n - k)/1000. Thus being mapped to a number between -k and k. The 1000 is there to allow for very fine values between 
                                                    // +k and -k as the threshold values adjust better to smaller changes it seems
Mat I,T;
float al1,al2,al3,al4;

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

string get_file_name(string img, string spc,float a1, float a2,float a3,float a4)
{
        
    
    string a_1 = to_string(a1);
    string a_2 = to_string(a2);
    string a_3 = to_string(a3);
    string a_4 = to_string(a4);
    
    string nam(img.data());
    nam.append(spc.data());
    nam.append("_");
    nam.append(a_1);
    nam.append("_");
    nam.append(a_2);
    nam.append("_");
    nam.append(a_3);
    nam.append("_");
    nam.append(a_4);
    nam.append(".jpeg");

    return nam;
    
}

void track_1( int, void* opn )
{
    
    int *val = reinterpret_cast<int*>(opn);
    
    if(*val == 0)
    {
    a1 = a1 - (k*1000);                                 // normalization technique to manually allow values between +k and -k (The OpenCV slider does not allow -ve values)
    al1 = float(a1)/float(1000);
    cout << "\n a1:" << al1 << "\n";                    //a1
    }
    else if( *val == 1)
    {
    a2 = a2 - (k*1000);    
    al2 = float(a2)/float(1000);
    cout << "\n a2:" << al2 << "\n";                    //a2
    
    }
    else if(*val == 2)
    {
    a3 = a3 - (k*1000);   
    al3 = float(a3)/float(1000);
    cout << "\n a3:" << al3 << "\n";                    //gamma1
    }
    else if(*val == 3)
    {
    a4 = a4 - (k*1000);   
    al4 = float(a4)/float(1000);
    cout << "\n a3:" << al4 << "\n";                    //gamma2
    }
 
 
    Mat Ib,Ig,Ir;
    vector<Mat> v;
    split(I,v);
    
    Ib = v[0];                                          // OpenCV follows the BGR scheme instead of a RGB scheme
    Ig = v[1];
    Ir = v[2];
   
    Mat alpha = 1 - al1*(Ig - al2*(Ib));                // The matting equation
    normalize(alpha,alpha,0,1,NORM_MINMAX);             // normalize between 0 and 1

    Mat mal,m2,m3;                                      //
    m2 = Mat::zeros(alpha.size(),CV_32F);               //
    m3 = Mat::zeros(alpha.size(),CV_32F);               //
    alpha.copyTo(mal);                                  //
    mal.convertTo(mal,CV_8U);                           //
    threshold(mal,mal,255,1,THRESH_OTSU);               // determine mask to threshold only the FG region in alpha
    Mat mal2 = 255*(mal==0);
    
    alpha.copyTo(m2,mal); 
    alpha.copyTo(m3,mal2); 
    
    pow(m2,al3,m2);                 
    pow(m3,-al4,m3);                 
    
    m2.copyTo(alpha,mal);                               // gamma correct only portion specified by mask
    m3.copyTo(alpha,mal2);                               // gamma correct only portion specified by mask
    
    
    Mat aly;
    resize(alpha,aly,alpha.size()/3);

    Mat Y =  apply_alpha(T, 1-alpha) + apply_alpha(I, alpha);       // apply alpha, multiply alpha to each channel of the given image
    
    normalize(Y,Y,0,255,NORM_MINMAX);                               // normalize resultant to 0 to 255 and save as an 8 bit image
    Y.convertTo(Y,CV_8U); 
    Mat yul;
    resize(Y,yul,I.size()/3);
    
    imshow("Composite",yul);                                        // live display with change of values (scaled down)
    imshow("Matte",aly);    
       
    string alnam = get_file_name("alpha","img",al1,al2,al3,al4);
    string comnam = get_file_name("composite","img",al1,al2,al3,al4);
    
    normalize(alpha,alpha,0,255,NORM_MINMAX);
    alpha.convertTo(alpha,CV_8U); 
    imwrite(comnam.data(),Y);                                       // write files
    imwrite(alnam.data(),alpha);

    
    
    
}

int main(int argc, char *argv[])
{
    
   
    if(argc != 3)
    {
        cout << "\nFATAL ERROR: Incorrect number of parameters!\n";
        exit(1);
    }
        
    
    I = imread(argv[1]);
    I.convertTo(I,CV_32FC3);
    normalize(I,I,0,1,NORM_MINMAX);                             // normalize to [0,1]
    
    T = imread(argv[2]);
    T.convertTo(T,CV_32FC3);
    normalize(T,T,0,1,NORM_MINMAX);                             // normalize to [0,1]
    resize(T,T,I.size());                                       // resize target to size of original image

    
    namedWindow("Matte", 1);
    createTrackbar("a1", "Matte", &a1, am, track_1, new int(0) );       // a1
    createTrackbar("a2", "Matte", &a2, am, track_1, new int(1) );       // a2   
    createTrackbar("a3", "Matte", &a3, am, track_1, new int(2) );       // gamma1
    createTrackbar("a4", "Matte", &a4, am, track_1, new int(3) );       // gamma2
    waitKey();

    
return 1;
}
