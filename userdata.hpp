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
#include "saker.hpp"

namespace userdata {
    
    
    std::pair<std::string, std::vector<std::string>> get_directory_icon_and_parts() {
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
        if (!result_string_parts.empty()) {
            result_string_parts.back().append(" ");
        } else {
            selected_icon.append(" ");
        }
        return {selected_icon, result_string_parts};
    }
    
    std::tuple<std::string, std::string, saker::BgColor> get_user_icon_name_and_bg() {
        std::string user = getenv("USER");
        std::string user_icon;
        saker::BgColor bg = saker::Bg::cyan;
        if (user == "esege") {
            user_icon = " \uF007 ";
        } else if (user == "root") {
            user_icon = " \uF21B ";
            bg = saker::Bg::blue;
        } else {
            user_icon = " \uF0C0 ";
            bg = saker::BgB::gray;
        }
        return {user_icon, user, bg};
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
