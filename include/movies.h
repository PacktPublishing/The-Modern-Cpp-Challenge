#pragma once

#include <string>
#include <vector>

#ifdef USE_BOOST_OPTIONAL
#  include <boost/optional.hpp>
using boost::optional;
#else
#  include <optional>
using std::optional;
#endif

struct casting_role
{
   std::string actor;
   std::string role;
};

struct movie
{
   unsigned int   id;
   std::string    title;
   int            year;
   unsigned int   length;

   std::vector<casting_role>  cast;
   std::vector<std::string>   directors;
   std::vector<std::string>   writers;
};

struct media
{
   unsigned int            id;
   unsigned int            movie_id;
   std::string             name;
   optional<std::string>   text;
   std::vector<char>       blob;
};

using movie_list = std::vector<movie>;
using media_list = std::vector<media>;