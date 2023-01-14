#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/PointStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h" //注意: 调用 transform 必须包含该头文件
/* 
    订约方：订阅发布的坐标相对关系，传入一个坐标点，调用tf实现转换

    流程：
        1.包含头文件
        2.编码、初始化、NodeHandle（必要的）；
        3.创建订阅对象；---》订阅坐标系相对关系
        4.组织一个坐标点数据
        5.转换算法实现，需要调用tf内置实现
        6.输出转换结果


 */

int main(int argc, char *argv[])
{
    /* code */
        // 2.编码、初始化、NodeHandle（必要的）；
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "static_sub");
    ros::NodeHandle nh;

    // 3.创建订阅对象；---》订阅坐标系相对关系
    // 3-1 创建buffer缓存
    tf2_ros::Buffer buffer;

    // 3-2 创建订阅对象(监听对象可以将订阅的数据存入buffer)
    tf2_ros::TransformListener lisnter(buffer);

    ros::Rate r(1);
    while (ros::ok())
    {
        // 4.生成一个坐标点(相对于子级坐标系)
        geometry_msgs::PointStamped point_laser;
        point_laser.header.frame_id = "laser";
        point_laser.header.stamp = ros::Time::now();
        point_laser.point.x = 1;
        point_laser.point.y = 2;
        point_laser.point.z = 7.3;
        // 5.转换坐标点(相对于父级坐标系)
        // 新建一个坐标点，用于接收转换结果
        //--------------使用 try 语句或休眠，否则可能由于缓存接收延迟而导致坐标转换失败------------------------
        try
        {
            geometry_msgs::PointStamped point_base;
            point_base = buffer.transform(point_laser, "base_link");
            ROS_INFO("转换后的数据:(%.2f,%.2f,%.2f),参考的坐标系是:%s", point_base.point.x, point_base.point.y, point_base.point.z, point_base.header.frame_id.c_str());
        }
        catch (const std::exception &e)
        {
            // std::cerr << e.what() << '\n';
            ROS_INFO("程序异常.....");
        }

        r.sleep();
        ros::spinOnce();
    }
        return 0;
    }
