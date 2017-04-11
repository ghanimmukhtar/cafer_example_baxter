
#include <iostream>

#include "cafer_core/cafer_core.hpp"
#include <sensor_msgs/Image.h>
#include <thread>
#include <chrono>

#include "../include/globals.h"

using namespace cafer_core;

class Wave_1 : public Supervisor {

public :
    using Supervisor::Supervisor;

    void database_manager_cb(const DBManagerConstPtr& status_msg) override{
        if (status_msg->id == this->get_id()) {
            if (static_cast<DatabaseManager::Response>(status_msg->type) ==
                DatabaseManager::Response::STATUS_READY) {
                db_done_recording = true;
            }
        }
    }

    void update() override{
        DBManager db_request;
        db_request.type = static_cast<uint8_t>(DatabaseManager::Request::ASK_STATUS);
        db_request.id = this->get_id();

        if(!db_done_recording){
            _pub_db_manager->publish(db_request);
            ROS_WARN("Waiting for DBManager to be ready");
        }
        else{
            db_request.type=static_cast<uint8_t>(DatabaseManager::Request::RECORD_DATA);
            _pub_db_manager->publish(db_request);
            db_done_recording = false;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            db_request.type = static_cast<u_int8_t>(DatabaseManager::Request::STOP_RECORDING);
            _pub_db_manager->publish(db_request);
        }
    }

    void client_disconnect_from_ros() override{}

    ~Wave_1()
    {
        ROS_ERROR("Destroy?");
        client_disconnect_from_ros();
    }
};



int main(int argc, char** argv)
{
    std::string node_name;
    XmlRpc::XmlRpcValue cafer;

    node_name = tools::parse_arg(argc, argv, "wave_1_node");
    std::cout << "my node name is: " << node_name << std::endl;

    cafer_core::init(argc, argv, node_name);
    cafer_core::ros_nh->getParam("cafer", cafer);
    Wave_1 wave_1(cafer["mgmt"], cafer["type"], cafer["freq"], cafer["uuid"]);

    wave_1.wait_for_init();

    while (ros::ok() && (!wave_1.get_terminate())) {
        wave_1.spin();
        wave_1.update();
        wave_1.sleep();
    }

    return 0;
}
