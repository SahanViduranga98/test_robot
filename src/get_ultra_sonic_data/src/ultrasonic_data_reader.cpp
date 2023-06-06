#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <pigpiod_if2.h>
#include <bitset>
#include <iostream>
#include <vector>
#include <std_msgs/String.h>
// #include <wiringPi.h>
// #include <wiringPiI2C.h>
int start_pin;
int pi;
   // Connect to the pigpiod daemon
std::string edge_find(const std::vector<int> left_sensor,const std:: vector<int> right_sensor){
    std::string find_edge="0";
    if(left_sensor[0]==1 && left_sensor[1]==1 && right_sensor[0]==1 && right_sensor[1]==1){
        find_edge="3";
        std::cout << find_edge<<std::endl;
    }else if(left_sensor[2]==1 && right_sensor[2]==1){
        find_edge="3";
    }else if(left_sensor[1]){
        find_edge="4";
    }else if(right_sensor[1]){
        find_edge="5";
    }
    return find_edge;
}
int read_distance_from_sensor(int handle) {
    int data=0;
   
    //ROS_INFO("pi:%d",pi);
        
        //std::cout<<handle<<std::endl
            // Read data from the I2C device
            // Replace with your actual I2C communication code
            data = i2c_read_byte(pi, handle);
            // std::cout<<data<<std::endl;

            std_msgs::Float32 msg;
            msg.data = static_cast<float>(data);
            if(msg.data<0){
                data=-1;
            }
            return data;

            //ROS_INFO("I2C Data: %f", msg.data);
            //pub.publish(msg);

             // Close the I2C device
        // }else{
        //     ROS_INFO("I2C handle not received");
        //     return -1;
        // }
        //i2c_close(pi, handle); 
        //pigpio_stop(pi);  // Disconnect from the pigpiod daemon
    // }else{
    //     ROS_INFO("Not connected to pigpio daemon");
    //     return -1;
    // }
    
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "ultrasonic_publisher");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("ultrasonic_data", 10);
    ros::Publisher pub_edge_warn = nh.advertise<std_msgs::String>("warning", 1);
    ros::Rate rate(10);  // Publish at a rate of 10 Hz
    std_msgs::String msg_warn;
    pi = pigpio_start(NULL,NULL);
    int address1=0x10;
    int address2=0x09;
    int handle1 = i2c_open(pi,1, address1, 0);  // Open the I2C device
    int handle2 = i2c_open(pi,1, address2, 0);  // Open the I2C device
    if(handle1<0 || handle2<0){
        
        msg_warn.data="1";
       // pub_edge_warn(msg_warn);
        pub_edge_warn.publish(msg_warn);

    }
    else{
        std::cout<<handle1<<" "<<handle2<<std::endl;
    }
    //int i2c_handle =i2cOpen(1,0x68,0);  // Use I2C bus 1
         while (ros::ok()) {
            
        // Read distance from ultrasonic sensor via I2C
        // Replace with your actual I2C communication code
      
      
        //device_wait(100);
        int distance_right = read_distance_from_sensor(handle1);
        int distance_left = read_distance_from_sensor(handle2);

        if(distance_left<0 || distance_right<0){
            msg_warn.data="1";
           // pub_edge_warn(msg_warn);
            pub_edge_warn.publish(msg_warn);
        }else{
            std_msgs::String msg;
            std::string binary1 = std::bitset<3>(distance_right).to_string();
    // Pad the binary string with leading zeros if necessary
            binary1 = std::string(3 - binary1.length(), '0') + binary1;
            std::vector<int> right_sensor;
            for(char bit:binary1){
                int digit=bit-'0';
                right_sensor.push_back(digit);
            }

            std::string binary2 = std::bitset<3>(distance_left).to_string();
    // Pad the binary string with leading zeros if necessary
            binary2 = std::string(3 - binary2.length(), '0') + binary2;
            std::vector<int> left_sensor;
            for(char bit:binary2){
                int digit=bit-'0';
                left_sensor.push_back(digit);
            }

            std::cout<<"Left UltraSonicData:"<<left_sensor[0]<<" "<<left_sensor[1]<<" "<<left_sensor[2]<<std::endl;
           std::cout<<"Right UltraSonicData:"<<right_sensor[0]<<" "<<right_sensor[1]<<" "<<right_sensor[2]<<std::endl;

            std::string edge_state=edge_find(left_sensor,right_sensor);
            msg.data=edge_state;
            std::cout << msg.data<<std::endl;

            msg_warn.data=edge_state;
            //pub_edge_warn(msg_warn);
            pub_edge_warn.publish(msg_warn);

            pub.publish(msg);
        }

            
            ros::spinOnce();
            rate.sleep();
        }
    pigpio_stop(pi); 
    return 0;
}
