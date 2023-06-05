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

std::string edge_find(const std::vector<int> left_sensor,const std:: vector<int> right_sensor){
    std::string find_edge="no";
    if(left_sensor[0]==1 && left_sensor[1]==1 && right_sensor[0]==1 && right_sensor[1]==1){
        find_edge="fp";
        std::cout << find_edge<<std::endl;
    }else if(left_sensor[2]==1 && right_sensor[2]==1){
        find_edge="bp";
    }else if(left_sensor[1]){
        find_edge="lp";
    }else if(right_sensor[1]){
        find_edge="rp";
    }
    return find_edge;
}
int read_distance_from_sensor(int address,int regester) {
    int data=0;
    int pi = pigpio_start(NULL,NULL);  // Connect to the pigpiod daemon
    //ROS_INFO("pi:%d",pi);
    if (pi >= 0) {
        int handle = i2c_open(pi, 1, address, 0);  // Open the I2C device

        if (handle >= 0) {
            // Read data from the I2C device
            // Replace with your actual I2C communication code
            data = i2c_read_byte_data(pi, handle, regester);

            std_msgs::Float32 msg;
            msg.data = static_cast<float>(data);

            //ROS_INFO("I2C Data: %f", msg.data);
            //pub.publish(msg);

            i2c_close(pi, handle);  // Close the I2C device
        }else{
            ROS_INFO("I2C handle not received");
        }

        pigpio_stop(pi);  // Disconnect from the pigpiod daemon
    }else{
        ROS_INFO("Not connected to pigpio daemon");
    }
    return data;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "ultrasonic_publisher");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("ultrasonic_data", 10);
    ros::Rate rate(10);  // Publish at a rate of 10 Hz
    //int i2c_handle =i2cOpen(1,0x68,0);  // Use I2C bus 1
         while (ros::ok()) {
        // Read distance from ultrasonic sensor via I2C
        // Replace with your actual I2C communication code
            int distance_right = read_distance_from_sensor(0x01,0x03);
            int distance_left = read_distance_from_sensor(0x02,0x04);

            std_msgs::String msg;
            //msg.data = static_cast<float>(distance1);

            //ROS_INFO("Ultrasonic Distance x:  %f", msg.data);
          //  ROS_INFO("Ultrasonic Distance y:  %d", distance2);
           // int data1 = 10; // Example data value

            std::string binary1 = std::bitset<3>(distance_right).to_string();
    // Pad the binary string with leading zeros if necessary
            binary1 = std::string(3 - binary1.length(), '0') + binary1;
            std::vector<int> left_sensor;
            for(char bit:binary1){
                int digit=bit-'0';
                left_sensor.push_back(digit);
            }

            std::string binary2 = std::bitset<3>(distance_left).to_string();
    // Pad the binary string with leading zeros if necessary
            binary2 = std::string(3 - binary2.length(), '0') + binary2;
            std::vector<int> right_sensor;
            for(char bit:binary2){
                int digit=bit-'0';
                right_sensor.push_back(digit);
            }

            std::cout<<"Left UltraSonicData:"<<left_sensor[0]<<" "<<left_sensor[1]<<" "<<left_sensor[2]<<std::endl;
            std::cout<<"Right UltraSonicData:"<<right_sensor[0]<<" "<<right_sensor[1]<<" "<<right_sensor[2]<<std::endl;

            std::string edge_state=edge_find(left_sensor,right_sensor);
            msg.data=edge_state;
            std::cout << msg.data<<std::endl;


            pub.publish(msg);
            ros::spinOnce();
            rate.sleep();
        }

    return 0;
}
