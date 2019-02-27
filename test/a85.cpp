#include <iostream>
#include <vector>

#include "sqlite3.h"
#include "sqlite_modern_cpp.h"
#include "movies.h"

void print_movie(movie const & m)
{
   std::cout << "[" << m.id << "] "
      << m.title << " (" << m.year << ") "
      << m.length << "min" << std::endl;
   std::cout << " directed by: ";
   for (auto const & d : m.directors) std::cout << d << ",";
   std::cout << std::endl;
   std::cout << " written by: ";
   for (auto const & w : m.writers) std::cout << w << ",";
   std::cout << std::endl;
   std::cout << " cast: ";
   for (auto const & r : m.cast) std::cout << r.actor << " (" << r.role << "),";
   std::cout << std::endl << std::endl;
}

std::vector<std::string> get_directors(sqlite3_int64 const movie_id, 
                                       sqlite::database & db)
{
   std::vector<std::string> result;
   db << R"(select p.name from directors as d 
            join persons as p on d.personid = p.rowid 
            where d.movieid = ?;)"
      << movie_id
      >> [&result](std::string const name)
   {
      result.emplace_back(name);
   };

   return result;
}

std::vector<std::string> get_writers(sqlite3_int64 const movie_id, 
                                     sqlite::database & db)
{
   std::vector<std::string> result;
   db << R"(select p.name from writers as w
         join persons as p on w.personid = p.rowid 
         where w.movieid = ?;)"
      << movie_id
      >> [&result](std::string const name)
   {
      result.emplace_back(name);
   };

   return result;
}

std::vector<casting_role> get_cast(sqlite3_int64 const movie_id, 
                                   sqlite::database & db)
{
   std::vector<casting_role> result;
   db << R"(select p.name, c.role from casting as c
         join persons as p on c.personid = p.rowid
         where c.movieid = ?;)"
      << movie_id
      >> [&result](std::string const name, std::string role)
   {
      result.emplace_back(casting_role{ name, role });
   };

   return result;
}

movie_list get_movies(sqlite::database & db)
{
   movie_list movies;

   db << R"(select rowid, * from movies;)"
      >> [&movies, &db](sqlite3_int64 const rowid, std::string const & title, 
                        int const year, int const length)
   {
      movies.emplace_back(movie{
         static_cast<unsigned int>(rowid),
         title,
         year,
         static_cast<unsigned int>(length),
         get_cast(rowid, db),
         get_directors(rowid, db),
         get_directors(rowid, db)
         });
   };

   return movies;
}

int main()
{
   try
   {
      sqlite::database db(R"(cppchallenger85.db)");

      auto movies = get_movies(db);
      for (auto const & m : movies)
         print_movie(m);
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
