#include <iostream>
#include <string>
#include <string_view>
#include <fstream>

#include "ZipFile.h"
#include "utils/stream_utils.h"

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

void compress(
   fs::path const & source,
   fs::path const & archive,
   std::function<void(std::string_view)> reporter)
{
   if (fs::is_regular_file(source))
   {
      if (reporter) reporter("Compressing " + source.string());
      ZipFile::AddFile(archive.string(), source.string(), LzmaMethod::Create());
   }
   else
   {
      for (auto const & p : fs::recursive_directory_iterator(source))
      {
         if (reporter) reporter("Compressing " + p.path().string());

         if (fs::is_directory(p))
         {
            auto zipArchive = ZipFile::Open(archive.string());
            auto entry = zipArchive->CreateEntry(p.path().string());
            entry->SetAttributes(ZipArchiveEntry::Attributes::Directory);
            ZipFile::SaveAndClose(zipArchive, archive.string());
         }
         else if (fs::is_regular_file(p))
         {
            ZipFile::AddFile(archive.string(), p.path().string(), LzmaMethod::Create());
         }
      }
   }
}

void ensure_directory_exists(fs::path const & dir)
{
   if (!fs::exists(dir))
   {
#ifdef USE_BOOST_FILESYSTEM
      boost::system::error_code err;
#else
      std::error_code err;
#endif
      fs::create_directories(dir, err);
   }
}

void decompress(
   fs::path const & destination,
   fs::path const & archive,
   std::function<void(std::string_view)> reporter)
{
   ensure_directory_exists(destination);

   auto zipArchive = ZipFile::Open(archive.string());

   for (size_t i = 0; i < zipArchive->GetEntriesCount(); ++i)
   {
      auto entry = zipArchive->GetEntry(i);
      if (entry)
      {
         auto filepath = destination / fs::path{ entry->GetFullName() }.relative_path();
         if (reporter) reporter("Creating " + filepath.string());

         if (entry->IsDirectory())
         {
            ensure_directory_exists(filepath);
         }
         else
         {
            ensure_directory_exists(filepath.parent_path());

            std::ofstream destFile;
            destFile.open(
               filepath.string().c_str(),
               std::ios::binary | std::ios::trunc);

            if (!destFile.is_open())
            {
               if (reporter)
                  reporter("Cannot create destination file!");
            }

            auto dataStream = entry->GetDecompressionStream();
            if (dataStream)
            {
               utils::stream::copy(*dataStream, destFile);
            }
         }
      }
   }
}

int main()
{
   char option = 0;
   std::cout << "Select [c]ompress/[d]ecompress?";
   std::cin >> option;

   if (option == 'c')
   {
      std::string archivepath;
      std::string inputpath;
      std::cout << "Enter file or dir to compress:";
      std::cin >> inputpath;
      std::cout << "Enter archive path:";
      std::cin >> archivepath;

      compress(inputpath, archivepath, [](std::string_view message) {std::cout << message << std::endl; });
   }
   else if (option == 'd')
   {
      std::string archivepath;
      std::string outputpath;
      std::cout << "Enter dir to decompress:";
      std::cin >> outputpath;
      std::cout << "Enter archive path:";
      std::cin >> archivepath;

      decompress(outputpath, archivepath, [](std::string_view message) {std::cout << message << std::endl; });
   }
   else
   {
      std::cout << "invalid option" << std::endl;
   }
}
