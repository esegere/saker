#ifndef PROMPT_ICONS_HPP
#define PROMPT_ICONS_HPP

#include <string>
#include <map>

namespace icons {

    std::map<std::string, std::string> special_dirs() {
        return {
                {"sgarcia",       "\ufd24"},
                {"home",                           "\uf0c0"},
                {".config",                         "\ue615"},
                {"downloads",                       "\uf019"},
                {"IdeaProjects",                    "\ue256"},
                {"PycharmProjects",                 "\uf81f"},
                {"go",                              "\ufcd1"},
                {"rust",                            "\ue7a8"},
                {"shell",                           "\ue795"},
                {"cpp",                           "\ue61d"},
        };
    }

}

#endif //PROMPT_ICONS_HPP
