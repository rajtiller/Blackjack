#include "Simulator.h"

Simulator::Simulator(std::vector<Player *> player_ptrs, std::vector<int> player_inxs, int num_players, int num_player_hands)
    : player_hands(), players(), deck(), num_player_hands(num_player_hands), deck_size(312),
      rounds_simulated(0), gen(std::random_device{}()), dis(0.0, 1.0)
{
    deck.resize(11);
    for (int card : {9, 8, 7, 6, 5, 4, 3, 2, 1})
    {
        deck[card] = 24;
    }
    deck[10] = 96;
    std::random_device rd;
    static std::mt19937 gen(rd()); // Mersenne Twister engine
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    player_hands.resize(1 + num_player_hands);
    player_hands[0].player_inx = 0;
    players.resize(num_players + 1);
    for (int i = 0; i < num_player_hands; i++)
    {
        player_hands[i].bet = 1;
        player_hands[i].soft = false;
        player_hands[i].splittable = false;
        player_hands[i].sum = 0;
        player_hands[i + 1].player_inx = player_inxs[i]; // player_hands is size num_players+1 bc dealer is player_inx 0
        players[i + 1] = player_ptrs[i];
    }
    player_earnings.resize(num_players + 1, 0);
    num_busts=0;
}
void Simulator::simulate_rounds(int num_rounds)
{
    for (int j = 0; j < num_rounds; j++)
    {
        std::vector<int> cards_to_be_dealt;
        int num_cards_need_dealt = 1 + 2 * num_player_hands;
        int num_cards_dealt = 0;
        while (num_cards_dealt < num_cards_need_dealt)
        {
            cards_to_be_dealt.push_back(draw_card());
            num_cards_dealt++;
        }
        player_hands[0].sum += cards_to_be_dealt[0];
        player_hands[0].soft = cards_to_be_dealt[0] == 1;
        for (int i = 1; i < num_cards_need_dealt; i++)
        {
            if (cards_to_be_dealt[i] == 1)
            {
                player_hands[int(0.6 + i / 2.0)].soft = true;
            }
            if (i % 2 == 0 && player_hands[int(0.6 + i / 2.0)].sum == cards_to_be_dealt[i])
            {
                player_hands[int(0.6 + i / 2.0)].splittable = true;
            }
            player_hands[int(0.6 + i / 2.0)].sum += cards_to_be_dealt[i];
        }
        size_t curr_hand_inx = 1;
        while (curr_hand_inx < player_hands.size())
        { // simulate all players' decisions
            bool curr_hand_done = false;
            Player *curr_player = players[player_hands[curr_hand_inx].player_inx];
            Hand &curr_player_hand = player_hands[curr_hand_inx];
            while (!curr_hand_done && curr_player_hand.sum < 22)
            {
                switch (curr_player->get_choice(curr_player_hand, player_hands[0], deck))
                {
                case HIT:
                    curr_player_hand.sum += draw_card();
                    break;
                case STAND:
                    curr_hand_done = true;
                    break;
                case SPLIT:
                {
                    if (!curr_player_hand.splittable) {
                        curr_hand_done = true;
                        break;
                    }
                    int new_card_1 = draw_card(); // goes to new_hand
                    Hand new_hand = {curr_player_hand.player_inx, curr_player_hand.sum / 2 + new_card_1, curr_player_hand.sum == 2, 1, new_card_1 * 2 == curr_player_hand.sum};
                    player_hands.push_back({new_hand});
                    int new_card_2 = draw_card(); // goes to old_hand
                    curr_player_hand.sum /= 2;
                    curr_player_hand.splittable = new_card_2 == curr_player_hand.sum;
                    curr_player_hand.sum += new_card_2;
                    curr_player_hand.soft = new_card_2 == 1;
                }
                break;
                case DOUBLE:
                    curr_player_hand.sum += draw_card();
                    curr_player_hand.bet = 2;
                    curr_hand_done = true;
                    break;
                }
            }
            curr_hand_inx++;
        }
        while (player_hands[0].sum < 2 || (player_hands[0].sum == 17 && player_hands[0].soft))
        {
            int new_card = draw_card();
            if (new_card == 1)
            {
                player_hands[0].soft = true;
            }
            player_hands[0].sum += new_card;
        }
        // add to overall money and erase extra hands
        while (player_hands.size() > num_player_hands + 1)
        {
            Hand &curr_hand = player_hands.back(); // might wanna change this back to non-reference
            player_hands.pop_back();
            player_earnings[curr_hand.player_inx] += player_wins(curr_hand) * curr_hand.bet;
        }
        if (player_hands[0].sum > 21) {
            num_busts++;
        }
        for (int i = player_hands.size() - 1; i >= 0; i--)
        {                                      // player_earnings[0] will be nonzero but thats fine
            Hand &curr_hand = player_hands[i]; // might wanna change this back to non-reference
            player_earnings[curr_hand.player_inx] += player_wins(curr_hand) * curr_hand.bet;
            curr_hand.soft = false;
            curr_hand.sum = 0;
            curr_hand.bet = 1;
        }
        rounds_simulated++;
    }
}

int Simulator::player_wins(Hand &player_hand)
{
    Hand &dealer_hand = player_hands[0];
    if (player_hand.sum > 21)
    {
        return -1;
    }
    if (dealer_hand.sum > 21)
    {
        return 1;
    }
    if (player_hand.sum < 12 && player_hand.soft)
    {
        player_hand.sum += 10;
    }
    if (dealer_hand.sum < 12 && dealer_hand.soft)
    {
        dealer_hand.sum += 10;
    }
    if (dealer_hand.sum > player_hand.sum)
    {
        return -1;
    }
    else
    {
        return (player_hand.sum > dealer_hand.sum);
    }
}

int Simulator::draw_card()
{
    if (deck_size < 62)
    {
        // we should probably make it so one can only play with at most 4 hands at a time
        deck_size = 312;
        for (int card : {9, 8, 7, 6, 5, 4, 3, 2, 1})
        {
            deck[card] = 24;
        }
        deck[10] = 96;
    }
    int card = int(dis(gen) * deck_size);
    int inx = 0;
    while (card >= deck[inx])
    {
        card -= deck[inx++];
    }
    deck[inx]--;
    deck_size--;
    return inx;
}

void Simulator::print_statistics()
{
    std::string cent = "\xC2\xA2";
    for (int i = 1; i < player_earnings.size(); i++)
    {
        int num_of_that_player = 0;
        for (int j = 1; j < player_hands.size(); j++)
        {
            int x = player_hands[i].player_inx;
            if (x == i)
            {
                num_of_that_player++;
            }
        }
        if (player_earnings[i] < 0)
        {
            std::cout << "\n\nPlayer " << i << ": -" << std::setprecision(4) << -100.0 * player_earnings[i] / rounds_simulated / num_of_that_player << cent << " per round\n\n";
        }
        else
        {
            std::cout << "\n\nPlayer " << i << ": " << std::setprecision(4) << 100.0 * player_earnings[i] / rounds_simulated / num_of_that_player << cent << " per round\n\n";
        }
    }
    std::cout << "Dealer Bust Rate: " << 100.0*double(num_busts)/rounds_simulated << "%\n\n";
}