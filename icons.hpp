#ifndef PROMPT_ICONS_HPP
#define PROMPT_ICONS_HPP

#include <string>
#include <map>

namespace icons {

    std::map<std::string, std::string> special_dirs() {
        return {
                {"sergioomargarciarodriguez",       "\ufd24"},
                {"Users",                           "\uf0c0"},
                {".config",                         "\ue615"},
                {"Downloads",                       "\uf019"},
                {"IdeaProjects",                    "\ue256"},
                {"PycharmProjects",                 "\uf81f"},
                {"go",                              "\ufcd1"},
                {"rust",                            "\ue7a8"},
                {"shell",                           "\ue795"},
        };
    }

}

#endif //PROMPT_ICONS_HPP
