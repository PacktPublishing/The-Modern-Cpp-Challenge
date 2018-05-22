#include <string>
#include <sstream>
#include <vector>
#include <assert.h>

template <class Elem>
using tstring = std::basic_string<Elem, std::char_traits<Elem>, std::allocator<Elem>>;

template <class Elem>
using tstringstream = std::basic_stringstream<Elem, std::char_traits<Elem>, std::allocator<Elem>>;

template<typename Elem>
inline std::vector<tstring<Elem>> split(tstring<Elem> text, Elem const delimiter)
{
   auto sstr = tstringstream<Elem>{ text };
   auto tokens = std::vector<tstring<Elem>>{};
   auto token = tstring<Elem>{};
   while (std::getline(sstr, token, delimiter))
   {
      if (!token.empty()) tokens.push_back(token);
   }

   return tokens;
}

template<typename Elem>
inline std::vector<tstring<Elem>> split(tstring<Elem> text, tstring<Elem> const & delimiters)
{
   auto tokens = std::vector<tstring<Elem>>{};

   size_t pos, prev_pos = 0;
   while ((pos = text.find_first_of(delimiters, prev_pos)) != std::string::npos)
   {
      if (pos > prev_pos)
         tokens.push_back(text.substr(prev_pos, pos - prev_pos));
      prev_pos = pos + 1;
   }

   if (prev_pos< text.length())
      tokens.push_back(text.substr(prev_pos, std::string::npos));

   return tokens;
}

int main()
{
   using namespace std::string_literals;

   std::vector<std::string> expected{"this", "is", "a", "sample"};

   assert(expected == split("this is a sample"s, ' '));
   assert(expected == split("this,is a.sample!!"s, ",.! "s));
}
