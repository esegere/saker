#ifndef PROMPT_PROMPT_HPP
#define PROMPT_PROMPT_HPP

#include <vector>
#include <variant>
#include <string>
#include <ostream>
#include <numeric>
#include <algorithm>
#include "zone.hpp"

namespace saker {
    
    using namespace rang;
    
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
                std::vector<Zone_> valid_zones;
                std::copy_if(
                    zones.begin(),
                    zones.end(),
                    std::back_inserter(valid_zones),
                    [](Zone_ zone) {
                        return zone.isToBeShown();
                    }
                );
                this->inner.zones = valid_zones;
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

#endif //PROMPT_PROMPT_HPP
