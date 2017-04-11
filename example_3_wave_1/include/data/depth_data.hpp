//
// Created by phlf on 22/07/16.
//
#include "cafer_core/cafer_core.hpp"
#include <sensor_msgs/Image.h>
#include <yaml-cpp/yaml.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class DepthData : public cafer_core::Data {
public:
    using cafer_core::Data::Data;

    virtual std::map<std::string, std::string> get_serialized_data() const override
    {
        std::string frame_id;
        cv::Mat depth;
        std::vector<uchar> image_buffer;
        std::istringstream buffer_to_encode;

        std::map<std::string, std::string> serialized_data;
        YAML::Emitter depth_yml;

        cafer_core::shared_ptr<sensor_msgs::Image> msg;
        msg = _stored_msg.instantiate<sensor_msgs::Image>();

        depth = cv_bridge::toCvShare(*msg, nullptr)->image;
        depth = cv::Mat(depth.rows, depth.cols, CV_8UC4, depth.data);

        //Encode depth as PNG and encode it as BASE64 string:
        cv::imencode(".png", depth, image_buffer);
        buffer_to_encode = std::istringstream(std::string(image_buffer.begin(), image_buffer.end()));

        depth_yml << YAML::Binary(
                         reinterpret_cast<const unsigned char*>(buffer_to_encode.str().c_str()),
                         buffer_to_encode.str().size()) << "\n";

        serialized_data["depth"] = depth_yml.c_str();

        return serialized_data;
    }
};


/*frame_id = "frame_" + std::to_string(msg->header.stamp.sec + msg->header.stamp.nsec);

if (!msg->result.joints_positions.empty()) {

    joints_yml << YAML::BeginMap
                   << YAML::Key << frame_id << YAML::Value
                   << YAML::BeginMap
                       << YAML::Key << "timestamp" << YAML::Value
                       << YAML::BeginMap
                           << YAML::Key << "sec" << YAML::Value << msg->header.stamp.sec
                           << YAML::Key << "nsec" << YAML::Value << msg->header.stamp.nsec
                       << YAML::EndMap
                       << YAML::Key << "joints_values" << YAML::Value
                       << YAML::BeginMap;

    for (unsigned int i = 0; i < msg->result.joints_positions.size(); ++i) {
        joints_yml << YAML::Key << "joint_" + std::to_string(i) << YAML::Value
                   << msg->result.joints_positions[i];
    }
    joints_yml << YAML::EndMap << YAML::EndMap << YAML::EndMap << YAML::Newline;


}

                depth_yml << YAML::BeginMap
                              << YAML::Key << frame_id << YAML::Value
                              << YAML::BeginMap
                                  << YAML::Key << "timestamp" << YAML::Value
                                  << YAML::BeginMap
                                      << YAML::Key << "sec" << YAML::Value << msg->header.stamp.sec
                                      << YAML::Key << "nsec" << YAML::Value << msg->header.stamp.nsec
                                  << YAML::EndMap
                                  << YAML::Key << "depth" << YAML::Value << YAML::Binary(
                                reinterpret_cast<const unsigned char*>(buffer_to_encode.str().c_str()),
                                buffer_to_encode.str().size())
                              << YAML::EndMap
                          << YAML::EndMap << YAML::Newline;

*/
