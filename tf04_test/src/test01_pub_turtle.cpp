#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "tf2_ros/transform_broadcaster.h" //发布动态坐标关系
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"
/*
    发布方：需要订阅乌龟的位姿信息，转换为相对于窗体的坐标关系，并发布
    准备：
        话题：/turtle1/pose
        消息：/turtlesim/Pose

    实现流程：
        1.包含头文件
        2.设置编码、节点初始化、NodleHandle
        3.创建订阅对象，订阅/turtle/pose
        4.回调函数处理订阅的消息：将位姿信息转化为坐标相对关系并发布（***）
        5.spin()


 */

// 声明变量来接受传入的参数
std::string turtle_name;

void doPose(const turtlesim::Pose::ConstPtr &pose)
{
    // 获取位姿信息，转换成坐标系相对关系（核心），并发布
    // a.创建发布对象
    static tf2_ros::TransformBroadcaster pub; // 使用静态的变量，防止重复调用

    // b.组织被发布的数据；
    geometry_msgs::TransformStamped ts;
    // 填充数据
    ts.header.frame_id = "world";
    ts.header.stamp = ros::Time::now();
    ts.child_frame_id = turtle_name;
    // 关键点：动态传入
    // 坐标系偏移量
    ts.transform.translation.x = pose->x;
    ts.transform.translation.y = pose->y;
    ts.transform.translation.z = 0;
    // 坐标系四元数
    /*
    位姿信息中没有四元数，但有偏航角，又已知乌龟为2D，又已知没有翻滚与俯仰角，所以可以得出乌龟的欧拉角为：0，0，theta

     */
    tf2::Quaternion qtn;
    // 向该对象设置欧拉角，这个对象可以将欧拉角转为四元数
    qtn.setRPY(0, 0, pose->theta);

    ts.transform.rotation.x = qtn.getX();
    ts.transform.rotation.y = qtn.getY();
    ts.transform.rotation.z = qtn.getZ();
    ts.transform.rotation.w = qtn.getW();

    // c.发布
    pub.sendTransform(ts);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "dynamic_pub");
    /*
        解析launch文件通过 args传入参数

     */
    if (argc != 2)
    {
        ROS_ERROR("请传入一个参数");
    }
    else
    {
        turtle_name = argv[1];
        ROS_INFO("乌龟 %s 坐标发送启动", turtle_name.c_str());
    }
    // 2.设置编码、节点初始化、NodleHandle

        ros::NodeHandle nh;

    // 3.创建订阅对象，订阅/turtle/pose
    // 关键点：1.订阅的话题名称、turtle1或turtle2为动态传入
    ros::Subscriber sub = nh.subscribe<turtlesim::Pose>(turtle_name + "/pose", 100, doPose);
    // 4.回调函数处理订阅的消息：将位姿信息转化为坐标相对关系并发布（***）
    // 5.spin()
    ros::spin();

    /* code */
    return 0;
}
