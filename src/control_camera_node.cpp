#include <ros/ros.h>
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include "rocam/velocity.h"


//const std::string right = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=6&onestep=1";
//const std::string left = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=4&onestep=1";
//const std::string up = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=0&onestep=1";
//const std::string down = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=2&onestep=1";

//const std::string right_up = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=91&onestep=1";
//const std::string right_down = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=93&onestep=1";
//const std::string left_up = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=0&onestep=1";
//const std::string left_down = "http://192.168.1.36/decoder_control.cgi?loginuse=admin&loginpas=12345&command=2&onestep=1";

const std::string right = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=6&onestep=1";
const std::string left = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=4&onestep=1";
const std::string up = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=0&onestep=1";
const std::string down = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=2&onestep=1";

const std::string right_up = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=91&onestep=1";
const std::string right_down = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=93&onestep=1";
const std::string left_up = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=0&onestep=1";
const std::string left_down = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=2&onestep=1";

std::string NumberToString (int Number )
{
	std::stringstream ss;
	ss << Number;
	return ss.str();
}

void control_ROCAM_motionCB(const rocam::velocity::ConstPtr &msg)
{
  int n1 = msg->direction;
  int n2 = msg->step;
  std::string command_cam = "http://192.168.1.80:81/decoder_control.cgi?loginuse=admin&loginpas=12345&command=" + NumberToString(n1) + "&onestep=" + NumberToString(n2);
  ROS_DEBUG("I heard: %s", command_cam.c_str());

  CURL *curl =curl_easy_init();
  CURLcode res;

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, command_cam.data());
    /* example.com is redirected, so we tell libcurl to follow redirection */
    //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      ROS_ERROR( "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "control_ROCAM_motion");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("control_ROCAM",1000,control_ROCAM_motionCB);
  ros::spin();
  return 0;
}
