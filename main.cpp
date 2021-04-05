#include <iostream>
#include "saker.hpp"

int main() {
    std::cout << saker::Prompt{
            saker::Zone{
                    "hello"
            }.fg(saker::FgB::red)
                    .style(saker::Style::reversed),
            saker::Zone{
                    "world"
            }.bg(saker::BgB::cyan)
    }.style(saker::Style::bold)
            .show()
              << '\n';
}
