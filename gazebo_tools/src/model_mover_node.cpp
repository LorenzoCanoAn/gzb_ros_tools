#include "ros/ros.h"
#include "std_msgs/String.h"
#include "gazebo_msgs/SetModelState.h"
#include <sstream>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <random>
#include <cstdlib>
const float LO = -20./180.*M_PI;
const float HI = 20./180.*M_PI;
float r_ang()
{
  float r = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
  return r;

}

void update_movemodel_request(gazebo_msgs::SetModelState &srv, float x, float y, float z, float roll, float pitch, float yaw)
{
  srv.request.model_state.pose.position.x = x;
  srv.request.model_state.pose.position.y = y;
  srv.request.model_state.pose.position.z = z;
  tf2::Quaternion my_quat;
  my_quat.setRPY(roll,pitch,yaw + M_PI/2.);
  tf2::convert(my_quat, srv.request.model_state.pose.orientation);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "model_mover");
  ros::NodeHandle n;
  ros::ServiceClient model_mover_client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
  ros::Publisher pub_1 = n.advertise<std_msgs::String>("/counter", 1);
  std_msgs::String my_str;
  my_str.data = "a";
  gazebo_msgs::SetModelState srv;
  srv.request.model_state.model_name = "quadrotor";
  srv.request.model_state.reference_frame = "";
  ros::Rate loop_rate(1);
  int x;
  while (ros::ok())
  {
    //std::cin.get();
    std::cout << x << std::endl;
    update_movemodel_request(srv, 0,10,1.5,r_ang(),r_ang(),r_ang());
    model_mover_client.call(srv);
    loop_rate.sleep();
  }

  return 0;
}
