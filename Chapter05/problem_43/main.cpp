#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <iomanip>
#include "date.h"
#include "tz.h"

namespace ch = std::chrono;

struct user
{
   std::string             Name;
   date::time_zone const * Zone;

   user(std::string_view name, std::string_view zone)
      : Name{name.data()}, Zone(date::locate_zone(zone.data()))
   {      
   }
};

template <class Duration, class TimeZonePtr>
void print_meeting_times(
   date::zoned_time<Duration, TimeZonePtr> const & time,
   std::vector<user> const & users)
{
   std::cout 
      << std::left << std::setw(15) << std::setfill(' ')
      << "Local time: " 
      << time << std::endl;

   for (auto const & user : users)
   {
      std::cout
         << std::left << std::setw(15) << std::setfill(' ')
         << user.Name
         << date::zoned_time<Duration, TimeZonePtr>(user.Zone, time) 
         << std::endl;
   }
}

int main()
{
   std::vector<user> users{
      user{ "Ildiko", "Europe/Budapest" },
      user{ "Jens", "Europe/Berlin" },
      user{ "Jane", "America/New_York" }
   };

   unsigned int h, m;
   std::cout << "Hour:"; std::cin >> h;
   std::cout << "Minutes:"; std::cin >> m;

   date::year_month_day today = date::floor<date::days>(ch::system_clock::now());

   auto localtime = date::zoned_time<std::chrono::minutes>(
      date::current_zone(), 
      static_cast<date::local_days>(today) + ch::hours{ h } +ch::minutes{ m });

   print_meeting_times(localtime, users);
}
