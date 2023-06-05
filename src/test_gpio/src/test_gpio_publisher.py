#!/usr/bin/env python

import rospy
from std_msgs.msg import String

def publish_check_msg():
    rospy.init_node('test_gpio_publisher', anonymous=True)
    pub = rospy.Publisher('check', String, queue_size=10)
    rate = rospy.Rate(1)  # Publish once per second

    while not rospy.is_shutdown():
        msg = "yes"
        rospy.loginfo("Publishing: %s", msg)
        pub.publish(msg)
        rate.sleep()

if __name__ == '__main__':
    try:
        publish_check_msg()
    except rospy.ROSInterruptException:
        pass
