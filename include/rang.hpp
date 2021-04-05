#ifndef RANG_DOT_HPP
#define RANG_DOT_HPP

#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace rang {

/* For better compability with most of terminals do not use any Style settings
 * except of reset, bold and reversed.
 * Note that on Windows terminals bold Style is same as FgB color.
 */
    enum class Style {
            unset = 0,
            bold = 1,
            dim = 2,
            italic = 3,
            underline = 4,
            blink = 5,
            rblink = 6,
            reversed = 7,
            conceal = 8,
            crossed = 9,
    };
    
    enum class Reset {
            all = 0,
            bold = 21,
            dim = 22,
            italic = 23,
            underline = 24,
            blink = 25,
            rblink = 26,
            reversed = 27,
            conceal = 28,
            crossed = 29,
    };
    
    enum class Fg {
            unset = 0,
            black = 30,
            red = 31,
            green = 32,
            yellow = 33,
            blue = 34,
            magenta = 35,
            cyan = 36,
            gray = 37,
            default_ = 39,
    };
    
    enum class Bg {
            unset = 0,
            black = 40,
            red = 41,
            green = 42,
            yellow = 43,
            blue = 44,
            magenta = 45,
            cyan = 46,
            gray = 47,
            default_ = 49,
    };
    
    enum class FgB {
            unset = 0,
            black = 90,
            red = 91,
            green = 92,
            yellow = 93,
            blue = 94,
            magenta = 95,
            cyan = 96,
            gray = 97,
    };
    
    enum class BgB {
            unset = 0,
            black = 100,
            red = 101,
            green = 102,
            yellow = 103,
            blue = 104,
            magenta = 105,
            cyan = 106,
            gray = 107,
    };
    
    enum class Control {  // Behaviour of rang function calls
            Off = 0,  // toggle off rang Style/color calls
            Auto = 1,  // (Default) autodect terminal and colorize if needed
            Force = 2  // force ansi color output to non terminal streams
    };
// Use rang::setControlMode to set rang Control mode
    
    enum class WinTerm {  // Windows Terminal Mode
            Auto = 0,  // (Default) automatically detects wheter Ansi or Native API
            Ansi = 1,  // Force use Ansi API
            Native = 2  // Force use Native API
    };
// Use rang::setWinTermMode to explicitly set terminal API for Windows
// Calling rang::setWinTermMode have no effect on other OS
    
    namespace rang_implementation {
        
        inline std::atomic<Control>& controlMode() noexcept {
            static std::atomic<Control> value(Control::Auto);
            return value;
        }
        
        inline std::atomic<WinTerm>& winTermMode() noexcept {
            static std::atomic<WinTerm> termMode(WinTerm::Auto);
            return termMode;
        }
        
        inline bool supportsColor() noexcept {
            static const bool result = [] {
                const char* Terms[]
                    = {"ansi", "color", "console", "cygwin", "gnome",
                       "konsole", "kterm", "linux", "msys", "putty",
                       "rxvt", "screen", "vt100", "xterm"};
                
                const char* env_p = std::getenv("TERM");
                if (env_p == nullptr) {
                    return false;
                }
                return std::any_of(std::begin(Terms), std::end(Terms),
                                   [&](const char* term) {
                                       return std::strstr(env_p, term) != nullptr;
                                   });
            }();
            return result;
        }
        
        
        inline bool isTerminal(const std::streambuf* osbuf) noexcept {
            using std::cerr;
            using std::clog;
            using std::cout;
            if (osbuf == cout.rdbuf()) {
                static const bool cout_term = isatty(fileno(stdout)) != 0;
                return cout_term;
            } else if (osbuf == cerr.rdbuf() || osbuf == clog.rdbuf()) {
                static const bool cerr_term = isatty(fileno(stderr)) != 0;
                return cerr_term;
            }
            return false;
        }
        
        template<typename T>
        using enableStd = typename std::enable_if<
            std::is_same<T, rang::Style>::value || std::is_same<T, rang::Fg>::value
            || std::is_same<T, rang::Bg>::value || std::is_same<T, rang::FgB>::value
            || std::is_same<T, rang::BgB>::value || std::is_same<T, rang::Reset>::value,
            std::ostream&>::type;
        
        template<typename T>
        inline enableStd<T> setColor(std::ostream& os, T const value) {
            switch (int int_value = static_cast<int>(value); int_value) {
                case 0:
                    if constexpr(!std::is_same<T, rang::Reset>::value) {
                        return os;
                    }
                default:
                    return os << "\033[" << int_value << "m";
            }
        }
        
    }  // namespace rang_implementation
    
    template<typename T>
    inline rang_implementation::enableStd<T> operator <<(std::ostream& os,
                                                         const T value) {
        const Control option = rang_implementation::controlMode();
        switch (option) {
            case Control::Auto:
                return rang_implementation::supportsColor()
                       && rang_implementation::isTerminal(os.rdbuf())
                       ? rang_implementation::setColor(os, value)
                       : os;
            case Control::Force:
                return rang_implementation::setColor(os, value);
            default:
                return os;
        }
    }
    
    inline void setWinTermMode(const rang::WinTerm value) noexcept {
        rang_implementation::winTermMode() = value;
    }
    
    inline void setControlMode(const Control value) noexcept {
        rang_implementation::controlMode() = value;
    }
    
    inline Reset getReseterForStyle(const Style& style) {
        switch (style) {
            case Style::bold :
                return Reset::bold;
            case Style::dim :
                return Reset::dim;
            case Style::italic :
                return Reset::italic;
            case Style::underline :
                return Reset::underline;
            case Style::blink :
                return Reset::blink;
            case Style::rblink :
                return Reset::rblink;
            case Style::reversed :
                return Reset::reversed;
            case Style::conceal :
                return Reset::conceal;
            case Style::crossed:
                return Reset::crossed;
            default:
                return Reset::all;
        }
    }
    
}  // namespace rang

#endif /* ifndef RANG_DOT_HPP */
