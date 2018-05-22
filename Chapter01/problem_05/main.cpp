#include <iostream>

bool is_prime(int const num)
{
   if (num <= 3)
   {
      return num > 1;
   }
   else if (num % 2 == 0 || num % 3 == 0)
   {
      return false;
   }
   else
   {
      for (int i = 5; i * i <= num; i += 6)
      {
         if (num % i == 0 || num % (i + 2) == 0)
         {
            return false;
         }
      }

      return true;
   }
}

int main()
{
   int limit = 0;
   std::cout << "Upper limit:";
   std::cin >> limit;

   for (int n = 2; n <= limit; n++)
   {
      if (is_prime(n) && is_prime(n + 6))
      {
         std::cout << n << "," << n + 6 << std::endl;
      }
   }
}
