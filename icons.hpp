#ifndef PROMPT_ICONS_HPP
#define PROMPT_ICONS_HPP

#include <string>
#include <map>

namespace icons {
    
    std::map<std::string, std::string> special_dirs() {
        return {
            {"esege",              "\uf07c"},
            {"home",               "\uf7db"},
            {".config",            "\ue615"},
            {"Descargas",          "\uf019"},
            {"Codes",              "\uf121"},
            {"GoogleDrive",        "\ue246"},
            {"Imágenes",           "\uf03e"},
            {"MEGAsync Downloads", "\uf0c2"},
            {"Música",             "\ufc58"},
            {"Público",            "\uf1e0"},
            {"Vídeos",             "\uf03d"},
            {"Books",              "\uf02d"},
            {"CliTools",           "\ue795"},
            {"Android",            "\ue70e"},
            {"C#",                 "\uf81a"},
            {"C++",                "\ue61d"},
            {"Clojure",            "\ue76a"},
            {"Dart",               "\ue798"},
            {"Elixir",             "\ue62d"},
            {"Elm",                "\ue62c"},
            {"F#",                 "\ue7a7"},
            {"Go",                 "\ue627"},
            {"Groovy",             "\ue775"},
            {"Haskell",            "\ue61f"},
            {"Java",               "\ue256"},
            {"Javascript",         "\uf81d"},
            {"Julia",              "\ue624"},
            {"Nim",                "\ue26e"},
            {"PHP",                "\uf81e"},
            {"Python",             "\ue606"},
            {"Ruby",               "\ue791"},
            {"Rust",               "\ue7a8"},
            {"Scala",              "\ue737"},
            {"Shell",              "\uf120"},
            {"SQL",                "\ue706"},
            {"Swift",              "\ufbe3"},
            {"Typescript",         "\ufbe4"},
            {"Vue",                "\ufd42"},
            {"Web",                "\ue736"},
        };
    }
    
}

#endif //PROMPT_ICONS_HPP
