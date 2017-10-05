//
//  MarkerARWrapper.m
//  MarkerARExample
//
//  Created by Pablo Soto on 27/9/17.
//  Copyright © 2017 Neosentec. All rights reserved.
//

#import "MarkerlessARExample-Bridging-Header.h"
#import <opencv2/opencv.hpp>
#import "UIImage+OpenCV.h"
#import "MarkerlessAR.hpp"

@implementation MarkerlessARWrapper: NSObject

MarkerlessAR *markerlessAR = NULL;

- (id) init:(UIImage *)logo
{
    self = [super init];
    cv::Mat matLogo = [logo CVMat3];
    markerlessAR = new MarkerlessAR::MarkerlessAR(matLogo);
    return self;
}

- (UIImage *)processFrame:(UIImage *)frame
{
    cv::Mat matFrame = [frame CVMat3];
    markerlessAR->processFrame(matFrame);
    return [UIImage imageWithCVMat:matFrame];
}

- (SCNMatrix4) getCameraPose {
    GLKMatrix4 glkMatrix;
    if (!markerlessAR->getCameraPose().empty()) {
        for(int i = 0; i < 16; i++) {
            glkMatrix.m[i] = markerlessAR->getCameraPose().at<double>(i);
        }
    }
    return SCNMatrix4Invert(SCNMatrix4FromGLKMatrix4(glkMatrix));
}

- (SCNMatrix4) getCameraProjection {
    GLKMatrix4 glkMatrix;
    for (int i = 0; i < 16; i++) {
        glkMatrix.m[i] = markerlessAR->getCameraProjection().at<double>(i);
    }
    return SCNMatrix4FromGLKMatrix4(glkMatrix);
}

@end
