#include <iostream>
#include "saker.hpp"

int main() {
    std::cout <<
              saker::Prompt{
    
                  saker::Zone{
        
                      " hello "
        
                  }.bg(saker::Bg::green)
                   .priority(1)
                   .endWith("\ue0b4"),
    
                  saker::Zone{
        
                      " marvelous "
        
                  }.bg(saker::Bg::cyan)
                   .priority(2)
                   .endWith("\ue0b4"),
    
                  saker::Zone{
        
                      " world "
        
                  }.fg(saker::Fg::gray)
                   .bg(saker::BgB::black)
                   .endWith("\ue0b0"),
    
              }.fg(saker::Fg::black)
               .endWith(" ")
               .maxSize(50)
               .show()

              << '\n';
}
