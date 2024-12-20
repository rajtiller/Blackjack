#include "Player.h"
int Player::HandSum(Hand& hand) {
  if (hand.soft && hand.sum <= 10) {
    return hand.sum + 10;
  } else {
    return hand.sum;
  }
}
double Player::EvaluateHands(Hand& hand, Hand& dealer_hand) {
  int hand_count = HandSum(hand);
  int dealer_hand_count = HandSum(dealer_hand);
  if (hand_count > dealer_hand_count) {
    return 1;
  } else if (hand_count == dealer_hand_count) {
    return 0;
  } else {
    return -1;
  }
}
double Player::calculate_value_on_hit(Hand& hand, Hand& dealer_hand,
                                      std::vector<int>& deck, int max_hits,
                                      bool split_after) {
  // If it is 21 no reason to ever hit
  if (hand.sum >= 21) {
    return -1;
  }
  double ev = 0;
  int deck_count = 0;
  for (int card = 1; card <= 10; card++) {
    auto card_count = deck.at(card);
    if (card_count == 0) {
      continue;
    }
    hand.sum += card;
    bool changed = false;
    if (card == 1 && !hand.soft) {
      hand.soft = true;
      changed = true;
    }
    deck[card] -= 1;
    deck_count += card_count;
    double hit_ev = max_hits > 0 ? calculate_value_on_hit(hand, dealer_hand,
                                                          deck, max_hits - 1)
                                 : -1;
    double stand_ev = calculate_value_on_stand(hand, dealer_hand, deck);
    double split_ev =
        split_after ? calculate_value_on_split(hand, dealer_hand, deck) : -1;
    double double_ev =
        split_after ? calculate_value_on_double(hand, dealer_hand, deck) : -1;
    ev += std::max(std::max(hit_ev, stand_ev), std::max(split_ev, double_ev)) *
          card_count;
    deck[card] += 1;
    if (changed) {
      hand.soft = false;
    }
    hand.sum -= card;
  }
  if (deck_count == 0) {
    return EvaluateHands(hand, dealer_hand);
  }
  return ev / (double)deck_count;
}
double Player::calculate_value_on_stand(Hand& hand, Hand& dealer_hand,
                                        std::vector<int>& deck) {
  // Checks base cases
  if (hand.sum > 21) {
    return -1;
  }
  if (dealer_hand.sum > 21) {
    return 1;
  }
  // Dealer won already don't need to check further (Doesn't work because dealer
  // might still bust)
  // if (HandSum(dealer_hand) > HandSum(hand)) {
  //   return -1;
  // }
  if (HandSum(dealer_hand) > 17) {
    return EvaluateHands(hand, dealer_hand);
  }
  // If the dealer has a sum of 17 and it is not soft then dealer stands
  if (!dealer_hand.soft && dealer_hand.sum == 17) {
    return EvaluateHands(hand, dealer_hand);
  }
  // Checks for a tie
  if (HandSum(dealer_hand) == HandSum(hand)) {
    return 0;
  }
  double ev = 0;
  int deck_count = 0;
  for (int card = 1; card <= 10; card++) {
    auto card_count = deck.at(card);
    if (card_count == 0) {
      continue;
    }
    dealer_hand.sum += card;
    bool changed = false;
    if (card == 1 && !dealer_hand.soft) {
      dealer_hand.soft = true;
      changed = true;
    }
    deck[card] -= 1;
    deck_count += card_count;
    ev += calculate_value_on_stand(hand, dealer_hand, deck) * card_count;
    deck[card] += 1;
    if (changed) {
      dealer_hand.soft = false;
    }
    dealer_hand.sum -= card;
  }
  if (deck_count == 0) {
    return EvaluateHands(hand, dealer_hand);
  }
  return ev / (double)deck_count;
}
double Player::calculate_value_on_double(Hand& hand, Hand& dealer_hand,
                                         std::vector<int>& deck) {
  return calculate_value_on_hit(hand, dealer_hand, deck, 0) * 2;
}
double Player::calculate_value_on_split(Hand& hand, Hand& dealer_hand,
                                        std::vector<int>& deck) {
  if (!hand.splittable) {
    return -1;
  }
  hand.sum /= 2;
  double ev = get_choice_with_prob(hand, dealer_hand, deck, false).second * 2;
  hand.sum *= 2;
  return ev;
}
std::pair<Player::Choice, double> Player::get_choice_with_prob(
    Hand& hand, Hand& dealer_hand, std::vector<int>& deck, bool split_valid) {
  auto best = Choice::STAND;
  auto odds = calculate_value_on_stand(hand, dealer_hand, deck);
  auto new_odds =
      calculate_value_on_hit(hand, dealer_hand, deck, 200, !split_valid);
  if (new_odds > odds) {
    best = Choice::HIT;
    odds = new_odds;
  }
  if (split_valid) {
    new_odds = calculate_value_on_double(hand, dealer_hand, deck);
    if (new_odds > odds) {
      best = Choice::DOUBLE;
      odds = new_odds;
    }
    new_odds = calculate_value_on_split(hand, dealer_hand, deck);
    if (new_odds > odds) {
      best = Choice::SPLIT;
      odds = new_odds;
    }
  }
  return {best, odds};
}
Player::Choice Player::get_choice(Hand& hand, Hand& dealer_hand,
                                  std::vector<int>& deck) {
  return get_choice_with_prob(hand, dealer_hand, deck).first;
}