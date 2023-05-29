#include<ros/ros.h>
#include <tf/transform_broadcaster.h>
#include<nav_msgs/Odometry.h>

class transform_publisher
{
private:
    ros::NodeHandle nh;
public:
    transform_publisher()
    {
        nh = ros::NodeHandle("~");
        ros::Subscriber tf_pub = nh.subscribe<nav_msgs::Odometry>("/ackermann_streering_controller/odom", 1000, &transform_publisher::getPose,this);

    };
    ~transform_publisher()
    {
        ROS_INFO("World is shutted down!");
    };
    void getPose(const nav_msgs::OdometryConstPtr& pose);

};

void transform_publisher::getPose(const nav_msgs::OdometryConstPtr &pose)
{
    //创建tf的广播器
    static tf::TransformBroadcaster br;

    //初始化tf数据
    tf::Transform transform;
    transform.setOrigin( tf::Vector3(pose->pose.pose.position.x, pose->pose.pose.position.y, pose->pose.pose.position.z) );
    tf::Quaternion q(
        pose->pose.pose.orientation.x,
        pose->pose.pose.orientation.y,
        pose->pose.pose.orientation.z,
        pose->pose.pose.orientation.w
    );
    double roll,pitch,yaw;
    tf::Matrix3x3(q).getRPY(roll, pitch, yaw);
    q.setRPY(roll,pitch,yaw);
    transform.setRotation(q);

    //广播world与hunter_base坐标系之间的tf数据
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(),  "ackermann_steering_controller", "world"));
}   


int main(int argc, char **argv)
{
    ros::init ( argc, argv, "transformer_node");
    transform_publisher tf_p;
    ros::spin();
    return 0;
}