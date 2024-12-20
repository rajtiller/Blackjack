#include "Simulator.h"

int main()
{
    Player p1;
    Simulator s({&p1}, {1}, 1, 1);
    std::unordered_map<int, double> cards;
    // for (int i = 0; i < 10000; i++)
    // {
    //     cards[s.draw_card()] += 13.0 / 10000;
    // }
    // for (auto it : cards)
    // {
    //     std::cout << "{" << it.first << ", " << it.second << "}, ";
    // }
    s.simulate_rounds(500000);
    s.print_statistics();
    return 0;
}