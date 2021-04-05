#ifndef SAKER_HPP
#define SAKER_HPP

#include "rang.hpp"
#include <vector>
#include <string_view>
#include <string>
#include <variant>
#include <ostream>

namespace saker {
    using namespace rang;
    using FgColor = std::variant<Fg, FgB>;
    using BgColor = std::variant<Bg, BgB>;
    
    class Zone_ {
        private:
            std::string repr{};
            std::string bg{};
            std::string end{};
            
            Zone_() = default;
            
            friend class Zone;
        
        public:
            
            friend std::ostream& operator <<(std::ostream& os, const Zone_& zone);
    };
    
    class Zone {
        private:
            Zone_ inner;
            std::string content;
        public:
            Zone(std::string_view sv) {
                this->content = std::string(sv);
            }
            
            operator Zone_&() {
                return this->inner;
            }
            
            Zone& fg(FgColor zone_fg_color) {
                return *this;
            }
            
            Zone& bg(BgColor zone_bg_color) {
                return *this;
            }
            
            Zone& style(Style zone_style) {
                return *this;
            }
    };
    
    class Prompt_ {
        private:
            FgColor global_fg_color{};
            BgColor global_bg_color{};
            Style global_style{};
            std::vector<Zone_> zones{};
            std::string end{};
            
            Prompt_() = default;
            
            friend class Prompt;
        
        public:
            
            friend std::ostream& operator <<(std::ostream&, const Prompt_&);
    };
    
    
    class Prompt {
        private:
            Prompt_ inner;
        public:
            explicit Prompt(std::initializer_list<Zone_> zones) {
                this->inner.zones = std::vector<Zone_>();
                for (auto& zone : zones) {
                    this->inner.zones.emplace_back(zone);
                }
            }
            
            Prompt& fg(FgColor global_fg_color) {
                this->inner.global_fg_color = global_fg_color;
                return *this;
            }
            
            Prompt& bg(BgColor global_bg_color) {
                this->inner.global_bg_color = global_bg_color;
                return *this;
            }
            
            Prompt& style(Style global_style) {
                this->inner.global_style = global_style;
                return *this;
            }
            
            Prompt& endWith(std::string_view final_chars) {
                return *this;
            }
            
            Prompt_& show() {
                return this->inner;
            }
    };
    
    std::ostream& operator <<(std::ostream& os, const Zone_& zone) {
        return os;
    }
    
    std::ostream& operator <<(std::ostream& os, const Prompt_& prompt) {
//        for (auto& zone : prompt.zones) {
//            os
//                    << prompt.global_style
//                    << prompt.global_bg_color
//                    << prompt.global_fg_color
//                    << zone;
//        }
        return os <<
                  Style::reset <<
                  Fg::reset <<
                  Bg::reset <<
                  prompt.end;
    }
}

#endif //SAKER_HPP
