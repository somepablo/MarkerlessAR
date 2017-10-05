//
//  Utils.cpp
//  MarkerlessARExample
//
//  Created by Pablo Soto on 29/9/17.
//  Copyright © 2017 Neosentec. All rights reserved.
//

#include "Utils.hpp"

vector<Point2f> keypointsToPoints(vector<KeyPoint>& keypoints) {
    vector<Point2f> points;
    for (int i = 0; i < keypoints.size(); i++) {
        points.push_back(keypoints[i].pt);
    }
    return points;
}

vector<KeyPoint> pointsToKeypoints(vector<Point2f>& points) {
    vector<KeyPoint> keypoints;
    for (int i = 0; i < points.size(); i++) {
        keypoints.push_back(KeyPoint(points[i], 1, 0, 0));
    }
    return keypoints;
}

void keepVectorsByStatus(vector<KeyPoint>& v1, vector<KeyPoint>& v2, const vector<uchar>& status) {
    vector<KeyPoint> oldv1 = v1;
    vector<KeyPoint> oldv2 = v2;
    v1.clear();
    v2.clear();
    for (int i = 0; i < status.size(); ++i) {
        if(status[i])
        {
            v1.push_back(oldv1[i]);
            v2.push_back(oldv2[i]);
        }
    }
}

void keepVectorsByStatus(vector<Point2f>& v1, vector<Point2f>& v2, const vector<uchar>& status) {
    vector<Point2f> oldv1 = v1;
    vector<Point2f> oldv2 = v2;
    v1.clear();
    v2.clear();
    for (int i = 0; i < status.size(); ++i) {
        if(status[i])
        {
            v1.push_back(oldv1[i]);
            v2.push_back(oldv2[i]);
        }
    }
}

void keepVectorsByStatus(vector<Point2f>& v1, vector<Point3f>& v2, const vector<uchar>& status) {
    vector<Point2f> oldv1 = v1;
    vector<Point3f> oldv2 = v2;
    v1.clear();
    v2.clear();
    for (int i = 0; i < status.size(); ++i) {
        if(status[i])
        {
            v1.push_back(oldv1[i]);
            v2.push_back(oldv2[i]);
        }
    }
}

void keepVectorsByStatus(vector<Point2f>& v1, vector<Point2f>& v2, vector<Point3f>& v3, const vector<uchar>& status) {
    vector<Point2f> oldv1 = v1;
    vector<Point2f> oldv2 = v2;
    vector<Point3f> oldv3 = v3;
    v1.clear();
    v2.clear();
    v3.clear();
    for (int i = 0; i < status.size(); ++i) {
        if(status[i])
        {
            v1.push_back(oldv1[i]);
            v2.push_back(oldv2[i]);
            v3.push_back(oldv3[i]);
        }
    }
}

void ratioTest(vector<DMatch> &matches, vector<DMatch>& goodMatches) {
    double minDist = 1000;
    for (int i = 0; i < matches.size(); i++) {
        double dist = matches[i].distance;
        if( dist < minDist ) minDist = dist;
    }
    for (int i = 0; i < matches.size(); i++) {
        if (matches[i].distance < 4 * minDist)
        {
            goodMatches.push_back(matches[i]);
        }
    }
}
