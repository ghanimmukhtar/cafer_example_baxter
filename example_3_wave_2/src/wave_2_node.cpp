#include <iostream>
#include <fstream>
#include <dirent.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>

#include "cafer_core/cafer_core.hpp"
#include <sensor_msgs/JointState.h>
#include <thread>
#include <chrono>

#include "../include/globals.h"

using namespace cafer_core;
namespace fs = boost::filesystem;

class Wave_2 : public Supervisor {

public :
    using Supervisor::Supervisor;

    void database_manager_cb(const DBManagerConstPtr& status_msg) override{
        if (status_msg->id == this->get_id()) {
            ROS_INFO_STREAM(status_msg->type);
            switch (static_cast<DatabaseManager::Response>(status_msg->type))
            {
               case DatabaseManager::Response::STATUS_READY :  {db_done_recording = true;
            ROS_INFO("got the ready message");}
                break;
               case DatabaseManager::Response::DATA : {
                DIR *directory;
                struct dirent *ent;
                std::string my_path = "/home/ghanim/ros_home/cafer_db"+status_msg->data[0];
                std::string file_name, new_path;
                if ((directory = opendir(my_path.c_str())) != NULL){
                //fs::directory_iterator out_end_iter;
                //for (fs::directory_iterator out_dir_itr(my_path); out_dir_itr != out_end_iter; ++out_dir_itr)
                    /* print all the files and directories within directory */
                    while ((ent = readdir (directory)) != NULL) {
                        new_path = my_path + "/" + ent->d_name;
                        std::string my_ent = ent->d_name;
                        if(my_ent != "." && my_ent != ".."){
                        fs::path full_path(new_path);
                        if(fs::is_directory(full_path)){
                            std::cout << "\nIn directory: " << full_path.string() << "\n";
                            fs::directory_iterator end_iter;
                            for (fs::directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr)
                                if (fs::is_regular_file(dir_itr->status())){
                         
                                    fs::path my_file_path = dir_itr->path().filename();
                                    file_name = full_path.string() + "/" + my_file_path.c_str();
                                    //std::cout << file_name << "\n";
                                    std::ifstream ff(file_name);
                                    std::string my_line;
                                    int line_count = 0;
                                    while(std::getline(ff, my_line)){
                                        ++line_count;
                                    }
                                    std::cout << "number of lines is: " << line_count << std::endl;
                                 }
                              }
                           }
                      }
                    closedir (directory);
                }
                else {
                    ROS_INFO("It is empty");
                    ROS_INFO_STREAM(status_msg->data[0]);
                    ROS_INFO_STREAM(my_path);
                }
            }
                break;
               default : ROS_INFO("I don't know this message");
            }
            std::cout << "\n************************************************" << std::endl;
        }
    }

    void update() override{
        DBManager db_request;
        db_request.id = this->get_id();
        db_request.type=static_cast<uint8_t>(DatabaseManager::Request::REQUEST_DATA);
        db_request.data.emplace_back("numbers_storage");
        _pub_db_manager->publish(db_request);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    void client_disconnect_from_ros() override{}

    ~Wave_2()
    {
        ROS_ERROR("Destroy?");
        client_disconnect_from_ros();
    }
};



int main(int argc, char** argv)
{
    std::string node_name;
    XmlRpc::XmlRpcValue cafer;

    node_name = tools::parse_arg(argc, argv, "wave_2_node");
    std::cout << "my node name is: " << node_name << std::endl;
    std::cout << "my argc is: " << argc << std::endl;
    std::cout << "my argv is: " << argv[2] << std::endl;

    cafer_core::init(argc, argv, node_name);
    cafer_core::ros_nh->getParam("cafer", cafer);
    Wave_2 wave_2(cafer["mgmt"], cafer["type"], cafer["freq"], cafer["uuid"]);

    wave_2.wait_for_init();

    while (ros::ok() && (!wave_2.get_terminate())) {
        wave_2.spin();
        wave_2.update();
        wave_2.sleep();
    }

    return 0;
}
