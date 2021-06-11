#include <iostream>
#include "saker.hpp"

int main() {
    std::cout <<
              saker::Prompt{
    
                  saker::Zone{
        
                      saker::Content{
                          "dellneon"
                      }
        
                  }.bg(saker::Bg::green)
                   .priority(1)
                   .endWith("\ue0b4"),
    
                  saker::Zone{
        
                      saker::Content{
                          " esege "
                      }
        
                  }.bg(saker::Bg::cyan)
                   .priority(2)
                   .endWith("\ue0b4"),
    
                  saker::Zone{
    
                      saker::Content{
                          std::vector<std::string>({" home", "esege", "Codes "})
                      }.separatedBy(" \uE0B1 ")
                       .separatorFg(saker::Fg::black)
        
                  }.fg(saker::Fg::gray)
                   .bg(saker::BgB::black)
                   .endWith("\ue0b0"),
    
                  saker::Zone{
    
                      " this is not shown "
    
                  }.showIf(false),
    
              }.fg(saker::Fg::black)
               .endWith(" ")
               .maxSize(50)
               .show()

              << '\n';
    
    auto var = std::vector({"w", "o", "r", "l", "d"});
}
