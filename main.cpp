#include <iostream>
#include "saker.hpp"

int main() {
    std::cout << saker::Prompt{
        saker::Zone{
            "hello"
        }.fg(saker::FgB::green)
    }.style(saker::Style::bold)
    .bg(saker::BgB::gray)
    .fg(saker::Fg::green)
    .show()
    << '\n';
}
