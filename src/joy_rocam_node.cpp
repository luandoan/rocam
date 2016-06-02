#include <ros/ros.h>
#include <rocam/velocity.h>
#include <sensor_msgs/Joy.h>

class TeleopROCAM
{
public:
  TeleopROCAM();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  int linear_, angular_;
  double l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;
};

TeleopROCAM::TeleopROCAM():
  linear_(1),
  angular_(2)
  {
    nh_.param("axis_linear", linear_, linear_);
    nh_.param("axis_angular", angular_, angular_);
    nh_.param("scale_angular", a_scale_, a_scale_);
    nh_.param("scale_linear", l_scale_, l_scale_);

    vel_pub_ = nh_.advertise<rocam::velocity>("control_ROCAM", 1);
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy",10, &TeleopROCAM::joyCallback, this);
  }

void TeleopROCAM::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    rocam::velocity vel;
    vel.direction = 7;
    vel.step = 0;

    if (joy->axes[angular_] > 0)
        vel.direction = 4;
      else if (joy->axes[angular_] < 0)
        vel.direction = 6;
      else
        vel.step = 1;

    if (joy->axes[linear_] > 0)
        vel.direction = 0;
      else if (joy->axes[linear_] < 0)
        vel.direction = 2;
      else
        vel.step = 1;


    vel_pub_.publish(vel);
}



int main(int argc, char** argv)
{
  ros::init(argc, argv, "joy_rocam");
  TeleopROCAM teleop_rocam;

  ros::spin();
}
