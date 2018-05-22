#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

bool starts_with(std::string_view str, std::string_view prefix)
{
   return str.find(prefix) == 0;
}

void normalize_phone_numbers(
   std::vector<std::string>& numbers,
   std::string const & countryCode)
{
   std::transform(
      std::cbegin(numbers), std::cend(numbers),
      std::begin(numbers),
      [countryCode](std::string const & number) {
      std::string result;
      if (number.size() > 0)
      {
         if (number[0] == '0')
            result = "+" + countryCode + number.substr(1);
         else if (starts_with(number, countryCode))
            result = "+" + number;
         else if (starts_with(number, "+" + countryCode))
            result = number;
         else
            result = "+" + countryCode + number;
      }

      result.erase(
         std::remove_if(
            std::begin(result), std::end(result),
            [](const char ch) {return isspace(ch); }),
         std::end(result));

      return result;
   });
}

int main()
{
   std::vector<std::string> numbers{
      "07555 123456",
      "07555123456",
      "+44 7555 123456",
      "44 7555 123456",
      "7555 123456"
   };

   normalize_phone_numbers(numbers, "44");

   for (auto const & number : numbers)
   {
      std::cout << number << std::endl;
   }
}
