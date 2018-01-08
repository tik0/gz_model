#include <stdio.h>
#include <signal.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>    // copy
#include <boost/tokenizer.hpp>
#include <iterator>     // ostream_operator
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>
#include <sdf/sdf.hh>
#include <string>
#include <list>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include "gazebo/transport/transport.hh"
#include "gazebo/common/common.hh"
#include "gazebo/msgs/msgs.hh"

namespace po = boost::program_options;

using namespace gazebo;


/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  std::string modelName = "", inputfile = "";
  math::Pose pose;
  math::Vector3 rpy;
  int sleep_us = 1000000;

  // register them and add descriptions
  po::options_description options("Allowed options");
  options.add_options()("help,h", "Display a help message.")
          ("inputfile",       po::value<std::string>(&inputfile)->required(), "CSV file with (x,y,Yaw) tuple")
          ("model",           po::value<std::string>(&modelName)->required(), "Model name")
          ("z",               po::value<double>(&(pose.pos.x))->required(), "z-Value (m)")
          ("R",               po::value<double>(&(rpy.x))->required(), "Roll-Value (rad)")
          ("P",               po::value<double>(&(rpy.y))->required(), "Pitch-Value (rad)")
          ("delay",           po::value<int>(&sleep_us)->default_value(sleep_us), "Delay betwenn two messages (µs)");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(options).style(po::command_line_style::unix_style ^ po::command_line_style::allow_short).run(), vm);

  // first, process the help option
  if (vm.count("help")) {
      std::cout << options << std::endl;
      exit(EXIT_FAILURE);
  }

  // afterwards, let program options handle argument errors
  po::notify(vm);

  // Handle the file
  std::ifstream in(inputfile.c_str());
  if (!in.is_open()) {
      std::cerr << "File not valid" << std::endl;
      return 1;
  }

  // Open connetion to gazebo
  std::cout << "Init connection ..." << std::endl;
  gazebo::transport::init();
  std::cout << "Run ..." << std::endl;
  gazebo::transport::run();
  std::cout << "Init node ..." << std::endl;
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init("default");
  transport::PublisherPtr pub =
    node->Advertise<msgs::Model>("~/model/modify");
  std::cout << "Wait for connection ..." << std::endl;
  pub->WaitForConnection();
  std::cout << "... established" << std::endl;

  // Iterate through the CSV
  typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
  std::vector< std::string > vec;
  std::string line;

  while (getline(in,line))
  {
      Tokenizer tok(line);
      vec.assign(tok.begin(),tok.end());

      if (vec.size() != 3) {
          std::cerr << "Something wrong with csv file?" << std::endl;
          continue;
      } else {
         pose.pos.x = std::stod(vec.at(0));
         pose.pos.y = std::stod(vec.at(1));
         rpy.z = std::stod(vec.at(2));
         pose.rot.SetFromEuler(rpy);
         std::cout << "(x,y,Y): (" << pose.pos.x << ", " << pose.pos.y << ", " << rpy.z << ")\n";
         msgs::Model msg;
         msg.set_name(modelName);
         msgs::Set(msg.mutable_pose(), pose.Ign());
         pub->Publish(msg, true);
         usleep(sleep_us);
      }
  }
  pub->Fini();
}
