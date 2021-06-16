#ifndef PROMPT_ZONE_HPP
#define PROMPT_ZONE_HPP

#include <vector>
#include <variant>
#include <string>
#include <ostream>
#include <numeric>
#include <algorithm>
#include <any>

#include "content.hpp"

#include "rang.hpp"
#include "environment.hpp"

namespace saker {
    
    using namespace rang;
    
    template<typename T>
    using fitting_function = std::function<std::pair<std::string, T>(const std::string&, T)>;
    
    class Zone_ {
        private:
            // helpers
            fitting_function<std::string> transform_func_str;
            fitting_function<std::vector<std::string>> transform_func_vec_str;
            bool show_icon_first{true};
            bool to_be_shown{true};
            unsigned int order{};
            unsigned int priority{};
            Reset style_reseter{};
            mutable std::string carried_end{};
            mutable BgColor carried_bg{};
            // content-related fields
            FgColor zone_fg_color{};
            BgColor zone_bg_color{};
            Style zone_style{};
            Content content;
            Icon icon{};
            std::string end{};
        
            Zone_(const Content& content) : content(content) {}
        
            friend class Zone;
        
            inline void applyStrTransform() {
                const std::string str_content = std::get<0>(this->content.actual_content);
                const auto func = this->transform_func_str;
                const auto[new_icon_str, new_content_str] = transform_func_str(this->icon.actual_icon, str_content);
                this->icon.actual_icon = new_icon_str;
                this->content.actual_content = new_content_str;
            }
        
            inline void applyVecTransform() {
                const std::vector<std::string> vec_content = std::get<1>(this->content.actual_content);
                const auto func = this->transform_func_vec_str;
                const auto[new_icon_str, new_content_str] = transform_func_vec_str(
                    this->icon.actual_icon, vec_content
                );
                this->icon.actual_icon = new_icon_str;
                this->content.actual_content = new_content_str;
            }
    
        public:
        
            Zone_& setIfNotStyle(Style zone_style) {
                if (static_cast<int>(zone_style) == 0) {
                    this->zone_style = zone_style;
                    this->style_reseter = getReseterForStyle(zone_style);
                    this->content.setIfNotStyle(zone_style);
                    this->icon.setIfNotStyle(zone_style);
                }
                return *this;
            }
            
            Zone_& setIfNotBg(BgColor bg_color) {
                if (intReprOfBg(this->zone_bg_color) == 0) {
                    this->zone_bg_color = bg_color;
                    this->content.setIfNotBg(bg_color);
                    this->icon.setIfNotBg(bg_color);
                }
                return *this;
            }
            
            Zone_& setIfNotFg(FgColor fg_color) {
                if (intReprOfFg(this->zone_fg_color) == 0) {
                    this->zone_fg_color = fg_color;
                    this->content.setIfNotFg(fg_color);
                    this->icon.setIfNotFg(fg_color);
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
            
            unsigned int size() const {
                return this->content.size() + this->end.size();
            }
            
            unsigned int getPriority() const {
                return this->priority;
            }
            
            unsigned int getOrder() const {
                return this->order;
            }
            
            bool isToBeShown() const {
                return this->to_be_shown;
            }
            
            void transformWithFunc() {
                if (this->content.actual_content.index() == 0) { // string
                    if (!static_cast<bool>(this->transform_func_str)) { return; }
                    this->applyStrTransform();
                } else if (static_cast<bool>(this->transform_func_vec_str)) {
                    this->applyVecTransform();
                }
            }
    };
    
    class Zone {
        private:
            inline static unsigned int next_index{};
            Zone_ inner;
        public:
            Zone(const Content& content) : inner(content) {
                this->inner.order = next_index++;
                this->inner.content = content;
            }
            
            Zone(const Icon& icon, const Content& content) : inner(content) {
                this->inner.order = next_index++;
                this->inner.content = content;
                this->inner.show_icon_first = true;
                this->inner.icon = icon;
            }
            
            Zone(const Content& content, const Icon& icon) : inner(content) {
                this->inner.order = next_index++;
                this->inner.content = content;
                this->inner.show_icon_first = false;
                this->inner.icon = icon;
            }
            
            operator Zone_&() {
                return this->inner;
            }
            
            Zone& fg(FgColor zone_fg_color) {
                this->inner.zone_fg_color = zone_fg_color;
                this->inner.content.setIfNotFg(zone_fg_color);
                this->inner.icon.setIfNotFg(zone_fg_color);
                return *this;
            }
            
            Zone& bg(BgColor zone_bg_color) {
                this->inner.zone_bg_color = zone_bg_color;
                this->inner.content.setIfNotBg(zone_bg_color);
                this->inner.icon.setIfNotBg(zone_bg_color);
                return *this;
            }
            
            Zone& style(Style zone_style) {
                this->inner.setIfNotStyle(zone_style);
                this->inner.content.setIfNotStyle(zone_style);
                this->inner.icon.setIfNotStyle(zone_style);
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
        
            Zone& showIf(bool condition) {
                this->inner.to_be_shown = condition;
                return *this;
            }
        
            Zone& transformToFit(fitting_function<const std::vector<std::string>&> transform_func) {
                this->inner.transform_func_vec_str = transform_func;
                return *this;
            }
        
            Zone& transformToFit(fitting_function<const std::string&> transform_func) {
                this->inner.transform_func_str = transform_func;
                return *this;
            }
    
    };
    
    std::ostream& operator <<(std::ostream& os, const Zone_& zone) {
        os <<
           zone.zone_bg_color <<
           getFgForBg(zone.carried_bg) <<
           zone.carried_end <<
           zone.zone_style <<
           zone.zone_fg_color <<
           zone.zone_bg_color;
        if (zone.show_icon_first) {
            os << zone.icon << zone.content;
        } else {
            os << zone.content << zone.icon;
        }
        return os <<
                  zone.style_reseter;
    }
}

#endif //PROMPT_ZONE_HPP
