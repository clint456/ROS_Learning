#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/PointStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"

/*
    需求1：换算出turtle1相对于turtle2的坐标关系
    需求2：计算角速度、线速度，并发布


 */

int main(int argc, char *argv[])
{
    // 2.编码、初始化、NodeHandle
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "tfs_sub");
    ros::NodeHandle nh;

    // 3.创建订阅对象
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener sub(buffer);

    // A.创建发布对象
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 100);
    //  4.编写解析逻辑

    ros::Rate rate(10);
    while (ros::ok())
    {
        // 核心
        try
        {
            // 1.计算son1与son2的相对关系
            /*
                参数1：目标坐标系
                参数2：源坐标系
                参数3：ros::Time(0)   取间隔最短的两个坐标关系帧
                返回值：geometry_msgs::TransformStamped   源坐标相对于目标坐标的相对关系
             */
            geometry_msgs::TransformStamped son1Toson2 =
                buffer.lookupTransform("turtle2", "turtle1", ros::Time(0));
            // ROS_INFO("turtle1相对于turtle2的信息，  父级：%s   子级：%s   偏移量：(%.2f,%.2f,%.2f)",
            //          son1Toson2.header.frame_id.c_str(), // turtle2
            //          son1Toson2.child_frame_id.c_str(),  // turtle1
            //          son1Toson2.transform.translation.x,
            //          son1Toson2.transform.translation.y,
            //          son1Toson2.transform.translation.z);
            // B.根据相对计算关系，计算速度消息
            geometry_msgs::Twist twist;
            // 组织速度，只需要设置线速度的X与角速度的z
            // x = 系数*开方(y^2+x^2)
            // z = 系数*反正切(对边，邻边)
            twist.linear.x = 0.5 * sqrt(pow(son1Toson2.transform.translation.x, 2) + pow(son1Toson2.transform.translation.y, 2));
            twist.angular.z = 4 * atan2(son1Toson2.transform.translation.y, son1Toson2.transform.translation.x);

            pub.publish(twist);
            // 发布消息
        }
        catch (const std::exception &e)
        {
            ROS_INFO("错误提示: %s", e.what());
        }

        rate.sleep();
        ros::spinOnce();
    }
    // 5.spinOnce()
    return 0;
}
