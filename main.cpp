#include <iostream>
#include "saker.hpp"

int main() {
    std::cout <<
              saker::Prompt{
                  saker::Zone{
                      " hello "
                  }
                      .bg(saker::BgB::green)
                      .priority(1)
                      .endWith("\ue0b4"),
                  saker::Zone{
                      " marvelous "
                  }
                      .bg(saker::BgB::cyan)
                      .priority(2)
                      .endWith("\ue0b4"),
                  saker::Zone{
                      " world "
                  }
                      .fg(saker::FgB::gray)
                      .bg(saker::Bg::black)
                      .endWith("\ue0b0")
              }
                  .fg(saker::Fg::black)
                  .endWith(" ")
                  .maxSize(50)
                  .show()
              << '\n';
}
