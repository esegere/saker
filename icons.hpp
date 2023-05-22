#ifndef PROMPT_ICONS_HPP
#define PROMPT_ICONS_HPP

#include <string>
#include <map>

namespace icons {

    auto special_dirs() -> std::map<std::string, std::string> {
        return {
                {"sgarcia",                         "\ue617"},
                {"home",                            "\uf0c0"},
                {".config",                         "\ue615"},
                {"downloads",                       "\uf019"},
                {"Downloads",                       "\uf019"},
                {"IdeaProjects",                    "\ue634"},
                {"PycharmProjects",                 "\ue73c"},
                {"go",                              "\ue65e"},
                {"rust",                            "\ue7a8"},
                {"shell",                           "\ue795"},
                {"cpp",                             "\ue61d"},
        };
    }

}

#endif //PROMPT_ICONS_HPP
