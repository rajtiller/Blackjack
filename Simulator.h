#include "Player.h"
class Simulator
{
public:
    
    Simulator( std::vector<Player *> player_ptrs,std::vector<int> player_inxs = {1}, int num_player = 1, int num_player_hands=1);
    void simulate_rounds(int num_rounds);
    int player_wins(Hand &player_hand);
    int draw_card();
    void print_statistics();

private:
    std::deque<Hand> player_hands;
    std::vector<Player *> players;
    std::vector<int> deck;
    int num_player_hands;
    int deck_size;
    int rounds_simulated;
    std::vector<int> player_earnings;

    std::mt19937 gen;                     // Random number generator (Mersenne Twister)
    std::uniform_real_distribution<> dis; // Uniform distribution [0, 1)
};