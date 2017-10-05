#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <SceneKit/SceneKit.h>

@interface MarkerlessARWrapper: NSObject

- (id) init:(UIImage *)logo;
- (UIImage *) processFrame:(UIImage *)frame;
- (SCNMatrix4) getCameraProjection;
- (SCNMatrix4) getCameraPose;

@end
