#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"   // service definition

// Publisher that sends velocity commands to the robot
ros::Publisher motor_command_publisher;

/* ------------- service callback ---------------------------------
 * ----------------------------------------------------------------*/
bool handle_drive_request(ball_chaser::DriveToTarget::Request  &req,
                          ball_chaser::DriveToTarget::Response &res)
{
    ROS_INFO_STREAM("DriveToTarget request received – lin_x: "
                    << req.linear_x  << " | ang_z: " << req.angular_z);

    // build the Twist message
    geometry_msgs::Twist motor_command;
    motor_command.linear.x  = req.linear_x;
    motor_command.angular.z = req.angular_z;
    motor_command_publisher.publish(motor_command);

    // populate feedback and log it
    res.msg_feedback = "Wheel velocities set – lin_x: "
                       + std::to_string(req.linear_x) +
                       " , ang_z: " + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    /* ---- node setup ---- */
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle nh;

    // publisher: velocity commands -> robot differential-drive plugin
    motor_command_publisher =
        nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // service server: /ball_chaser/command_robot
    ros::ServiceServer service =
        nh.advertiseService("/ball_chaser/command_robot",
                            handle_drive_request);

    ROS_INFO("drive_bot ready for /ball_chaser/command_robot requests");
    ros::spin();         
    return 0;
}

