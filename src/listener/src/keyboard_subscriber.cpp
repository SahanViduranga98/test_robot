#include <ros/ros.h>
#include <std_msgs/String.h>


void keyboardCallback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("Received keyboard input: %s", msg->data.c_str());
}

void arduinoCallback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("Received arduino msg: %s", msg->data.c_str());
}

void phoneCallback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("Received phone msg: %s", msg->data.c_str());
}
void powerCallback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("Received power type: %s", msg->data.c_str());
    ROS_INFO("Received power colum: %s", msg->data.c_str());
    ROS_INFO("Received power rows: %s", msg->data.c_str());
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "keyboard_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe<std_msgs::String>("keyboard_input", 10, keyboardCallback);
    ros::Subscriber sub1 = nh.subscribe<std_msgs::String>("newRow", 10, arduinoCallback);
    ros::Subscriber sub2 = nh.subscribe<std_msgs::String>("manualControl", 10, phoneCallback);
    ros::Subscriber sub3 = nh.subscribe<std_msgs::String>("powerOn", 10, powerCallback);
    

    ros::spin();

    return 0;
    
}
