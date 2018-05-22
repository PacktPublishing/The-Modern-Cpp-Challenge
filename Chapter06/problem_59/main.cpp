#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <random>
#include <algorithm>
#include <array>
#include <iomanip>

class weasel
{
   std::string target;
   std::uniform_int_distribution<> chardist;
   std::uniform_real_distribution<> ratedist;
   std::mt19937 mt;
   std::string const allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

public:
   weasel(std::string_view t) :
      target(t), 
      chardist(0, 26),
      ratedist(0, 100)
   {
      std::random_device rd;
      auto seed_data = std::array<int, std::mt19937::state_size> {};
      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
      mt.seed(seq);
   }

   void run(int const copies)
   {
      auto parent = make_random();
      int step = 1;
      std::cout << std::left << std::setw(5) << std::setfill(' ') << step << parent << std::endl;

      do
      {
         std::vector<std::string> children;
         std::generate_n(
            std::back_inserter(children), 
            copies, 
            [parent, this]() {return mutate(parent, 5); });

         parent = *std::max_element(
            std::begin(children), std::end(children),
            [this](std::string_view c1, std::string_view c2) {return fitness(c1) < fitness(c2); });

         std::cout << std::setw(5) << std::setfill(' ') << step << parent << std::endl;
         step++;

      } while (parent != target);
   }

private:
   weasel() = delete;

   double fitness(std::string_view candidate)
   {
      int score = 0;
      for (size_t i = 0; i < candidate.size(); ++i)
      {
         if (candidate[i] == target[i])
            score++;
      }

      return score;
   }

   std::string mutate(std::string_view parent, double const rate)
   {
      std::stringstream sstr;
      for (auto const c : parent)
      {
         auto nc = ratedist(mt) > rate ? c : allowed_chars[chardist(mt)];
         sstr << nc;
      }

      return sstr.str();
   }

   std::string make_random()
   {
      std::stringstream sstr;
      for (size_t i = 0; i < target.size(); ++i)
      {
         sstr << allowed_chars[chardist(mt)];
      }
      return sstr.str();
   }
};

int main()
{
   weasel w("METHINKS IT IS LIKE A WEASEL");
   w.run(100);
}
