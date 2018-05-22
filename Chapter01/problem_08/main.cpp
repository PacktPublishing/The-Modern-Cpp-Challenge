#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <chrono>

template <typename Time = std::chrono::microseconds,
   typename Clock = std::chrono::high_resolution_clock>
   struct perf_timer
{
   template <typename F, typename... Args>
   static Time duration(F&& f, Args... args)
   {
      auto start = Clock::now();

      std::invoke(std::forward<F>(f), std::forward<Args>(args)...);

      auto end = Clock::now();

      return std::chrono::duration_cast<Time>(end - start);
   }
};

void print_narcissistics_1(bool const printResults)
{
   for (int a = 1; a <= 9; a++)
   {
      for (int b = 0; b <= 9; b++)
      {
         for (int c = 0; c <= 9; c++)
         {
            auto abc = a * 100 + b * 10 + c;
            auto arm = a * a * a + b * b * b + c * c * c;
            if (abc == arm)
            {
               if (printResults) std::cout << arm << std::endl;
            }
         }
      }
   }
}

void print_narcissistics_2(bool const printResults)
{
   for (int i = 100; i <= 1000; ++i)
   {
      int arm = 0;
      int n = i;
      while (n > 0)
      {
         auto d = n % 10;
         n = n / 10;
         arm += d*d*d;
      }

      if (i == arm)
      {
         if (printResults) std::cout << arm << std::endl;
      }
   }
}

void print_narcissistics_3(int const limit, bool const printResults)
{
   for (int i = 1; i <= limit; ++i)
   {
      std::vector<int> digits;
      int n = i;
      while (n > 0)
      {
         digits.push_back(n % 10);
         n = n / 10;
      }

      int arm = std::accumulate(
         std::begin(digits), std::end(digits),
         0,
         [s = digits.size()](int const sum, int const digit) {return sum + static_cast<int>(std::pow(digit, s)); });

      if (i == arm)
      {
         if (printResults) std::cout << arm << std::endl;
      }
   }
}

int main()
{
   print_narcissistics_1(true);
   print_narcissistics_2(true);
   print_narcissistics_3(1000, true);

   auto t1 = perf_timer<>::duration([]() { for (int i = 0; i < 10000; ++i) print_narcissistics_1(false); });
   std::cout << std::chrono::duration<double, std::milli>(t1).count() << "ms" << std::endl;

   auto t2 = perf_timer<>::duration([]() { for (int i = 0; i < 10000; ++i) print_narcissistics_2(false); });
   std::cout << std::chrono::duration<double, std::milli>(t2).count() << "ms" << std::endl;

   auto t3 = perf_timer<>::duration([]() { for (int i = 0; i < 10000; ++i) print_narcissistics_3(1000, false); });
   std::cout << std::chrono::duration<double, std::milli>(t3).count() << "ms" << std::endl;
}
