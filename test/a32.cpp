#include <string>
#include <iostream>
#include <cmath>

unsigned int number_of_digits(unsigned int const i)
{
   return i > 0 ? (int)log10((double)i) + 1 : 1;
}

void print_pascal_triangle(int const n)
{
   for (int i = 0; i < n; i++) 
   {
      auto x = 1;
      std::cout << std::string((n - i - 1)*(n / 2), ' ');
      for (int j = 0; j <= i; j++) 
      {
         auto y = x;
         x = x * (i - j) / (j + 1);
         auto maxlen = number_of_digits(x) - 1;
         std::cout << y << std::string(n - 1 - maxlen - n%2, ' ');
      }
      std::cout << std::endl;
   }
}

int main()
{
   int n = 0;
   std::cout << "Levels (up to 10): ";
   std::cin >> n;
   if (n > 10)
      std::cout << "Value too large" << std::endl;
   else
      print_pascal_triangle(n);
}
