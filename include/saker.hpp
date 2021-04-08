#ifndef SAKER_HPP
#define SAKER_HPP

#include "rang.hpp"
#include "environment.hpp"

#include <vector>
#include <variant>
#include <string>
#include <ostream>
#include <numeric>
#include <algorithm>

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
            // helpers
            unsigned int order{};
            unsigned int priority{};
            Reset style_reseter{};
            mutable std::string carried_end{};
            mutable BgColor carried_bg{};
            // content-related fields
            FgColor zone_fg_color{};
            BgColor zone_bg_color{};
            Style zone_style{};
            std::string content{};
            std::string end{};
            
            Zone_() = default;
            
            friend class Zone;
        
        public:
            
            Zone_& setIfNotStyle(Style zone_style) {
                if (static_cast<int>(zone_style) == 0) {
                    this->zone_style = zone_style;
                    this->style_reseter = getReseterForStyle(zone_style);
                }
                return *this;
            }
            
            Zone_& setIfNotBg(BgColor bg_color) {
                const auto intReprOfBg = [](BgColor bg) -> int {
                    if (bg.index() == 1) {
                        return static_cast<int>(std::get<1>(bg));
                    }
                    return static_cast<int>(std::get<0>(bg));
                };
                if (intReprOfBg(this->zone_bg_color) == 0) {
                    this->zone_bg_color = bg_color;
                }
                return *this;
            }
            
            Zone_& setIfNotFg(FgColor fg_color) {
                const auto intReprOfFg = [](FgColor fg) -> int {
                    if (fg.index() == 1) {
                        return static_cast<int>(std::get<1>(fg));
                    }
                    return static_cast<int>(std::get<0>(fg));
                };
                if (intReprOfFg(this->zone_fg_color) == 0) {
                    this->zone_fg_color = fg_color;
                }
                return *this;
            }
            
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
            
            Zone_& preRenderContent() {
                return *this;
            }
            
            unsigned int size() const {
                return this->content.size() + this->end.size();
            }
            
            unsigned int getPriority() const {
                return this->priority;
            }
            
            unsigned int getOrder() const {
                return this->order;
            }
        
    };
    
    class Zone {
        private:
            inline static unsigned int next_index{};
            Zone_ inner;
        public:
            Zone(const std::string& content) {
                this->inner.order = next_index++;
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
                this->inner.setIfNotStyle(zone_style);
                return *this;
            }
            
            Zone& endWith(const std::string& end) {
                this->inner.end = end;
                return *this;
            }
            
            Zone& priority(unsigned int priority) {
                this->inner.priority = priority;
                return *this;
            }
        
    };
    
    class Prompt_ {
        private:
            unsigned int max_size{100};
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
            
            void renderZones() {
                const auto[cols, _] = env::getTermSize();
                const unsigned int max_usable_cols = (cols * this->inner.max_size / 100);
                std::stable_sort( // sort by priority
                    this->inner.zones.begin(),
                    this->inner.zones.end(),
                    [](const Zone_& z1, const Zone_& z2) -> bool {
                        return z2.getPriority() < z1.getPriority();
                    }
                );
                unsigned int current_size{};
                this->inner.zones.erase( // erase things that wouldn't fit
                    std::remove_if(
                        this->inner.zones.begin(),
                        this->inner.zones.end(),
                        [&current_size, &max_usable_cols](const Zone_& z) {
                            current_size += z.size();
                            return current_size > max_usable_cols;
                        }
                    ),
                    this->inner.zones.end()
                );
                std::stable_sort( // re-sort by natural order
                    this->inner.zones.begin(),
                    this->inner.zones.end(),
                    [](const Zone_& z1, const Zone_& z2) -> bool {
                        return z1.getOrder() < z2.getOrder();
                    }
                );
            }
        
        public:
            explicit Prompt(std::initializer_list<Zone_> zones) {
                this->inner.zones = std::vector<Zone_>(zones);
            }
            
            Prompt& fg(FgColor global_fg_color) {
                for (auto& zone : this->inner.zones) {
                    zone.setIfNotFg(global_fg_color);
                }
                return *this;
            }
            
            Prompt& bg(BgColor global_bg_color) {
                for (auto& zone : this->inner.zones) {
                    zone.setIfNotBg(global_bg_color);
                }
                return *this;
            }
            
            Prompt& style(Style global_style) {
                for (auto& zone : this->inner.zones) {
                    zone.setIfNotStyle(global_style);
                }
                return *this;
            }
            
            Prompt& endWith(const std::string& final_chars) {
                this->inner.end = final_chars;
                return *this;
            }
            
            Prompt& maxSize(unsigned int max_percentage) {
                if (max_percentage <= 100) {
                    this->inner.max_size = max_percentage;
                }
                return *this;
            }
            
            Prompt_& show() {
                this->renderZones();
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
