#include <iostream>
#include <chrono>

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

namespace ch = std::chrono;

template <typename Duration>
bool is_older_than(fs::path const & path, Duration const duration)
{
   auto lastwrite = fs::last_write_time(path);
#ifdef USE_BOOST_FILESYSTEM
   auto ftimeduration = ch::system_clock::from_time_t(lastwrite).time_since_epoch();
#else
   auto ftimeduration = lastwrite.time_since_epoch();
#endif
   auto nowduration = (ch::system_clock::now() - duration).time_since_epoch();
   return ch::duration_cast<Duration>(nowduration - ftimeduration).count() > 0;
}

template <typename Duration>
void remove_files_older_than(fs::path const & path, Duration const duration)
{
   try
   {
      if (fs::exists(path))
      {
         if (is_older_than(path, duration))
         {
            fs::remove(path);
         }
         else if(fs::is_directory(path))
         {
            for (auto const & entry : fs::directory_iterator(path))
            {
               remove_files_older_than(entry.path(), duration);
            }
         }
      }
   }
   catch (std::exception const & ex)
   {
      std::cerr << ex.what() << std::endl;
   }
}

int main()
{
   using namespace std::chrono_literals;

#ifdef _WIN32
   auto path = R"(..\Test\)";
#else
   auto path = R"(../Test/)";
#endif

   remove_files_older_than(path, 1h + 20min);
}
