
#include <ros/ros.h>
#include <rocam/velocity.h>
#include "find_object_2d/cam_translate.h"

class track_object_ROCAM
{
  public:
    track_object_ROCAM();
  private:
    void control_ROCAM_CB(const find_object_2d::cam_translate::ConstPtr& msg);
    ros::NodeHandle nh_;
    ros::Subscriber sub_;
    ros::Publisher pub_;
    rocam::velocity vel;

};

track_object_ROCAM::track_object_ROCAM()
{
  sub_ = nh_.subscribe("tracking_error", 1, &track_object_ROCAM::control_ROCAM_CB, this);
  pub_ = nh_.advertise<rocam::velocity>("control_ROCAM",1);
}

void track_object_ROCAM::control_ROCAM_CB(const find_object_2d::cam_translate::ConstPtr& msg)
{
  vel.direction = 7; // just a dumb invalid command
  vel.step = 0; //keep moving
  const int tol = 40; //tolerance. if object is in the square of radius = tol, then stop moving
  if  (msg->translate_x < -tol) //left
    {
      if (msg->translate_y < -tol) //down
        vel.direction = 92; //left down
      else if (msg->translate_y > tol) //up
        vel.direction = 90; //left up
      else
        vel.direction = 4; //only move to the left
    }
  else if (msg->translate_x > tol) //right
    {
      if (msg->translate_y < -tol) //down
        vel.direction = 93; //right down
      else if (msg->translate_y > tol) //up
        vel.direction = 91; //right up
      else
        vel.direction = 6; //only move to the right
    }
  else
    {
      if (msg->translate_y < -tol) //down
        vel.direction = 2; //only move down
      else if (msg->translate_y > tol) //up
        vel.direction = 0; //only move up
      else //the object is already in the center
        vel.step = 1; //stop
    }

  pub_.publish(vel);
}


int main(int argc, char *argv[])
{
  ros::init(argc,argv, "track_cam_node");
  track_object_ROCAM track_ROCAM_motion;
  ros::spin();
}
