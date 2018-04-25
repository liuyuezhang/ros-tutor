#ifndef BULLDOG_PAD_H
#define BULLDOG_PAD_H

//所需要包含的头文件
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <ros/console.h>
#include <rviz/panel.h>   //plugin基类的头文件

#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QString>

#include <string>

namespace rviz_bulldog_commander
{
#define STATE_WAIT 0
#define STATE_DETECTED 1
#define STATE_PICKED 2

// 所有的plugin都必须是rviz::Panel的子类
class BulldogPanel: public rviz::Panel
{
// 后边需要用到Qt的信号和槽，都是QObject的子类，所以需要声明Q_OBJECT宏
Q_OBJECT
public:
  BulldogPanel( QWidget* parent = 0 );

protected:
  QTabWidget *tab;

  //Tab 1
  QPushButton *button_auto;

  // Tab 2
  QPushButton *button_detect;
  QPushButton *button_move;
  QPushButton *button_pick;
  QLabel *label_display;
  QString display;

  // Tab 3

  // ROS
  ros::NodeHandle n;
  ros::Publisher pub;
  ros::Subscriber sub;

  void callback(const std_msgs::String::ConstPtr& msg);

  // Variables
  int state; //State Machine: 0-STATE_WAIT, 1-STATE_DETECTED, 2-STATE_PICKED

private Q_SLOTS:
  void button_auto_click();
  void button_detect_click();
  void button_move_click();
  void button_pick_click();
};

} // end namespace rviz_bulldog_commander

#endif // BULLDOG_PANEL_H
