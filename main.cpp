#include <iostream>
#include "userdata.hpp"
#include "saker.hpp"
#include "argh.hpp"

int main(int argc, const char* argv[]) {
    
    const auto cli = argh::parser(argv, argh::parser::Mode::PREFER_PARAM_FOR_UNREG_OPTION);
    
    // presets
    const auto[hosticon, hostname] = userdata::get_host_icon_and_name();
    const auto[usericon, username, userbg] = userdata::get_user_icon_name_and_bg();
    const auto[diricon, dirparts] = userdata::get_directory_icon_and_parts();
    int line_number;
    cli("l", "0") >> line_number;
    int prev_error_code;
    cli("e", "0") >> prev_error_code;
    
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
    
                  saker::Zone{ // host
            
                      saker::Icon{
                          hosticon
                      },
            
                      saker::Content{
                          hostname
                      }
            
                  }.bg(saker::Bg::green)
                   .priority(2)
                   .endWith("\ue0b4"),
    
                  saker::Zone{ // user
        
                      saker::Icon{
                          usericon
                      },
        
                      saker::Content{
                          username
                      }
        
                  }.bg(userbg)
                   .endWith("\ue0b4"),
    
                  saker::Zone{ // directory
        
                      saker::Icon{
                          diricon
                      },
        
                      saker::Content{
                          dirparts
                      }.separatedBy(" \uE0B1 ", true)
                       .separatorFg(saker::Fg::black)
        
                  }.fg(saker::Fg::gray)
                   .bg(saker::BgB::black)
                   .priority(10)
                   .endWith("\ue0b0"),
        
        
              }.fg(saker::Fg::black)
               .endWith(" ")
               .maxSize(50)
               .show()
    
              << '\n';
}
