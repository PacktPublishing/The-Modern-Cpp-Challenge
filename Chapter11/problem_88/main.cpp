#include <iostream>
#include <string>
#include <string_view>
#include <assert.h>

std::string caesar_encrypt(std::string_view text, int const shift)
{
   std::string str;
   str.reserve(text.length());
   for (auto const & c : text)
   {
      if (isalpha(c) && isupper(c))
         str += 'A' + (c - 'A' + shift) % 26;
      else
         str += c;
   }

   return str;
}

std::string caesar_decrypt(std::string_view text, int const shift)
{
   std::string str;
   str.reserve(text.length());
   for (auto const & c : text)
   {
      if (isalpha(c) && isupper(c))
         str += 'A' + (26 + c - 'A' - shift) % 26;
      else
         str += c;
   }

   return str;
}

int main()
{
   auto text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   for (int i = 1; i <= 26; ++i)
   {
      auto enc = caesar_encrypt(text, i);
      auto dec = caesar_decrypt(enc, i);
      assert(text == dec);
   }
}
