#include <ros/ros.h>
#include <std_msgs/String.h>

// Callback function for the "edge_warn" topic
void errorCallback(const std_msgs::String::ConstPtr& msg)
{
    // Perform error handling logic here
    ROS_INFO_STREAM("Received edge warning: " << msg->data);

    // Publish error message on "error" topic
        ros::NodeHandle nh;
        ros::Publisher errorPub = nh.advertise<std_msgs::String>("warning", 1);
        std_msgs::String errorMsg;
        errorMsg.data = msg->data;
        errorPub.publish(errorMsg);

   
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "error_handling");
    ros::NodeHandle nh;

    // Subscribe to the "edge_warn" topic
    ros::Subscriber edgeWarnSub = nh.subscribe("edge_warn", 10, errorCallback);
    ros::Subscriber tiltWarnSub = nh.subscribe("tilt_warning", 10, errorCallback);
    ros::Subscriber imuWarnSub = nh.subscribe("imu_warning", 10, errorCallback);

    ros::spin();

    return 0;
}
