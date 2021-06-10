#ifndef PROMPT_CONTENT_HPP
#define PROMPT_CONTENT_HPP

#include <numeric>
#include "string"
#include "vector"

namespace saker {
    
    using namespace rang;
    
    using multi_string = std::variant<std::string, std::vector<std::string>>;
    
    inline std::ostream& operator <<(std::ostream& os, multi_string ms) {
        if (ms.index() == 1) {
            return os << std::get<1>(ms);
        }
        return os << std::get<0>(ms);
    }
    
    inline unsigned int size_of_ms(const multi_string& ms) {
        if (ms.index() == 0) {
            return std::get<0>(ms).size();
        }
        auto value = std::get<1>(ms);
        return std::accumulate(
            value.begin(),
            value.end(),
            0,
            [](int accum, const std::string& str) {
                return accum + str.size();
            }
        );
    }
    
    class Content {
        private:
            //helpers
            FgColor fg_color;
            FgColor prev_fg;
            BgColor bg_color;
            BgColor prev_bg;
            Style font_style;
            Style prev_style;
            // to show
            multi_string actual_content{};
            
            friend std::ostream& operator <<(std::ostream&, const Content&);
        
        public:
            
            Content(const char* msg) : actual_content(std::string(msg)) {}
            
            Content(const std::string& actual_content) : actual_content(actual_content) {}
            
            Content(const std::vector<std::string>& actual_content) : actual_content(actual_content) {}
            
            Content& fg(FgColor content_fg_color) {
                this->fg_color = content_fg_color;
                return *this;
            }
            
            Content& bg(BgColor content_bg_color) {
                this->bg_color = content_bg_color;
                return *this;
            }
            
            Content& style(Style content_style) {
                this->font_style = content_style;
                return *this;
            }
            
            Content& setPreviousValues(FgColor prev_fg, BgColor prev_bg, Style prev_style) {
                this->prev_bg = prev_bg;
                this->prev_fg = prev_fg;
                this->prev_style = prev_style;
                return *this;
            }
            
            unsigned int size() const {
                return size_of_ms(actual_content);
            }
    };
    
    
    std::ostream& operator <<(std::ostream& os, const Content& content) {
        return os <<
                  content.bg_color <<
                  content.fg_color <<
                  content.font_style <<
                  content.actual_content <<
                  content.prev_bg <<
                  content.prev_fg <<
                  content.prev_style;
    }
    
}

#endif //PROMPT_CONTENT_HPP
