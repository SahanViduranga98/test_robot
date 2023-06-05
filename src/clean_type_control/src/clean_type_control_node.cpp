#include <ros/ros.h>
#include <std_msgs/String.h>
#include <pigpiod_if2.h>
int pin_number = 4;  // Example: GPIO pin 18
int start_pin;
void callback(const std_msgs::String::ConstPtr& msg)
{
  if (msg->data == "w")
  {
    // Set the GPIO pin voltage
    // Replace 'pin_number' with the actual GPIO pin number you want to control
   
    ROS_INFO("On");
  
    gpio_write(start_pin,pin_number, 1);  // Set pin voltage to HIGH
  }else{
    // int pin_number = 18;  // Example: GPIO pin 18
    gpio_write(start_pin,pin_number, 0); 
    ROS_INFO("Off");
  }
}

int main(int argc, char** argv)
{
  start_pin=pigpio_start(NULL,NULL);
  set_mode(start_pin,pin_number, PI_OUTPUT);
  set_pull_up_down(start_pin, pin_number, PI_PUD_DOWN);
  //ROS_INFO("Hello, ROS!");
  ros::init(argc, argv, "gpio_control_node");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("clean_type_manual", 10, callback);

  ros::spin();

  return 0;
}
