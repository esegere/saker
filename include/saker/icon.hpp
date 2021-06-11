#ifndef PROMPT_ICON_HPP
#define PROMPT_ICON_HPP

namespace saker {
    
    using namespace rang;
    
    class Icon {
        private:
            //helpers
            FgColor fg_color{};
            FgColor prev_fg{};
            BgColor bg_color{};
            BgColor prev_bg{};
            Style font_style{};
            Style prev_style{};
            Reset font_style_reset{};
            // to show
            std::string actual_icon{};
            
            friend std::ostream& operator <<(std::ostream&, const Icon&);
            
            friend class Zone_;
            
            friend class Zone;
            
            unsigned int size() const {
                return actual_icon.size();
            }
            
            Icon& setIfNotStyle(Style content_style) {
                if (static_cast<int>(this->font_style) == 0) {
                    this->font_style = content_style;
                    this->font_style_reset = getReseterForStyle(content_style);
                }
                return *this;
            }
            
            Icon& setIfNotBg(BgColor bg_color) {
                const auto intReprOfBg = [](BgColor bg) -> int {
                    if (bg.index() == 1) {
                        return static_cast<int>(std::get<1>(bg));
                    }
                    return static_cast<int>(std::get<0>(bg));
                };
                if (intReprOfBg(this->bg_color) == 0) {
                    this->bg_color = bg_color;
                }
                return *this;
            }
            
            Icon& setIfNotFg(FgColor fg_color) {
                const auto intReprOfFg = [](FgColor fg) -> int {
                    if (fg.index() == 1) {
                        return static_cast<int>(std::get<1>(fg));
                    }
                    return static_cast<int>(std::get<0>(fg));
                };
                if (intReprOfFg(this->fg_color) == 0) {
                    this->fg_color = fg_color;
                }
                return *this;
            }
        
        
        public:
            
            explicit Icon() : actual_icon("") {}
            
            explicit Icon(const char* msg) : actual_icon(std::string(msg)) {}
            
            explicit Icon(const std::string& actual_content) : actual_icon(actual_content) {}
            
            Icon& fg(FgColor content_fg_color) {
                this->fg_color = content_fg_color;
                return *this;
            }
            
            Icon& bg(BgColor content_bg_color) {
                this->bg_color = content_bg_color;
                return *this;
            }
            
            Icon& style(Style content_style) {
                this->font_style = content_style;
                return *this;
            }
        
    };
    
    
    std::ostream& operator <<(std::ostream& os, const Icon& icon) {
        return os <<
                  icon.bg_color <<
                  icon.fg_color <<
                  icon.font_style <<
                  icon.actual_icon << // icon
                  icon.font_style_reset <<
                  icon.prev_bg <<
                  icon.prev_fg <<
                  icon.prev_style;
    }
    
}

#endif //PROMPT_ICON_HPP
