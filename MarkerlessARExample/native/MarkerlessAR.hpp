//
//  MarkerDetector.hpp
//  MarkerARExample
//
//  Created by Pablo Soto on 27/9/17.
//  Copyright © 2017 Neosentec. All rights reserved.
//

#ifndef MarkerlessAR_hpp
#define MarkerlessAR_hpp

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

    
class MarkerlessAR {
     
private:
        
    const int CAMERA_WIDTH = 640;
    const int CAMERA_HEIGHT = 480;
     
    Mat K;                                          // Camera intrinsics
    Mat P;                                          // Projection matrix
    Mat cv2gl;                                      // OpenCV to OpenGL transformation matrix
    Mat Pcw;                                        // Pose camera world (OpenGL coordinates)
    
    Mat grayLogo;                                   // Logo to detect and track
    Mat prevGray;                                   // Previous gray frame for opticalFlow
    
    Ptr<ORB> orb;                                   // ORB Feature detector
    
    vector<KeyPoint> logoKeypoints;                 // Logo and frame keypoins, tracked points and descriptors
    vector<Point2f> logoTrackedPoints;
    Mat logoDescriptors;
    
    vector<KeyPoint> frameKeypoints;
    vector<Point2f> frameTrackedPoints;
    Mat frameDescriptors;
    
    bool tracking = false;                          // status flag to swith between detection and tracking modes
    
    
        
public:
        
    MarkerlessAR(Mat& logo);
    void processFrame(Mat& frame);
    Mat getCameraProjection() { return P; }
    Mat getCameraPose() { return Pcw; }
    
};

#endif
