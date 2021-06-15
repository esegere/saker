#ifndef PROMPT_PROMPT_HPP
#define PROMPT_PROMPT_HPP

#include <vector>
#include <variant>
#include <string>
#include <ostream>
#include <numeric>
#include <algorithm>

#include "content.hpp"
#include "zone.hpp"

#include "rang.hpp"
#include "environment.hpp"

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
        
            bool tryToReduceUntilFits(Zone_& zone_to_fit, int available_space) {
                int prev_size = zone_to_fit.size();
                int new_size;
                do {
                    zone_to_fit.transformWithFunc();
                    new_size = zone_to_fit.size();
                    if (prev_size == new_size) {
                        return false;
                    }
                    prev_size = new_size;
                } while (new_size > available_space);
                return true;
            }
        
            auto findLastFittingContent() -> decltype(this->inner.zones.begin()) {
                const auto[cols, _] = env::getTermSize();
                const unsigned int max_usable_cols = (cols * this->inner.max_size / 100);
                int used_cols = 0;
                for (auto zone_iter = this->inner.zones.begin(); zone_iter != this->inner.zones.end(); zone_iter++) {
                    if (int cols_to_use = used_cols + (*zone_iter).size(); max_usable_cols < cols_to_use) {
                        const bool did_reduce = tryToReduceUntilFits(*zone_iter, max_usable_cols - used_cols);
                        if (!did_reduce) {
                            return zone_iter;
                        }
                    }
                    used_cols += zone_iter->size();
                }
                return this->inner.zones.end();
            }
        
            void renderZones() {
                std::stable_sort( // sort by priority
                    this->inner.zones.begin(),
                    this->inner.zones.end(),
                    [](const Zone_& z1, const Zone_& z2) -> bool {
                        return z2.getPriority() < z1.getPriority();
                    }
                );
                this->inner.zones.erase(
                    this->findLastFittingContent(),
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
                valid_zones.reserve(zones.size()); // to avoid multiple allocations
                std::copy_if(
                    zones.begin(),
                    zones.end(),
                    std::back_inserter(valid_zones),
                    [](Zone_ zone) {
                        return zone.isToBeShown();
                    }
                );
                this->inner.zones = std::move(valid_zones);
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
        
            // TODO: add a default minimal prompt style for very minimal situations
        
            // TODO: add support for non-powerline style switching
        
            Prompt_& show() {
                setControlMode(rang::Control::Force); // rang
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
