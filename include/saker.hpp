#ifndef SAKER_HPP
#define SAKER_HPP

#include "rang.hpp"
#include <vector>
#include <variant>
#include <string>
#include <ostream>

namespace saker {
    using namespace rang;
    using FgColor = std::variant<Fg, FgB>;
    using BgColor = std::variant<Bg, BgB>;
    
    inline std::ostream& operator <<(std::ostream& os, const FgColor& fg_color) {
        if (fg_color.index() == 1) {
            return os << std::get<1>(fg_color);
        }
        return os << std::get<0>(fg_color);
    }
    
    inline std::ostream& operator <<(std::ostream& os, const BgColor& bg_color) {
        if (bg_color.index() == 1) {
            return os << std::get<1>(bg_color);
        }
        return os << std::get<0>(bg_color);
    }
    
    inline FgColor getFgForBg(BgColor bg) {
        if (bg.index() == 1) {
            return getFgForBg(std::get<1>(bg));
        }
        return getFgForBg(std::get<0>(bg));
    }
    
    class Zone_ {
        private:
            FgColor zone_fg_color{};
            BgColor zone_bg_color{};
            Style zone_style{};
            Reset style_reseter{};
            std::string content{};
            mutable std::string carried_end{};
            mutable BgColor carried_bg{};
            std::string end{};
            
            Zone_() = default;
            
            friend class Zone;
        
        public:
            
            friend std::ostream& operator <<(std::ostream& os, const Zone_& zone);
            
            const std::string& getEnd() const {
                return this->end;
            }
            
            BgColor getBg() const {
                return this->zone_bg_color;
            }
            
            const Zone_& carriedBg(BgColor bg) const {
                this->carried_bg = bg;
                return *this;
            }
            
            const Zone_& carriedEnd(const std::string& end) const {
                this->carried_end = end;
                return *this;
            }
    };
    
    class Zone {
        private:
            Zone_ inner;
        public:
            Zone(const std::string& content) {
                this->inner.content = content;
            }
            
            operator Zone_&() {
                return this->inner;
            }
            
            Zone& fg(FgColor zone_fg_color) {
                this->inner.zone_fg_color = zone_fg_color;
                return *this;
            }
            
            Zone& bg(BgColor zone_bg_color) {
                this->inner.zone_bg_color = zone_bg_color;
                return *this;
            }
            
            Zone& style(Style zone_style) {
                this->inner.zone_style = zone_style;
                this->inner.style_reseter = getReseterForStyle(zone_style);
                return *this;
            }
            
            Zone& endWith(const std::string& end) {
                this->inner.end = end;
                return *this;
            }
    };
    
    class Prompt_ {
        private:
            FgColor global_fg_color{Fg::default_};
            BgColor global_bg_color{Bg::default_};
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
                this->inner.zones = std::vector<Zone_>(zones);
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
            
            Prompt& endWith(const std::string& final_chars) {
                this->inner.end = final_chars;
                return *this;
            }
            
            Prompt_& show() {
                return this->inner;
            }
    };
    
    std::ostream& operator <<(std::ostream& os, const Zone_& zone) {
        return os <<
                  zone.zone_bg_color <<
                  getFgForBg(zone.carried_bg) <<
                  zone.carried_end <<
                  zone.zone_style <<
                  zone.zone_fg_color <<
                  zone.content <<
                  zone.style_reseter;
    }
    
    std::ostream& operator <<(std::ostream& os, const Prompt_& prompt) {
        std::string buffered_end{};
        BgColor buffered_bg{};
        for (auto& zone : prompt.zones) {
            os <<
               prompt.global_style <<
               prompt.global_bg_color <<
               prompt.global_fg_color <<
               zone
                   .carriedEnd(buffered_end)
                   .carriedBg(buffered_bg);
            
            buffered_end = zone.getEnd();
            buffered_bg = zone.getBg();
        }
        return os <<
                  getFgForBg(buffered_bg) <<
                  buffered_end <<
                  Reset::all <<
                  prompt.end;
    }
}

#endif //SAKER_HPP
