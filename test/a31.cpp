#include <string>
#include <string_view>
#include <regex>
#include <assert.h>

std::string transform_date(std::string_view text)
{
   auto rx = std::regex{ R"((\d{1,2})(\.|-|/)(\d{1,2})(\.|-|/)(\d{4}))" };
   return std::regex_replace(text.data(), rx, R"($5-$3-$1)");
}

int main()
{
   using namespace std::string_literals;

   assert(
      transform_date("today is 01.12.2017!"s) == 
      "today is 2017-12-01!"s);
}
