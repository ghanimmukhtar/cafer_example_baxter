//
// Created by phlf on 22/07/16.
//
#include "cafer_core/cafer_core.hpp"
#include <sensor_msgs/Image.h>
#include <yaml-cpp/yaml.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class ImageData : public cafer_core::Data {
public:
    using cafer_core::Data::Data;

    virtual std::map<std::string, std::string> get_serialized_data() const override
    {
        std::string frame_id;
        cv::Mat rgb;
        std::vector<uchar> image_buffer;
        std::istringstream buffer_to_encode;

        std::map<std::string, std::string> serialized_data;
        YAML::Emitter rgb_yml;

        cafer_core::shared_ptr<sensor_msgs::Image> msg;
        msg = _stored_msg.instantiate<sensor_msgs::Image>();

        rgb = cv_bridge::toCvShare(*msg, nullptr, "bgr8")->image;

        //Encode depth as PNG and encode it as BASE64 string:
        cv::imencode(".png", rgb, image_buffer);
        buffer_to_encode = std::istringstream(std::string(image_buffer.begin(), image_buffer.end()));

        rgb_yml << YAML::Binary(
                         reinterpret_cast<const unsigned char*>(buffer_to_encode.str().c_str()),
                         buffer_to_encode.str().size()) << "\n";

        serialized_data["rgb"] = rgb_yml.c_str();

        return serialized_data;
    }
};
