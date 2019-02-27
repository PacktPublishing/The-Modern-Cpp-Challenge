#include <fstream>
#include <string>

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

void remove_empty_lines(fs::path filepath)
{
   std::ifstream filein(filepath.native(), std::ios::in);
   if (!filein.is_open())
      throw std::runtime_error("cannot open input file");

   auto temppath = fs::temp_directory_path() / "temp.txt";
   std::ofstream fileout(temppath.native(), std::ios::out | std::ios::trunc);
   if (!fileout.is_open())
      throw std::runtime_error("cannot create temporary file");

   std::string line;
   while (std::getline(filein, line))
   {
      if (line.length() > 0 &&
         line.find_first_not_of(' ') != line.npos)
      {
         fileout << line << '\n';
      }
   }

   filein.close();
   fileout.close();

   fs::remove(filepath);
   fs::rename(temppath, filepath);
}

int main()
{
   remove_empty_lines("sample34.txt");
}
