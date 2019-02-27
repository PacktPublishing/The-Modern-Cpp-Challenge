#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#ifdef USE_BOOST_FILESYSTEM
#  include <boost/filesystem/path.hpp>
#  include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;
#else
#  include <filesystem>
#  ifdef FILESYSTEM_EXPERIMENTAL
namespace fs = std::experimental::filesystem;
#  else
namespace fs = std::filesystem;
#  endif
#endif

#ifdef USE_BOOST_OPTIONAL
#  include <boost/optional.hpp>
using boost::optional;
#else
#  include <optional>
using std::optional;
#endif

#include "sqlite3.h"

#define MODERN_SQLITE_STD_OPTIONAL_SUPPORT
#include "sqlite_modern_cpp.h"
#include "movies.h"

movie_list get_movies(std::string_view title, sqlite::database & db)
{
   movie_list movies;

   db << R"(select rowid, * from movies where title=?;)"
      << title.data()
      >> [&movies, &db](sqlite3_int64 const rowid, std::string const & title,
         int const year, int const length)
   {
      movies.emplace_back(movie{
         static_cast<unsigned int>(rowid),
         title,
         year,
         static_cast<unsigned int>(length),
         {},
         {},
         {}
         });
   };

   return movies;
}

bool add_media(sqlite_int64 const movieid,
               std::string_view name,
               std::string_view description,
               std::vector<char> content,
               sqlite::database & db)
{
   try
   {
      db << "insert into media values(?,?,?,?)"
         << movieid
         << name.data()
         << description.data()
         << content;
         
      return true;
   }
   catch (...) { return false; }
}

media_list get_media(sqlite_int64 const movieid,
                     sqlite::database & db)
{
   media_list list;

   db << "select rowid, * from media where movieid = ?;"
      << movieid
      >> [&list](sqlite_int64 const rowid, 
            sqlite_int64 const movieid, 
            std::string const & name,
#ifdef USE_BOOST_OPTIONAL
            std::unique_ptr<std::string> const text,
#else
            optional<std::string> const text,
#endif
            std::vector<char> const & blob
         )
         {
      list.emplace_back(media{
         static_cast<unsigned int>(rowid),
         static_cast<unsigned int>(movieid),
         name,
#ifdef USE_BOOST_OPTIONAL
               text != nullptr ? *text : optional<std::string>{},
#else
               text,
#endif
               blob});
         };

   return list;
}

bool delete_media(sqlite_int64 const mediaid,
                  sqlite::database & db)
{
   try
   {
      db << "delete from media where rowid = ?;"
         << mediaid;

      return true;
   }
   catch (...) { return false; }
}

std::vector<std::string> split(std::string text, char const delimiter)
{
   auto sstr = std::stringstream{ text };
   auto tokens = std::vector<std::string>{};
   auto token = std::string{};
   while (std::getline(sstr, token, delimiter))
   {
      if (!token.empty()) tokens.push_back(token);
   }
   return tokens;
}

inline bool starts_with(std::string_view text, std::string_view part)
{
   return text.find(part) == 0;
}

inline std::string trim(std::string_view text)
{
   auto first{ text.find_first_not_of(' ') };
   auto last{ text.find_last_not_of(' ') };
   return text.substr(first, (last - first + 1)).data();
}

std::vector<char> load_image(std::string_view filepath)
{
   std::vector<char> data;

   std::ifstream ifile(filepath.data(), std::ios::binary | std::ios::ate);
   if (ifile.is_open())
   {
      auto size = ifile.tellg();
      ifile.seekg(0, std::ios::beg);

      data.resize(static_cast<size_t>(size));
      ifile.read(reinterpret_cast<char*>(data.data()), size);
   }

   return data;
}

void run_find(std::string_view line, sqlite::database & db)
{
   auto title = trim(line.substr(5));

   auto movies = get_movies(title, db);
   if(movies.empty())
      std::cout << "empty" << std::endl;      
   else
   {
      for (auto const m : movies)
      {
         std::cout
            << m.id << " | "
            << m.title << " | "
            << m.year << " | "
            << m.length << "min"
            << std::endl;
      }
   }      
}

void run_list(std::string_view line, sqlite::database & db)
{
   auto movieid = std::stoi(trim(line.substr(5)));
   if (movieid > 0)
   {
      auto list = get_media(movieid, db);
      if (list.empty())
      {
         std::cout << "empty" << std::endl;
      }
      else
      {
         for (auto const & m : list)
         {
            std::cout
               << m.id << " | "
               << m.movie_id << " | "
               << m.name << " | "
               << m.text.value_or("(null)") << " | "
               << m.blob.size() << " bytes"
               << std::endl;
         }
      }
   }
   else
      std::cout << "input error" << std::endl;
}

void run_add(std::string_view line, sqlite::database & db)
{
   auto parts = split(trim(line.substr(4)), ',');
   if (parts.size() == 3)
   {
      auto movieid = std::stoi(parts[0]);
      auto path = fs::path{parts[1]};
      auto desc = parts[2];

      auto content = load_image(parts[1]);
      auto name = path.filename().string();

      auto success = add_media(movieid, name, desc, content, db);
      if (success)
         std::cout << "added" << std::endl;
      else
         std::cout << "failed" << std::endl;
   }
   else
      std::cout << "input error" << std::endl;
}

void run_del(std::string_view line, sqlite::database & db)
{
   auto mediaid = std::stoi(trim(line.substr(4)));
   if (mediaid > 0)
   {
      auto success = delete_media(mediaid, db);
      if (success)
         std::cout << "deleted" << std::endl;
      else
         std::cout << "failed" << std::endl;
   }
   else
      std::cout << "input error" << std::endl;
}

void print_commands()
{
   std::cout
      << "find <title>                        finds a movie ID\n"
      << "list <movieid>                      lists the images of a movie\n"
      << "add <movieid>,<path>,<description>  adds a new image\n"
      << "del <imageid>                       delete an image\n"
      << "help                                shows available commands\n"
      << "exit                                exists the application\n";
}

int main()
{
   try
   {
      sqlite::database db(R"(cppchallenger87.db)");

      while (true)
      {
         std::string line;
         std::getline(std::cin, line);

         if (line == "help") print_commands();
         else if (line == "exit") break;
         else
         {
            if (starts_with(line, "find"))
               run_find(line, db);
            else if (starts_with(line, "list"))
               run_list(line, db);
            else if (starts_with(line, "add"))
               run_add(line, db);
            else if (starts_with(line, "del"))
               run_del(line, db);
            else
               std::cout << "unknown command" << std::endl;
         }

         std::cout << std::endl;
      }
   }
   catch (sqlite::sqlite_exception const & e)
   {
      std::cerr << e.get_code() << ": " << e.what() << " during "
                << e.get_sql() << std::endl;
   }
   catch (std::exception const & e)
   {
      std::cerr << e.what() << std::endl;
   }
}
