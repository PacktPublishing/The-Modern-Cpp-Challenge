#include <iostream>

unsigned int gcd(unsigned int const a, unsigned int const b)
{
   return b == 0 ? a : gcd(b, a % b);
}

unsigned int gcd2(unsigned int a, unsigned int b)
{
   while (b != 0)
   {
      unsigned int r = a % b;
      a = b;
      b = r;
   }

   return a;
}

int main()
{
   std::cout << "Input numbers:";

   unsigned int a, b;
   std::cin >> a >> b;

   std::cout
      << "rec gcd(" << a << ", " << b << ")="
      << gcd(a, b) << std::endl;

   std::cout
      << "    gcd(" << a << ", " << b << ")="
      << gcd2(a, b) << std::endl;
}
