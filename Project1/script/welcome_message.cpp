#include <gazebo/gazebo.hh>
#include <gazebo/physics/World.hh>
#include <iostream>

namespace gazebo
{
  class WelcomeMessage : public WorldPlugin
  {
    void Load(physics::WorldPtr /*_world*/, sdf::ElementPtr /*_sdf*/) override
    {
      std::cout << "\n Welcome to Aung Khant Ko's World!\n" << std::endl;
    }
  };
  GZ_REGISTER_WORLD_PLUGIN(WelcomeMessage)
}

