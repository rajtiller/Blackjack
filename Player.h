#pragma once
#include <unordered_map>
#include <vector>
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

class Player {
 public:
  struct Hand {
    int sum;
    bool soft;
    bool splittable;
  };
  enum Choice { HIT, STAND, DOUBLE, SPLIT };
  class Cache {
   private:
    int hits = 0;
    int misses = 0;
    std::unordered_map<int, double> cache;
    void hash_combine(std::size_t& hash, const std::size_t& new_value) {
      hash ^= new_value + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

   public:
    double get_hit_rate() { return (double)hits / (double)(hits + misses); }
    int get_size() { return cache.size(); }
    int hash(Hand const& hand, Hand const& dealer_hand,
             std::vector<int> const& deck) {
      std::size_t hash = 8723;
      hash_combine(hash, hand.sum);
      hash_combine(hash, hand.soft);
      hash_combine(hash, dealer_hand.sum);
      hash_combine(hash, dealer_hand.soft);
      for (int card = 1; card <= 10; card++) {
        hash_combine(hash, deck.at(card));
      }
      return hash;
    }
    std::pair<bool, double> get_value(Hand& hand, Hand& dealer_hand,
                                      std::vector<int>& deck) {
      auto hash = Cache::hash(hand, dealer_hand, deck);
      // return std::make_pair(false, 0.0);
      if (cache.find(hash) == cache.end()) {
        misses++;
        return std::make_pair(false, 0.0);
      }
      hits++;
      return std::make_pair(true, cache.at(hash));
    }
    void set_value(Hand& hand, Hand& dealer_hand, std::vector<int>& deck,
                   double value) {
      auto hash = Cache::hash(hand, dealer_hand, deck);
      cache.insert({hash, value});
    }
    void clear() {
      hits = 0;
      misses = 0;
      cache.clear();
    }
  };
  Player::Cache cache;

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