#ifndef PROMPT_ENVIRONMENT_HPP
#define PROMPT_ENVIRONMENT_HPP

#include <sys/ioctl.h>
#include <unistd.h>

#include <utility>

namespace env{
    using WindowSize = std::pair<unsigned int, unsigned int>;
    
    WindowSize getTermSize(){
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        return std::make_pair(size.ws_col, size.ws_row);
    }
    
}

#endif //PROMPT_ENVIRONMENT_HPP
