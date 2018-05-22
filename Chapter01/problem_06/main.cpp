#include <iostream>
#include <cmath>

int sum_proper_divisors(int const number)
{
   int result = 1;
   for (int i = 2; i <= std::sqrt(number); i++)
   {
      if (number%i == 0)
      {
         result += (i == (number / i)) ? i : (i + number / i);
      }
   }

   return result;
}

void print_abundant(int const limit)
{
   for (int number = 10; number <= limit; ++number)
   {
      auto sum = sum_proper_divisors(number);
      if (sum > number)
      {
         std::cout
            << number
            << ", abundance=" << sum - number << std::endl;
      }
   }
}

int main()
{
   int limit = 0;
   std::cout << "Upper limit:";
   std::cin >> limit;

   print_abundant(limit);
}
