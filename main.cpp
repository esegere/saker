#include <iostream>
#include "saker.hpp"

int main() {
    std::cout <<
              saker::Prompt{
                  saker::Zone{
                      "hello"
                  }
                      .fg(saker::Fg::black)
                      .bg(saker::BgB::green)
                      .endWith("\ue0b4"),
                  saker::Zone{
                      "world"
                  }
                      .fg(saker::Fg::black)
                      .bg(saker::BgB::cyan)
                      .endWith("\ue0b0")
              }
                  .endWith(" ")
                  .show()
              << '\n';
}
