#pragma once
#include <unordered_map>
#include <vector>
class Player {
 public:
  struct Hand {
    int sum;
    bool soft;
    bool splittable;
  };
  enum Choice { HIT, STAND, DOUBLE, SPLIT };

 private:
  int HandSum(Hand& hand);
  double EvaluateHands(Hand& hand, Hand& dealer_hand);

 public:
  double calculate_value_on_hit(Hand& hand, Hand& dealer_hand,
                                std::vector<int>& deck, int max_hits = 200,
                                bool split_after = false);
  double calculate_value_on_stand(Hand& hand, Hand& dealer_hand,
                                  std::vector<int>& deck);
  double calculate_value_on_double(Hand& hand, Hand& dealer_hand,
                                   std::vector<int>& deck);
  double calculate_value_on_split(Hand& hand, Hand& dealer_hand,
                                  std::vector<int>& deck);
  Choice get_choice(Hand& hand, Hand& dealer_hand, std::vector<int>& deck);
  std::pair<Choice, double> get_choice_with_prob(Hand& hand, Hand& dealer_hand,
                                                 std::vector<int>& deck,
                                                 bool split_valid = true);
};