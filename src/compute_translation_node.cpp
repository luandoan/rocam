#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <opencv2/opencv.hpp>
#include <QTransform>
#include <find_object_2d/cam_translate.h>

/**
 * IMPORTANT :
 *      Parameter General/MirrorView must be false
 *      Parameter Homography/homographyComputed must be true
 */
ros::Publisher pub_err;

void objectsDetectedCallback(const std_msgs::Float32MultiArray & msg)
{
    printf("---\n");
    find_object_2d::cam_translate vecTran;
    if(msg.data.size())
    {
      for(unsigned int i=0; i<msg.data.size(); i+=12)
      {
        // get data
        int id = (int)msg.data[i];
        float objectWidth = msg.data[i+1];
        float objectHeight = msg.data[i+2];

        // Find corners Qt
        QTransform qtHomography(msg.data[i+3], msg.data[i+4], msg.data[i+5],
                    msg.data[i+6], msg.data[i+7], msg.data[i+8],
                    msg.data[i+9], msg.data[i+10], msg.data[i+11]);

        QPointF qtTopLeft = qtHomography.map(QPointF(0,0));
        QPointF qtTopRight = qtHomography.map(QPointF(objectWidth,0));
        QPointF qtBottomLeft = qtHomography.map(QPointF(0,objectHeight));
        QPointF qtBottomRight = qtHomography.map(QPointF(objectWidth,objectHeight));

  //			ROS_INFO("Object %d detected, Qt corners at (%f,%f) (%f,%f) (%f,%f) (%f,%f)\n",
  //					id,
  //					qtTopLeft.x(), qtTopLeft.y(),
  //					qtTopRight.x(), qtTopRight.y(),
  //					qtBottomLeft.x(), qtBottomLeft.y(),
  //					qtBottomRight.x(), qtBottomRight.y());
        float x1 = qtTopLeft.x();
        float y1 = qtTopLeft.y();
        float x2 = qtBottomRight.x();
        float y2 = qtBottomRight.y();
        float x3 = qtBottomLeft.x();
        float y3 = qtBottomLeft.y();
        float x4 = qtTopRight.x();
        float y4 = qtTopRight.y();


        float del = (x1-x2)*(y3-y4) -(y1-y2)*(x3-x4);
        if ( abs(del)<0.0001)
        {
          vecTran.translate_x = 0;
          vecTran.translate_y =0;
        }
        else
        {
           vecTran.translate_x  = (int)(((x1*y2 -y1*x2)*(x3-x4) - (x1 - x2)*(x3*y4 -y3*x4))/del) - 320;
           vecTran.translate_y  = -((int)(((x1*y2 -y1*x2)*(y3-y4) - (y1 - y2)*(x3*y4 - y3*x4))/del) - 240);
        }
        ROS_INFO(" Object %d detected, Center Point at (%d,%d)", id, vecTran.translate_x, vecTran.translate_y);
      }
		}
		else
		{
			ROS_INFO("No objects detected.\n");
			 vecTran.translate_x  = 0;
			 vecTran.translate_x  =0;
		}
		pub_err.publish(vecTran);
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "camera_track_object");

    ros::NodeHandle nh;
    ros::Subscriber subs;
    subs = nh.subscribe("objects", 1, objectsDetectedCallback);
    pub_err = nh.advertise<find_object_2d::cam_translate>("tracking_error",1);
    ros::spin();

    return 0;
}
