#include <random>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <cmath>
#include <iomanip>
enum Choice
{
    HIT,
    STAND,
    DOUBLE,
    SPLIT
};
struct Hand
{
    Hand() : player_inx(0),sum(0), soft(false), bet(1), splittable(false) {};
    Hand(int player_inx, int sum, bool soft, int bet, bool splittable) : player_inx(player_inx), sum(sum),soft(soft),bet(bet),splittable(splittable){};
    int player_inx;
    int sum;
    bool soft;
    int bet;
    bool splittable;
};

class Player
{
public:
    Choice get_choice(Hand &hand, Hand &dealer, std::vector<int> &deck){return STAND;};
};