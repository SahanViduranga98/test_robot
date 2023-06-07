#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int64MultiArray.h>


ros::Publisher auto_velocity;
int raw_count=0;
int colums_count=0;
std::string clean_type ="d";
bool start_auto=false;
bool forward_edge=false;
bool back_edge=false;
bool left_edge=false;
bool right_edge=false;

bool previous_turn=true;//true->left,false->right
void edgeDetectCallback(const std_msgs::String::ConstPtr& msg){
    if(msg->data=="2"){
        forward_edge=true;
    }else if(msg->data=="3"){
        back_edge=true;
    }else if(msg->data=="4"){
        left_edge=true;
    }else if(msg->data=="5"){
        right_edge=true;
    }else{
         forward_edge=false;
         back_edge=false;
        left_edge=false;
         right_edge=false;
    }
}
// Callback function for the "edge_warn" topic
void autoDriveVelocityCallback(const std_msgs::Int64MultiArray::ConstPtr& msg)
{
    // Perform error handling logic here
   // ROS_INFO_STREAM("Received edge warning: " << msg->data);

    // Publish error message on "error" topic
    std_msgs::Int64MultiArray velocity;
    velocity.layout.dim.push_back(std_msgs::MultiArrayDimension());
    velocity.layout.dim[0].size=2;
    velocity.layout.dim[0].stride=1;
    velocity.layout.dim[0].label="auto_velocity";
    if(start_auto){
        if(!forward_edge){
            velocity.data.push_back(msg->data[0]);
            velocity.data.push_back(msg->data[1]);

        }
        else{
            velocity.data.push_back(0);
            velocity.data.push_back(0);
            if(previous_turn){
                //turn_right
                previous_turn=~previous_turn;
            }else{
                //turn right
                previous_turn=~previous_turn;
            }

        }
        ros::NodeHandle auto_drive;
        //ros::Publisher errorPub = auto_drive.advertise<std_msgs::String>("error", 10);
        auto_velocity = auto_drive.advertise<std_msgs::Int64MultiArray>("auto_velocity",10);
        std::cout<<"left:"<<msg->data[0]<<std::endl;
        std::cout<<"right:"<<msg->data[1]<<std::endl;

        
        //errorMsg.data = "An error occurred while processing edge warning.";
        auto_velocity.publish(velocity);
    }
    
}
void setRawCallback(const std_msgs::String::ConstPtr& msg){
    std::string raw=msg->data;
    std::cout<<"rows:"<<raw<<std::endl;
    raw_count=std::stoi(raw);
}

void setColumCallback(const std_msgs::String::ConstPtr& msg){
    std::string colum=msg->data;
    std::cout<<"colums:"<<colum<<std::endl;
    colums_count=std::stoi(colum);
}

void clean_typeCallback(const std_msgs::String::ConstPtr& msg){
    clean_type=msg->data;
    std::cout<<"clean type:"<<clean_type<<std::endl;
}
void autoStartCallback(const std_msgs::String::ConstPtr& msg){
    if(msg->data=="1"){
        std::cout<<"start:"<<msg->data<<std::endl;
        start_auto=true;
        if(left_edge){
            previous_turn=false;//initially turn right
        }else if(right_edge){
            previous_turn=true;//initially turn left
        }
    }else{
        std::cout<<"start:"<<msg->data<<std::endl;
        start_auto=false;
    }
}


int main(int argc, char** argv)
{
   ros::init(argc, argv, "autonomus_driving");
ros::NodeHandle nh;
    
    
    // Subscribe to the "edge_warn" topic
    ros::Subscriber sub1  = nh.subscribe("rows", 10, setRawCallback);
    ros::Subscriber sub2  = nh.subscribe("columns", 10, setColumCallback);
    ros::Subscriber sub3  = nh.subscribe("clean_type", 10, clean_typeCallback);
    ros::Subscriber sub4  = nh.subscribe("startRobot", 10, autoStartCallback);
    ros::Subscriber sub5 = nh.subscribe("line_follow_topic", 10, autoDriveVelocityCallback);
    ros::Subscriber sub6 = nh.subscribe("warning", 10, edgeDetectCallback);

    ros::spin();

    return 0;
}
