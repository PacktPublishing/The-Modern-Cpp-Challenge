#include <string>
#include <string_view>
#include <regex>
#include <assert.h>

#ifdef USE_BOOST_OPTIONAL
#  include <boost/optional.hpp>
using boost::optional;
#else
#  include <optional>
using std::optional;
#endif

struct uri_parts
{
   std::string           protocol;
   std::string           domain;
   optional<int>         port;
   optional<std::string> path;
   optional<std::string> query;
   optional<std::string> fragment;
};

optional<uri_parts> parse_uri(std::string uri)
{
   std::regex rx(R"(^(\w+):\/\/([\w.-]+)(:(\d+))?([\w\/\.]+)?(\?([\w=&]*)(#?(\w+))?)?$)");
   auto matches = std::smatch{};

   if (std::regex_match(uri, matches, rx))
   {
      if (matches[1].matched && matches[2].matched)
      {
         uri_parts parts;
         parts.protocol = matches[1].str();
         parts.domain = matches[2].str();
         if(matches[4].matched)
            parts.port = std::stoi(matches[4]);
         if (matches[5].matched)
            parts.path = matches[5];
         if (matches[7].matched)
            parts.query = matches[7];
         if (matches[9].matched)
            parts.fragment = matches[9];

         return parts;
      }
   }

   return {};
}

int main()
{
   auto p1 = parse_uri("https://packt.com");
   assert(p1);
   assert(p1->protocol == "https");
   assert(p1->domain == "packt.com");
   assert(!p1->port);
   assert(!p1->path);
   assert(!p1->query);
   assert(!p1->fragment);

   auto p2 = parse_uri("https://bbc.com:80/en/index.html?lite=true#ui");
   assert(p2);
   assert(p2->protocol == "https");
   assert(p2->domain == "bbc.com");
   assert(p2->port == 80);
   assert(p2->path.value() == "/en/index.html");
   assert(p2->query.value() == "lite=true");
   assert(p2->fragment.value() == "ui");
}
