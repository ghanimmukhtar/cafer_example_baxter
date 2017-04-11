//
// Created by phlf on 06/07/16.
//

#ifndef DREAM_BABBLING_BABBLING_CONSTANTS_HPP
#define DREAM_BABBLING_BABBLING_CONSTANTS_HPP

#include <string>

namespace tools {
    std::string parse_arg(int& argc, char**& argv, const std::string& default_val)
    {
        std::string key;
        std::string value;
        std::string temp_str;
        std::string::size_type res;

        key = "__name:=";

        for (unsigned short i = 0; i < argc; ++i) {
            temp_str = argv[i];
            res = temp_str.find(key);

            if (res != std::string::npos) {
                value = temp_str.erase(res, key.length());
                break;
            }
            else if (i == argc - 1) {
                value = default_val;
            }
        }
        return value;
    }
}
#endif // DREAM_BABBLING_BABBLING_CONSTANTS_HPP
