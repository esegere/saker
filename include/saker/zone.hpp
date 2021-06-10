#ifndef PROMPT_ZONE_HPP
#define PROMPT_ZONE_HPP

#include <vector>
#include <variant>
#include <string>
#include <ostream>
#include <numeric>
#include <algorithm>

namespace saker {
    
    using namespace rang;
    
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
}

#endif //PROMPT_ZONE_HPP
