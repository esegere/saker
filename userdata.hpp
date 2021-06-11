#ifndef PROMPT_USERDATA_HPP
#define PROMPT_USERDATA_HPP

#include <vector>
#include <string>
#include <utility>
#include "icons.hpp"
#include <fplus/fplus.hpp>
#include <fplus/fwd.hpp>
#include <unistd.h>
#include <climits>

namespace userdata {
    
    
    std::vector<std::string> get_directory_parts() {
        // modify only this parameters
        
        constexpr const char* DEFAULT_ICON = "\uf023";
        constexpr unsigned int NUMBER_OF_RELEVANT_SUBDIRS = 2;
        const std::map<std::string, std::string> ICONS = icons::special_dirs();
        // creation
        
        const std::string path = get_current_dir_name();
        std::string selected_icon{DEFAULT_ICON};
        std::vector<std::string> result_string_parts =
            fplus::fwd::apply(
                path,
                fplus::fwd::split('/', false),
                fplus::fwd::reverse(),
                fplus::fwd::take_while(
                    [&ICONS, &selected_icon](auto elem) -> bool {
                        if (auto found = ICONS.find(elem); found != ICONS.end()) {
                            selected_icon = found->second;
                            return false;
                        }
                        return true;
                    }
                ),
                fplus::fwd::take(NUMBER_OF_RELEVANT_SUBDIRS),
                fplus::fwd::reverse()
            );
        selected_icon = " " + selected_icon;
        result_string_parts.insert(result_string_parts.cbegin(), selected_icon);
        result_string_parts.back().append(" ");
        return result_string_parts;
    }
    
    std::string get_user() {
        return getlogin();
    }
    
    std::pair<std::string, std::string> get_host_icon_and_name() {
        std::string hostname;
        char hn_char[HOST_NAME_MAX];
        gethostname(hn_char, HOST_NAME_MAX);
        hostname = hn_char;
        std::string hosticon{};
        if (hostname == "dellneon") {
            hosticon = " \uF31B ";
        } else if (hostname == "dellsuse") {
            hosticon = " \uF314 ";
        } else {
            hosticon = " \uE712 ";
        }
        return {hosticon, hostname};
    }
    
}

#endif //PROMPT_USERDATA_HPP
