#include "ros/ros.h"
#include "rosbag/bag.h"
#include "std_msgs/String.h"

/*
    需求： 使用rosbag 向磁盘文件写出数据（话题 + 消息)
    流程：
        1.导包
        2.初始化 ：编码 ros节点 NodeHandle
        3.创建rosbag对象
        4.打开文件流
        5.写数据
        6.关闭文件流
 */
int main(int argc, char *argv[])
{
    // 2.初始化 ：编码 ros节点 NodeHandle
    ros::init(argc, argv, "rosbag_node");
    ros::NodeHandle nh;
    // 3.创建rosbag对象
    rosbag::Bag bag;
    // 4.打开文件流
    bag.open("hello.bag", rosbag::BagMode::Write);
    // 5.写数据
    std_msgs::String data;
    data.data = "hello world";
    bag.write("/chatter", ros::Time::now(), data); // 参数1：话题 参数2：时间戳 参数3：数据
    bag.write("/chatter", ros::Time::now(), data);
    bag.write("/chatter", ros::Time::now(), data);
    bag.write("/chatter", ros::Time::now(), data);
    // 6.关闭文件流
    bag.close();

    return 0;
}
