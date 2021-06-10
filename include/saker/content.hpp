#ifndef PROMPT_CONTENT_HPP
#define PROMPT_CONTENT_HPP

#include <numeric>
#include "string"
#include "vector"

namespace saker {
    
    using namespace rang;
    
    using multi_string = std::variant<std::string, std::vector<std::string>>;
    
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
            FgColor fg_color{};
            FgColor prev_fg{};
            FgColor separator_fg{};
            BgColor bg_color{};
            BgColor prev_bg{};
            BgColor separator_bg{};
            Style font_style{};
            Style prev_style{};
            Style separator_style{};
            // to show
            std::string separator{};
            multi_string actual_content{};
        
            friend std::ostream& operator <<(std::ostream&, const Content&);
        
            friend class Zone_;
        
            unsigned int size() const {
                unsigned int size_without_separator = size_of_ms(this->actual_content);
                if (this->actual_content.index() == 0) {
                    return size_without_separator;
                }
                return size_without_separator +
                       (this->separator.size() * (
                           std::get<1>(this->actual_content).size() - 1
                       ));
            }
        
            Content& setPreviousValues(FgColor prev_fg, BgColor prev_bg, Style prev_style) {
                this->prev_bg = prev_bg;
                this->prev_fg = prev_fg;
                this->prev_style = prev_style;
                return *this;
            }
    
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
        
            Content& separatedBy(const std::string& separator) {
                this->separator = separator;
                return *this;
            }
        
            Content& separatorFg(FgColor separator_fg) {
                this->separator_fg = separator_fg;
                return *this;
            }
        
            Content& separatorBg(BgColor separator_bg) {
                this->separator_bg = separator_bg;
                return *this;
            }
        
            Content& separatorStyle(Style separator_style) {
                this->separator_style = separator_style;
                return *this;
            }
    };
    
    
    std::ostream& operator <<(std::ostream& os, const Content& content) {
    
        if (content.actual_content.index() == 1) { //if vector
            auto vector_content = std::get<1>(content.actual_content);
            std::for_each(
                vector_content.begin(),
                vector_content.end() - 1,
                [&os, &content](const auto& item) mutable {
                    os <<
                       item <<
                       content.separator_fg <<
                       content.separator_bg <<
                       content.separator_style <<
                       content.separator << // actual separator string
                       content.fg_color <<
                       content.bg_color <<
                       content.font_style;
                }
            );
            os << vector_content.back();
        } else { //if string
            os <<
               content.bg_color <<
               content.fg_color <<
               content.font_style <<
               std::get<0>(content.actual_content);
        }
        return os <<
                  content.prev_bg <<
                  content.prev_fg <<
                  content.prev_style;
    }
    
}

#endif //PROMPT_CONTENT_HPP
