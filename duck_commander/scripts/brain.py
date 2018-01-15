#!/usr/bin/env python

import rospy
import time
from duck_commander.msg import Twist2DStamped
from duck_commander.msg import RectLocation

# global variables for PID and Exp Average
last_x = 320
last_area = 8000
alpha = 0.8


def car_control(x, area):
    # PD control for v
    e = area - 8000
    d = area - last_area
    Kp = -0.00005
    Kd = 0
    v = Kp * e + Kd * d

    # PD control for omega
    e = x - 320
    d = x - last_x
    Kp = 0.01
    Kd = 0.005
    omega = Kp * e + Kd * d

    # constrain, motor protection
    if v > 0.5:
        v = 0.5
    elif v < -0.2:
        v = -0.2
    elif abs(v) < 0.05:
        v = 0
    if omega > 3:
        omega = 3
    elif omega < -3:
        omega = -3
    elif abs(omega) < 0.1:
        omega = 0

    time.sleep(0.1)
    return v, omega


def talker(vel):
    rate = rospy.Rate(5)  # 5hz
    v, omega = vel
    pub = rospy.Publisher('/duckiebot18/joy_mapper_node/car_cmd', Twist2DStamped, queue_size=1)
    msg = Twist2DStamped()
    msg.v = v
    msg.omega = omega
    rospy.loginfo(msg)
    pub.publish(msg)
    rate.sleep()


def callback(data):
    global last_x, last_area
    # Exp average
    x = alpha * data.x + (1-alpha)*last_x
    area = alpha * data.area + (1-alpha)*last_area
    vel = car_control(x, area)
    talker(vel)
    last_x = x
    last_area = area


def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('duck_brain', anonymous=True)

    rospy.Subscriber('/rect_location', RectLocation, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':
    listener()
