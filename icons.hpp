#ifndef PROMPT_ICONS_HPP
#define PROMPT_ICONS_HPP

#include <string>
#include <map>

namespace icons {

    std::map<std::string, std::string> special_dirs() {
        return {
                {"sergioomargarciarodriguez", "\ufd24"},
                {"Users",                     "\uf0c0"},
                {".config",                   "\ue615"},
                {"Downloads",                 "\uf019"},
        };
    }

}

#endif //PROMPT_ICONS_HPP
