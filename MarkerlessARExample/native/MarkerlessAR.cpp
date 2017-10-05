//
//  MarkerDetector.cpp
//  MarkerARExample
//
//  Created by Pablo Soto on 27/9/17.
//  Copyright © 2017 Neosentec. All rights reserved.
//

#include "MarkerlessAR.hpp"
#include "Utils.hpp"

    
MarkerlessAR::MarkerlessAR(Mat& logo) {
    
    // Set camera intrinsics (K)
    K = Mat::zeros(3, 3, CV_64F);
    K.at<double>(0,0) = CAMERA_WIDTH;
    K.at<double>(0,2) = CAMERA_WIDTH / 2;
    K.at<double>(1,1) = CAMERA_WIDTH;
    K.at<double>(1,2) = CAMERA_HEIGHT / 2;
    K.at<double>(2,2) = 1;
    
    double fx = K.at<double>(0,0);
    double fy = K.at<double>(1,1);
    double cx = K.at<double>(0,2);
    double cy = K.at<double>(1,2);
    double near = 0.1, far = 1000.0;
    
    // Set projection matrix (P)
    P = Mat::zeros(4,4, CV_64F);
    P.at<double>(0,0) = fx/cx;
    P.at<double>(1,1) = fy/cy;
    P.at<double>(2,2) = -(far+near)/(far-near);
    P.at<double>(2,3) = -2.0*far*near / (far-near);
    P.at<double>(3,2) = -1.0;
    P = P.t();
    
    // Set transformation matrix between OpenCV and OpenGL coordinate systems
    cv2gl = Mat::zeros(4, 4, CV_64F);
    cv2gl.at<double>(0,0) = 1;
    cv2gl.at<double>(1,1) = -1;
    cv2gl.at<double>(2,2) = -1;
    cv2gl.at<double>(3,3) = 1;
    
    // Create ORB detector with a max of 800 features.
    orb = ORB::create(800);
    
    // Compute logo keypoints and descriptors
    cvtColor(logo, grayLogo, CV_BGR2GRAY);
    orb->detectAndCompute(grayLogo, Mat(), logoKeypoints, logoDescriptors);
}

void MarkerlessAR::processFrame(Mat& frame) {
    
    // Convert to gray to improve speed/memory
    Mat grayFrame;
    cvtColor(frame, grayFrame, CV_BGR2GRAY);
    
    // If detecting ...
    if (!tracking) {
        
        // Compute frame keypoints and descriptors
        orb->detectAndCompute(grayFrame, Mat(), frameKeypoints, frameDescriptors);
        
        if (frameDescriptors.cols == logoDescriptors.cols) {
            
            // Match logo keypoints with frame keypoints
            vector<DMatch> matches;
            vector<DMatch> filteredMatches;
            BFMatcher(NORM_HAMMING, true).match(logoDescriptors, frameDescriptors, matches);
            
            // Keep only good matches
            ratioTest(matches, filteredMatches);
            logoTrackedPoints.clear();
            frameTrackedPoints.clear();
            for (int i = 0; i < filteredMatches.size(); i++) {
                logoTrackedPoints.push_back(logoKeypoints[filteredMatches[i].queryIdx].pt);
                frameTrackedPoints.push_back(frameKeypoints[filteredMatches[i].trainIdx].pt);
            }
            
            // Draw frame keypoints
            for (int i = 0; i < frameTrackedPoints.size(); i++) {
                circle(frame, frameTrackedPoints[i], 2, Scalar(0, 255, 0), -1);
            }
            
            if (filteredMatches.size() > 100) {
                
                // If enough matches, check if the scene is planar, and filter non-planar points
                Mat inliers;
                Mat homography = findHomography(frameTrackedPoints, logoTrackedPoints, RANSAC, 5.0, inliers);
                keepVectorsByStatus(logoTrackedPoints, frameTrackedPoints, inliers);
                
                if (frameTrackedPoints.size() > 50) {
                    
                    // Draw detection box
                    vector<Point2f> logoCorners;
                    logoCorners.push_back(Point2f(0, 0));
                    logoCorners.push_back(Point2f(640, 0));
                    logoCorners.push_back(Point2f(640, 480));
                    logoCorners.push_back(Point2f(0, 480));
                    vector<Point2f> frameCorners;

                    perspectiveTransform(logoCorners, frameCorners, homography.inv());
                    
                    line(frame, frameCorners[0], frameCorners[1], Scalar(0, 255, 0), 2);
                    line(frame, frameCorners[1], frameCorners[2], Scalar(0, 255, 0), 2);
                    line(frame, frameCorners[2], frameCorners[3], Scalar(0, 255, 0), 2);
                    line(frame, frameCorners[3], frameCorners[0], Scalar(0, 255, 0), 2);
                    
                    // ORB Keypoints are not very good for tracking, so compute Shi-Thomasi corners (goodFeaturesToTrack)
                    // and use them for tracking.
                    goodFeaturesToTrack(grayFrame, frameTrackedPoints, 800, 0.1, 30);
                    perspectiveTransform(logoTrackedPoints, frameTrackedPoints, homography.inv());
                    
                    tracking = true;
                }
            }
        }
        
    }
    
    // If tracking ...
    else {
        
        // Track with sparse optical-flow (Lucas-Kanade method)
        Mat status, err;
        vector<Point2f> newFrameTrackedPoints;
        calcOpticalFlowPyrLK(prevGray, grayFrame, frameTrackedPoints, newFrameTrackedPoints, status, err, Size(11,11));
        keepVectorsByStatus(newFrameTrackedPoints, logoTrackedPoints, status);
        frameTrackedPoints = newFrameTrackedPoints;
        
        // You may want to do some more filtering ...
        /*Mat inliers;
        Mat homography = findHomography(frameTrackedPoints, logoTrackedPoints, RANSAC, 10.0, inliers);
        keepVectorsByStatus(logoTrackedPoints, frameTrackedPoints, inliers);*/
        
        // Draw frame tracked points
        for (int i = 0; i < frameTrackedPoints.size(); i++) {
            circle(frame, frameTrackedPoints[i], 2, Scalar(255, 0, 0), -1);
        }
        
        // If enough points, compute camera pose
        if (frameTrackedPoints.size() > 25) {
            
            vector<Point3f> logoPoints3d;
            for (int i = 0; i < logoTrackedPoints.size(); i++) {
                Point2f p = logoTrackedPoints[i];
                logoPoints3d.push_back(Point3f(p.x - frame.cols/2, p.y - frame.rows/2, 0) * (1.0/frame.cols));
            }
            
            // Estimate object pose
            Mat rvec, tvec;
            solvePnP(logoPoints3d, frameTrackedPoints, K, Mat(), rvec, tvec, !rvec.empty());
            
            // Build [R | t] matrix
            Mat Rcw;
            Mat Tcw;
            rvec.convertTo(Rcw, CV_32F);
            tvec.convertTo(Tcw, CV_64F);
            Rodrigues(rvec, Rcw);
            
            // Generate OpenGL pose camera world
            Mat newPcw = Mat::eye(4,4, CV_64F);
            Rcw.convertTo(newPcw(Rect(0,0,3,3)),CV_64F);
            Tcw.copyTo(newPcw(Rect(3,0,1,3)));
            newPcw = (cv2gl * newPcw).t();
            
            if (!Pcw.empty()) {
                // Lowpass filter
                Pcw = Pcw + 0.3 * (newPcw - Pcw);
            } else {
                Mat(newPcw).copyTo(Pcw);
            }
        }
        else {
            tracking = false;
            Pcw = Mat();
        }
    }
    
    grayFrame.copyTo(prevGray);

}
