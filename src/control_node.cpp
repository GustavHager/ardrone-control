#include "ros/ros.h"
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Empty.h>

ros::Publisher emergency_land;
ros::Publisher takeoff;
ros::Publisher land;

ros::Publisher command_vel;

void controller_callback(const sensor_msgs::Joy joy_input){

  //Require bumber and first button to takeoff, press of second button will perform controlled landing
  if(joy_input.buttons[0] == 1){
    ROS_INFO("Taking off");
    takeoff.publish(std_msgs::Empty());
  }else if(joy_input.buttons[1] == 1){
    ROS_INFO("Landing");
    land.publish(std_msgs::Empty());
  }


  ROS_INFO_STREAM(joy_input);
  //translate the joystick message into a twist message for the drone
  float ctrl_forward = joy_input.axes[4];
  float ctrl_left = joy_input.axes[3];

  float ctrl_up = joy_input.axes[1];
  float ctrl_spin = joy_input.axes[0];


  geometry_msgs::Twist drone_command;
  drone_command.linear.x = ctrl_forward;
  drone_command.linear.y = ctrl_left;
  drone_command.linear.z = ctrl_up;
  drone_command.angular.x = 0;
  drone_command.angular.y = 0;
  drone_command.angular.z = ctrl_spin;

  command_vel.publish(drone_command);

  ROS_INFO_STREAM("drone command is " << drone_command);
}

int main(int argc, char** argv){
  ros::init(argc,argv,"xbone_controller");

  ros::NodeHandle nh;
  ROS_INFO("Setting up publishers");

  //setup the control publishes for drone
  emergency_land = nh.advertise<std_msgs::Empty>("ardrone/reset",1);
  takeoff = nh.advertise<std_msgs::Empty>("ardrone/takeoff",1);
  land = nh.advertise<std_msgs::Empty>("ardrone/land",1);

  command_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1);

  ros::Subscriber joy_sub = nh.subscribe("joy",100,controller_callback);


  ros::spin();
  
  //setup the subscriber for the controller
  return 0;
}
