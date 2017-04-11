#include <cafer_core/cafer_core.hpp>
#include "../include/data/joint_states_data.hpp"

namespace cc = cafer_core;

int main(int argc, char** argv){
    cc::shared_ptr<cc::DatabaseManager::_Wave> wave_1;
    cc::shared_ptr<cc::DatabaseManager::_Wave> wave_2;

    std::unique_ptr<cc::ManagerQueue<JointData>> joint_states_manager{new cc::ManagerQueue<JointData>()};


    XmlRpc::XmlRpcValue cafer;

    cc::init(argc, argv, "db_manager_node");
    cc::ros_nh->getParam("cafer",cafer);
    cc::DatabaseManager db_manager(cafer["mgmt"], cafer["type"], cafer["freq"]);

    // Retrieve WATCHDOG messages from the management topic.
    db_manager.wait_for_init();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    db_manager.spin();

    db_manager.add_wave("/numbers_addition_example/my_inputs_store");
    db_manager.add_wave("/numbers_addition_example/my_numbers_retrieve");

    db_manager.find_wave_by_name("/numbers_addition_example/my_inputs_store", wave_1);

    if (wave_1 != nullptr) {
        wave_1->add_manager(joint_states_manager.release(), wave_1->data_topics["numbers"]);
    }

    while (ros::ok() && (!db_manager.get_terminate())) {
        db_manager.spin();
        db_manager.update();
        db_manager.sleep();
    }

    return EXIT_SUCCESS;
}
