#include <ros/ros.h>
#include <std_msgs/String.h>

#include <termios.h>
#include <stdio.h>
#include <unistd.h>

int getch() {
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "keyboard_publisher");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<std_msgs::String>("keyboard_input", 10);

    std_msgs::String msg;

    while (ros::ok()) {
        int c = getch();

        if (c == '\n' || c == '\r')
            continue;

        msg.data = std::string(1, c);

        pub.publish(msg);

        if (c == 27)  // ASCII value for ESC key
            break;
    }

    return 0;
}
