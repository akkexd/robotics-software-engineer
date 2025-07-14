#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "ball_chaser/DriveToTarget.h"

ros::ServiceClient drive_client;          // /ball_chaser/command_robot
const uint8_t WHITE = 255;                // convenience


void command_robot(double vx, double wz)
{
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x  = vx;
    srv.request.angular_z = wz;

    if (!drive_client.call(srv))
        ROS_ERROR_STREAM("Failed to call /ball_chaser/command_robot");
}


void imageCb(const sensor_msgs::Image& img)
{
    const int row_step   = img.step;      // width * 3  (bytes/row)
    const int mid_col    = img.width / 2; // centre column index
    const double ANG_GAIN = 0.003;        

    bool ball_seen = false;
    int  ball_col  = 0;


    for (size_t i = 0; i < img.data.size(); i += 3)
    {
        if (   img.data[i]     == WHITE   // R
            && img.data[i + 1] == WHITE   // G
            && img.data[i + 2] == WHITE)  // B
        {
            ball_seen = true;
            ball_col  = (i / 3) % img.width;   
            break;
        }
    }

    if (ball_seen)
    {

        int offset_px = mid_col - ball_col;
        command_robot(0.5, offset_px * ANG_GAIN);
    }
    else
    {
        command_robot(0.0, 0.0);  // stop if no ball
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "process_image");
    ros::NodeHandle nh;

    drive_client = nh.serviceClient<ball_chaser::DriveToTarget>(
                     "/ball_chaser/command_robot");

    ros::Subscriber sub = nh.subscribe("/camera/rgb/image_raw",
                                       10, imageCb);

    ros::spin();
    return 0;
}

