//
//  Utils.hpp
//  MarkerlessARExample
//
//  Created by Pablo Soto on 29/9/17.
//  Copyright © 2017 Neosentec. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<Point2f> keypointsToPoints(vector<KeyPoint>& keypoints);
vector<KeyPoint> pointsToKeypoints(vector<Point2f>& points);
void keepVectorsByStatus(vector<KeyPoint>& v1, vector<KeyPoint>& v2, const vector<uchar>& status);
void keepVectorsByStatus(vector<Point2f>& v1, vector<Point2f>& v2, const vector<uchar>& status);
void keepVectorsByStatus(vector<Point2f>& v1, vector<Point3f>& v2, const vector<uchar>& status);
void keepVectorsByStatus(vector<Point2f>& v1, vector<Point2f>& v2, vector<Point3f>& v3, const vector<uchar>& status);
void ratioTest(vector<DMatch> &matches, vector<DMatch>& goodMatches);

#endif /* Utils_hpp */
