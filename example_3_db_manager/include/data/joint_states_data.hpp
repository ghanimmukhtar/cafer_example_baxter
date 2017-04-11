//
// Created by phlf on 22/07/16.
//
#include "cafer_core/cafer_core.hpp"
#include <sensor_msgs/JointState.h>
#include <yaml-cpp/yaml.h>

class JointData : public cafer_core::Data {
public:
    using cafer_core::Data::Data;

    virtual std::map<std::string, std::string> get_serialized_data() const override
    {
        std::string frame_id;
        std::vector<double> joints_positions;

        std::map<std::string, std::string> serialized_data;
        YAML::Emitter joints_position_yml;

        cafer_core::shared_ptr<sensor_msgs::JointState> msg;
        msg = _stored_msg.instantiate<sensor_msgs::JointState>();

        joints_positions = msg->position;
        //joints_position_yml << YAML::BeginSeq;
        joints_position_yml << YAML::Flow << joints_positions;
        //for(unsigned int i = 0; i < joints_positions.size(); ++i)
          //  joints_position_yml << joints_positions[i];
        joints_position_yml << YAML::Newline;
        //joints_position_yml << YAML::EndSeq;

        serialized_data["numbers"] = joints_position_yml.c_str();

        return serialized_data;
     }
};
