#ifndef PROMPT_USERDATA_HPP
#define PROMPT_USERDATA_HPP

#include <vector>
#include <string>
#include <utility>
#include "icons.hpp"
#include <fplus/fplus.hpp>
#include <fplus/fwd.hpp>
#include <unistd.h>
#include <climits>
#include "saker.hpp"
#include <git2.h>

namespace userdata {
    
    struct GitStatusCounts {
            int total = 0;
            int new_ = 0;
            int modified = 0;
            int deleted = 0;
            int renamed = 0;
            int type_change = 0;
            int with_errors = 0;
    };
    
    auto get_directory_icon_and_parts() -> std::pair<std::string, std::vector<std::string>> {
        // modify only this parameters
        
        constexpr const char* DEFAULT_ICON = "\uf023";
        constexpr unsigned int NUMBER_OF_RELEVANT_SUBDIRS = 2;
        const std::map<std::string, std::string> ICONS = icons::special_dirs();
        // creation
        
        const std::string path = get_current_dir_name();
        std::string selected_icon{DEFAULT_ICON};
        std::vector<std::string> result_string_parts =
            fplus::fwd::apply(
                path,
                fplus::fwd::split('/', false),
                fplus::fwd::reverse(),
                fplus::fwd::take_while(
                    [&ICONS, &selected_icon](auto elem) -> bool {
                        if (auto found = ICONS.find(elem); found != ICONS.end()) {
                            selected_icon = found->second;
                            return false;
                        }
                        return true;
                    }
                ),
                fplus::fwd::take(NUMBER_OF_RELEVANT_SUBDIRS),
                fplus::fwd::reverse()
            );
        selected_icon = " " + selected_icon;
        if (!result_string_parts.empty()) {
            result_string_parts.back().append(" ");
        } else {
            selected_icon.append(" ");
        }
        return {selected_icon, result_string_parts};
    }
    
    auto get_user_icon_name_and_bg() -> std::tuple<std::string, std::string, saker::BgColor> {
        std::string user = getenv("USER");
        std::string user_icon;
        saker::BgColor bg = saker::Bg::cyan;
        if (user == getlogin()) {
            user_icon = " \uF007 ";
        } else if (user == "root") {
            user_icon = " \uF2dd ";
            bg = saker::Bg::blue;
        } else {
            user_icon = " \uF0C0 ";
            bg = saker::BgB::gray;
        }
        return {user_icon, user, bg};
    }
    
    auto get_host_icon_and_name() -> std::pair<std::string, std::string> {
        std::string hostname;
        char hn_char[HOST_NAME_MAX];
        gethostname(hn_char, HOST_NAME_MAX);
        hostname = hn_char;
        std::string hosticon{};
        if (hostname == "dellneon") {
            hosticon = " \uF31B ";
        } else if (hostname == "dellsuse") {
            hosticon = " \uF314 ";
        } else {
            hosticon = " \uE712 ";
        }
        return {hosticon, hostname};
    }
    
    auto get_status_values(git_repository* repo) -> GitStatusCounts {
        git_status_list* status;
        git_status_options statusopt = {GIT_STATUS_OPTIONS_INIT};
        
        statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
        statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                          GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
                          GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
        int ret = git_status_list_new(&status, repo, &statusopt);
        if (ret != 0) {
            return {};
        }
        size_t maxi = git_status_list_entrycount(status);
        const git_status_entry* s;
        GitStatusCounts gs;
        for (size_t i = 0; i < maxi; ++i) {
            
            s = git_status_byindex(status, i);
            
            if ((s->status & (GIT_STATUS_INDEX_NEW | GIT_STATUS_WT_NEW)) != 0) {
                gs.new_++;
                gs.total++;
            }
            if ((s->status & (GIT_STATUS_INDEX_MODIFIED | GIT_STATUS_WT_MODIFIED)) != 0) {
                gs.modified++;
                gs.total++;
            }
            if ((s->status & (GIT_STATUS_INDEX_DELETED | GIT_STATUS_WT_DELETED)) != 0) {
                gs.deleted++;
                gs.total++;
            }
            if ((s->status & (GIT_STATUS_INDEX_RENAMED | GIT_STATUS_WT_RENAMED)) != 0) {
                gs.renamed++;
                gs.total++;
            }
            if ((s->status & (GIT_STATUS_INDEX_TYPECHANGE | GIT_STATUS_WT_TYPECHANGE)) != 0) {
                gs.type_change++;
                gs.total++;
            }
            if ((s->status & (GIT_STATUS_WT_UNREADABLE | GIT_STATUS_CONFLICTED)) != 0) {
                gs.with_errors++;
                gs.total++;
            }
        }
        return gs;
    }
    
    auto get_git_branch_parent_repo_and_status() -> std::tuple<std::string, std::string, GitStatusCounts> {
        const auto fail = []() {
            git_libgit2_shutdown();
            return std::make_tuple("", "", GitStatusCounts{});
        };
        git_libgit2_init();
        git_repository* repo;
        git_reference* head;
        int error;
        std::string path = get_current_dir_name();
        std::string repo_parent;
        
        while (!path.empty() && error != 0) {
            error = git_repository_open(&repo, path.c_str());
            auto last = path.find_last_of("/");
            repo_parent = path.substr(last + 1, path.size()); // +1 to skip "/"
            path = path.substr(0, last);
        }
        if (error != 0) return fail();
        error = git_repository_head(&head, repo);
        if (error != 0) return fail();
        std::string branch_name = git_reference_shorthand(head);
        GitStatusCounts gs = get_status_values(repo);
        git_libgit2_shutdown();
        return {branch_name + " ", repo_parent + " ", gs};
    }
    
}

#endif //PROMPT_USERDATA_HPP
