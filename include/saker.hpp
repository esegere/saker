#ifndef SAKER_HPP
#define SAKER_HPP

//available for all sub modules
#include "rang.hpp"
#include "environment.hpp"
// submodules import
#include "saker/icon.hpp"
#include "saker/content.hpp"
#include "saker/zone.hpp"
#include "saker/prompt.hpp"

namespace saker {
    
    namespace transforming {
        
        template<typename T>
        auto drop_icon(const std::string& icon_str, const T& content_str) -> decltype(std::pair<std::string, T>()) {
            return {"", content_str};
        }
        
        template<typename T>
        auto drop_content(const std::string& icon_str, const T& content_str) -> decltype(std::pair<std::string, T>()) {
            const T default_empty_content{};
            return {icon_str, default_empty_content};
        }
        
        
        std::pair<std::string, std::string>
        drop_first_str(const std::string& icon_str, const std::string& content_str) {
            if (content_str.size() < 2) {
                return {icon_str, ""};
            }
            return {icon_str, content_str.substr(1, content_str.size() - 1)};
        }
        
        std::pair<std::string, std::vector<std::string>>
        drop_first_vec(const std::string& icon_str, const std::vector<std::string>& content_vec) {
            if (content_vec.size() < 2) {
                return {icon_str, {}};
            }
            std::vector<std::string> new_vec = content_vec;
            new_vec.erase(new_vec.cbegin());
            return {icon_str, new_vec};
        }
        
        std::pair<std::string, std::string> drop_last_str(const std::string& icon_str, const std::string& content_str) {
            if (content_str.size() < 2) {
                return {icon_str, ""};
            }
            return {icon_str, content_str.substr(0, content_str.size() - 1)};
        }
        
        std::pair<std::string, std::vector<std::string>>
        drop_last_vec(const std::string& icon_str, const std::vector<std::string>& content_vec) {
            if (content_vec.size() < 2) {
                return {icon_str, {}};
            }
            std::vector<std::string> new_vec = content_vec;
            new_vec.pop_back();
            return {icon_str, new_vec};
        }
        
    }
    
}

#endif //SAKER_HPP
