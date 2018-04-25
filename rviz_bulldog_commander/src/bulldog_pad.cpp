#include "bulldog_pad.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QDebug>

#include <sstream>

namespace rviz_bulldog_commander
{

// 构造函数，初始化变量
BulldogPanel::BulldogPanel( QWidget* parent )
  : rviz::Panel( parent )
  , state( STATE_WAIT )
{
  tab = new QTabWidget();
  // Tab 1
  QWidget *tab_1 = new QWidget();
  button_auto = new QPushButton("Execute");
  QHBoxLayout* layout_1 = new QHBoxLayout;
  layout_1->addWidget(button_auto);
  tab_1->setLayout(layout_1);

  // Tab 2
  QWidget *tab_2 = new QWidget();
  // Button
  button_detect = new QPushButton("Detect");
  button_move = new QPushButton("Move");
  button_pick = new QPushButton("Pick");
  // Button Layout
  QVBoxLayout* button_layout = new QVBoxLayout;
  button_layout->addWidget(button_detect);
  button_layout->addWidget(button_move);
  button_layout->addWidget(button_pick);
  // Display Label
  label_display = new QLabel();
  // Layout
  QHBoxLayout* layout_2 = new QHBoxLayout;
  layout_2->addLayout(button_layout);
  layout_2->addWidget(label_display);
  tab_2 -> setLayout(layout_2);

  // Tab 3
  QWidget *tab_3 = new QWidget();

  // Add Tab Pages
  tab->addTab(tab_1, "Auto");
  tab->addTab(tab_2, "Semi-Auto");
  tab->addTab(tab_3, "Manual");

  // Add Tab
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(tab);
  setLayout(layout);

  // connect signal and slots, sender, signal, receiver, slots
  connect(button_auto, SIGNAL(clicked()),
          this, SLOT(button_auto_click()));
  connect(button_detect, SIGNAL(clicked()),
          this, SLOT(button_detect_click()));
  connect(button_move, SIGNAL(clicked()),
          this, SLOT(button_move_click()));
  connect(button_pick, SIGNAL(clicked()),
          this, SLOT(button_pick_click()));

  // ROS
  pub = n.advertise<std_msgs::String>("plugin_command", 1);
  sub = n.subscribe("plugin_return", 1, &BulldogPanel::callback, this);
}

void BulldogPanel::callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO("%s", msg->data.c_str());
    std::string rec = msg->data;
    display = QString::fromStdString(rec);
    label_display->setText(display);
    if(rec == "Detect succeed!")  state = STATE_DETECTED;
    else if(rec == "Navagation succeed");
    else if(rec == "Execute succeed!")  state = STATE_PICKED;
}

void BulldogPanel::button_auto_click(){
  // if(state == STATE_WAIT)
  // {
  //   //robot->main();
  // }
  if(ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "AUTO";
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    pub.publish(msg);
  }
}

void BulldogPanel::button_detect_click(){
  // if(state == STATE_WAIT)
  // {
  //   QString str;
  //   if(robot->detect(x,y,z)==true)
  //   {
  //     str = QString("x:%1\ny:%2\nz:%3\n").arg(x).arg(y).arg(z);
  //     state = STATE_DETECTED;
  //   }
  //   else
  //   {
  //     str = QString("Detected Failed!");
  //     state = WAIT;
  //   }
  //   label_display->setText(str);
  // }
  if(ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "DETECT";
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    pub.publish(msg);

    ros::spinOnce();
  }
}

void BulldogPanel::button_move_click(){
  if(ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "NAVIGATION";
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    pub.publish(msg);
  }
}

void BulldogPanel::button_pick_click(){
  // if(state == STATE_DETECTED)
  // {
  //   QString str;
  //   if(robot->execute()==true)
  //   {
  //     str = QString("Pick Succeed!");
  //     state = PICKED;
  //   }
  //   else
  //   {
  //     str = QString("Pick Failed!");
  //     state = DETECTED;
  //   }
  //   label_display->setText(str);
  // }
  if(ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "EXECUTE";
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    pub.publish(msg);
  }
}

} // end namespace rviz_bulldog_commander

// 声明此类是一个rviz的插件
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(rviz_bulldog_commander::BulldogPanel,rviz::Panel)
// END_TUTORIAL
