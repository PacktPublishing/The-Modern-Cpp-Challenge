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


std::string build_vigenere_table()
{
   std::string table;
   table.reserve(26*26);

   for (int i = 0; i < 26; ++i)
      table += caesar_encrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZ", i);

   return table;
}

std::string vigenere_encrypt(std::string_view text, std::string_view key)
{
   std::string result;
   result.reserve(text.length());

   static auto table = build_vigenere_table();

   for (size_t i = 0; i < text.length(); ++i)
   {
      auto row = key[i%key.length()] - 'A';
      auto col = text[i] - 'A';

      result += table[row * 26 + col];
   }

   return result;
}

std::string vigenere_decrypt(std::string_view text, std::string_view key)
{
   std::string result;
   result.reserve(text.length());

   static auto table = build_vigenere_table();

   for (size_t i = 0; i < text.length(); ++i)
   {
      auto row = key[i%key.length()] - 'A';

      for (size_t col = 0; col < 26; col++)
      {
         if (table[row * 26 + col] == text[i])
         {
            result += 'A' + col;
            break;
         }
      }
   }

   return result;
}

int main()
{
   auto text = "THECPPCHALLENGER";
   auto enc = vigenere_encrypt(text, "SAMPLE");
   auto dec = vigenere_decrypt(enc, "SAMPLE");

   assert(text == dec);
}
