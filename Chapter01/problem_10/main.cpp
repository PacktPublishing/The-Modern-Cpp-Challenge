#include <iostream>
#include <bitset>
#include <string>

unsigned int gray_encode(unsigned int const num)
{
   return num ^ (num >> 1);
}

unsigned int gray_decode(unsigned int gray)
{
   for (unsigned int bit = 1U << 31; bit > 1; bit >>= 1)
   {
      if (gray & bit) gray ^= bit >> 1;
   }
   return gray;
}

std::string to_binary(unsigned int value, int const digits)
{
   return std::bitset<32>(value).to_string().substr(32-digits, digits);
}

int main()
{
   std::cout << "Number\tBinary\tGray\tDecoded\n";
   std::cout << "------\t------\t----\t-------\n";

   for (unsigned int n = 0; n < 32; ++n)
   {
      auto encg = gray_encode(n);
      auto decg = gray_decode(encg);

      std::cout 
         << n << "\t" << to_binary(n, 5) << "\t" 
         << to_binary(encg, 5) << "\t" << decg << "\n";
   }
}
