#!/usr/bin/env python

import rospy
from duck_commander.msg import Twist2DStamped
from getch import getch, pause


def talker():
    pub = rospy.Publisher('/duckiebot18/joy_mapper_node/car_cmd', Twist2DStamped, queue_size=10)
    rospy.init_node('duck_keyboard', anonymous=True)
    rate = rospy.Rate(10)  # 10hz
    while not rospy.is_shutdown():
        msg = Twist2DStamped()
        key = getch()
        # forward
        if key == 'w':
            msg.v = 0.3
            msg.omega = 0
        # back
        elif key == 's':
            msg.v = -0.3
            msg.omega = 0
        # left
        elif key == 'a':
            msg.v = 0.2
            msg.omega = 3
        # right
        elif key == 'd':
            msg.v = 0.2
            msg.omega = -3
        # quit
        elif key == 'q':
            msg.v = 0
            msg.omega = 0
            break
        # stop
        else:
            msg.v = 0
            msg.omega = 0

        rospy.loginfo(msg)
        pub.publish(msg)
        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
