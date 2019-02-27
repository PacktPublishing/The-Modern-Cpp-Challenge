#include <iostream>
#include <numeric>
#include <vector>

int gcd(int const a, int const b)
{
   return b == 0 ? a : gcd(b, a % b);
}

int lcm(int const a, int const b)
{
   int h = gcd(a, b);
   return h ? (a * (b / h)) : 0;
}

template<class InputIt>
int lcmr(InputIt first, InputIt last)
{
   return std::accumulate(first, last, 1, lcm);
}

int main()
{
   int n = 0;
   std::cout << "Input count:";
   std::cin >> n;

   std::vector<int> numbers;
   for (int i = 0; i < n; ++i)
   {
      int v{ 0 };
      std::cin >> v;
      numbers.push_back(v);
   }

   std::cout
      << "lcm="
      << lcmr(std::begin(numbers), std::end(numbers))
      << std::endl;
}
