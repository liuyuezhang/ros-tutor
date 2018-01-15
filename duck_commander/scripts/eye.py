#!/usr/bin/env python

import cv2
import numpy as np
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import CompressedImage
from duck_commander.msg import RectLocation

cnt = 0
state_res_x = 320
state_area = 8000


def loc_barcode(img):
    # convert to grey
    grey = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # gaussian blur
    gb = cv2.GaussianBlur(grey, (5, 5), 0)

    # edge
    edge = cv2.Canny(gb, 100, 200)
    cv2.imshow("edge", edge)

    img_fc, contours, hierarchy = cv2.findContours(edge, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    hierarchy = hierarchy[0]
    found = []
    for i in range(len(contours)):
        k = i
        c = 0
        while hierarchy[k][2] != -1:
            k = hierarchy[k][2]
            c = c + 1
        if c >= 5:
            found.append(i)

    draw_img = img.copy()

    # find 3 mark points
    if len(found) == 3:
        x = []
        y = []
        for i in found:
            rect = cv2.minAreaRect(contours[i])
            x.append(rect[0][0])
            y.append(rect[0][1])
            box = cv2.boxPoints(rect)
            box = np.int0(box)
            cv2.drawContours(draw_img, [box], 0, (0, 255, 0), 3)
            cv2.imshow("draw_img", draw_img)
        x_min = min(x)
        x_max = max(x)
        y_min = min(y)
        y_max = max(y)
        res_x = (x_min+x_max)/2
        area = (y_max-y_min)*(x_max-x_min)
        global state_res_x, state_area
        state_res_x = res_x
        state_area = area
        return res_x, area
    # else:
    #     global cnt
    #     cnt += 1
    #     if cnt > 10:
    #         cnt = 0
    #         return 320, 8000
    #     else:
    #         return state_res_x, state_area
    else:
        return 320, 8000


def talker(loc):
    x, area = loc
    pub = rospy.Publisher('/rect_location', RectLocation, queue_size=1)
    msg = RectLocation()
    msg.x = x
    msg.area = area
    rospy.loginfo(msg)
    pub.publish(msg)


def callback(data):
    img = bridge.compressed_imgmsg_to_cv2(data)
    loc = loc_barcode(img)
    if loc is not None:
        talker(loc)
    cv2.waitKey(50)


def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('duck_eye', anonymous=True)

    rospy.Subscriber('/duckiebot18/camera_node/image/rect/compressed', CompressedImage, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':
    bridge = CvBridge()
    listener()
