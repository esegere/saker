#include <iostream>
#include "userdata.hpp"
#include "saker.hpp"
#include "argh.hpp"

int main(int argc, const char* argv[]) {
    
    const auto cli = argh::parser(argv);
    
    // presets
    const auto[hosticon, hostname] = userdata::get_host_icon_and_name();
    const auto[usericon, username, userbg] = userdata::get_user_icon_name_and_bg();
    auto git_data = userdata::get_git_branch_parent_repo_and_status();
    const auto git_branch = std::get<0>(git_data);
    const auto  git_repo_dir = std::get<1>(git_data);
    const auto  git_status_counts = std::get<2>(git_data);
    auto dir_info = userdata::get_directory_icon_and_parts(git_repo_dir);
    const auto diricon = std::get<0>(dir_info);
    auto dirparts = std::get<1>(dir_info);
    auto subdirs_skipped = std::get<2>(dir_info);
    auto git_subdirs_skipped = std::get<3>(dir_info);
    int line_number;
    cli("l", "0") >> line_number;
    int prev_error_code;
    cli("e", "0") >> prev_error_code;
    int jobs_managed;
    cli("j", "0") >> jobs_managed;
    std::string extra_info;
    cli("xc", "") >> extra_info;
    std::string extra_icon;
    cli("xi", "") >> extra_icon;
    
    if (!git_branch.empty()) {
        const auto last_to_erase = std::find_if(
            dirparts.cbegin(), dirparts.cend(),
            [&git_repo_dir](const auto elem) {
                return git_repo_dir.find(elem) != std::string::npos;
            }
        );
        if (last_to_erase != dirparts.cend()) {
            dirparts.erase(
                dirparts.cbegin(),
                last_to_erase + 1
            );
        }
        if (git_subdirs_skipped > 0){
            dirparts.insert(dirparts.begin(), std::to_string(git_subdirs_skipped));
        }
        if (!dirparts.empty()) {
            dirparts.front().insert(0, " ");
        } else if (git_status_counts.total > 0) {
            dirparts.emplace_back("");
        }
    } else if (!dirparts.empty() && subdirs_skipped > 0){
        dirparts.insert(dirparts.cbegin(),"\uf752 " + std::to_string(subdirs_skipped));
    }
    
    
    rang::BgColor git_zones_bg = saker::Bg::magenta;
    rang::BgColor git_statuses_bg = saker::Bg::black;
    
    std::cout <<
              saker::Prompt{
    
                  saker::Zone{ // line number
                      std::to_string(line_number)
                  }.bg(saker::Bg::black)
                   .fg(saker::Fg::yellow)
                   .style(saker::Style::bold)
                   .endWith("\ue0b4")
                   .priority(3)
                   .showIf(line_number > 0),
    
                  saker::Zone{ // error code of previous comand
        
                      saker::Icon{
                          " \uF071 "
                      },
        
                      std::to_string(prev_error_code)
        
                  }.bg(saker::Bg::red)
                   .priority(4)
                   .endWith("\ue0b4")
                   .showIf(prev_error_code > 0),
    
                  saker::Zone{ // jobs running in the background
        
                      saker::Icon{
                          " \uf56e "
                      },
        
                      std::to_string(jobs_managed)
        
                  }.bg(saker::Bg::blue)
                   .fg(saker::FgB::gray)
                   .priority(4)
                   .endWith("\ue0b4")
                   .showIf(jobs_managed > 0),
    
                  saker::Zone{ // host
        
                      saker::Icon{
                          hosticon
                      },

                      saker::Content{
                          hostname
                      }
            
                  }.bg(saker::Bg::green)
                   .priority(2)
                   .transformToFit(saker::transforming::drop_content<std::string>)
                   .endWith("\ue0b4"),
    
                  saker::Zone{ // user
            
                      saker::Icon{
                          usericon
                      },

                      saker::Content{
                          username
                      }
        
                  }.bg(userbg)
                   .transformToFit(saker::transforming::drop_content<std::string>)
                   .endWith("\ue0b4"),
    
    
                  saker::Zone{ // extra info
        
                      saker::Icon{
                          " " + extra_icon + " "
                      },
        
                      saker::Content{
                          extra_info
                      }
        
                  }.bg(saker::Bg::yellow)
                   .showIf(!extra_info.empty() && !extra_icon.empty())
                   .transformToFit(saker::transforming::drop_content<std::string>)
                   .priority(4)
                   .endWith("\ue0b4"),
    
    
                  saker::Zone{ // directory

                      saker::Icon{
                          diricon
                      },

                      saker::Content{
                                git_branch.empty() ?
                                    dirparts
                                    : subdirs_skipped > 0
                                        ? std::vector{"\uf752 " + std::to_string(subdirs_skipped), git_repo_dir}
                                        : std::vector{git_repo_dir}
                      }.separatedBy(" \uE0B1 ", true)
                       .separatorFg(saker::Fg::black)

                  }.fg(saker::Fg::gray)
                   .bg(saker::BgB::black)
                   .priority(7)
                   .transformToFit(saker::transforming::drop_first_vec)
                   .endWith(!git_branch.empty() ? "\ue0c6" : "\ue0b0"),

    
                  saker::Zone{ // git branch
            
                      saker::Icon{
                          " \uf418 "
                      },
            
                      saker::Content{
                          git_branch
                      }
            
                  }.bg(git_zones_bg)
                   .fg(saker::Fg::black)
                   .priority(9)
                   .endWith(git_status_counts.total > 0 ? "\ue0c6 " : "")
                   .transformToFit(saker::transforming::drop_icon<std::string>)
                   .showIf(!git_branch.empty()),
    
    
                  saker::Zone{ // git modified
            
                      saker::Content{
                          "\ufbfd "
                      }.fg(saker::FgB::blue)
            
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.modified > 0),
    
    
                  saker::Zone{ // git new
            
                      saker::Content{
                          "\uf916"
                      }.fg(saker::FgB::green)
            
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.new_ > 0),
    
    
                  saker::Zone{ // git deleted
            
                      saker::Content{
                          "\ufb81 "
                      }.fg(saker::FgB::red)
            
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.deleted > 0),
    
    
                  saker::Zone{ // git renamed
            
                      saker::Content{
                          "\uf138 "
                      }.fg(saker::FgB::yellow)
            
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.renamed > 0),
    
    
                  saker::Zone{ // git type change
            
                      saker::Content{
                          "\uf059 "
                      }.fg(saker::FgB::yellow)
            
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.type_change > 0),
    
    
                  saker::Zone{ // git error
            
                      saker::Content{
                          "\uf057 "
                      }.fg(saker::FgB::gray)
            
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.with_errors > 0),
    
                  saker::Zone{
        
                      saker::Content{
                          "\uf0a9 "
                      }.fg(saker::FgB::gray)
        
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.ahead_of_remote > 0),
  
                  saker::Zone{
        
                      saker::Content{
                          "\uf0a8 "
                      }.fg(saker::FgB::gray)
        
                  }.bg(git_statuses_bg)
                   .priority(8)
                   .showIf(git_status_counts.behind_remote > 0),
        
                  saker::Zone{ // --finish-- git info
            
                      ""
            
                  }.bg(git_status_counts.total > 0 ? git_statuses_bg : git_zones_bg)
                   .priority(9)
                   .showIf(!git_repo_dir.empty())
                   .endWith("\ue0b0"),
    
    
                  saker::Zone{ // repo subdir

                      saker::Icon{
                              git_subdirs_skipped > 0 ? " \uf752" : ""
                      },

                      saker::Content{
                          dirparts
                      }.separatedBy(" \uE0B1 ", false)
                       .separatorFg(saker::Fg::black)
            
                  }.fg(saker::Fg::gray)
                   .bg(saker::BgB::black)
                   .priority(10)
                   .transformToFit(saker::transforming::drop_first_vec)
                   .showIf(!git_branch.empty() && !dirparts.empty())
                   .endWith("\ue0b0")
        
              }.fg(saker::Fg::black)
               .endWith(" ")
               .maxSize(60)
               .show();
}
    
