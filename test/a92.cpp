#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "sha.h"
#include "md5.h"
#include "hex.h"
#include "files.h"

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

template <class Hash>
std::string compute_hash(fs::path const & filepath)
{
   std::ifstream file(filepath.string(), std::ios::binary);
   if (file.is_open())
   {
      Hash hash;
      CryptoPP::byte digest[Hash::DIGESTSIZE] = { 0 };

      do
      {
         char buffer[4096] = { 0 };
         file.read(buffer, 4096);

         auto extracted = static_cast<size_t>(file.gcount());

         if (extracted > 0)
         {
            hash.Update(
               reinterpret_cast<CryptoPP::byte*>(buffer),
               extracted);
         }
      } while (!file.fail());

      hash.Final(digest);

      CryptoPP::HexEncoder encoder;
      std::string result;

      encoder.Attach(new CryptoPP::StringSink(result));
      encoder.Put(digest, sizeof(digest));
      encoder.MessageEnd();

      return result;
   }

   throw std::runtime_error("Cannot open file!");
}

template <class Hash>
std::string compute_hash_ex(fs::path const & filepath)
{
   std::string digest;
   Hash hash;

   CryptoPP::FileSource source(
      filepath.c_str(),
      true,
      new CryptoPP::HashFilter(hash,
         new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(digest))));

   return digest;
}

int main()
{
   std::string path;
   std::cout << "Path: ";
   std::cin >> path;

   try
   {
      std::cout << "SHA1: "
         << compute_hash<CryptoPP::SHA1>(path) << std::endl;
      std::cout << "SHA256: "
         << compute_hash<CryptoPP::SHA256>(path) << std::endl;
      std::cout << "MD5: "
         << compute_hash<CryptoPP::Weak::MD5>(path) << std::endl;

      std::cout << "SHA1: "
         << compute_hash_ex<CryptoPP::SHA1>(path) << std::endl;
      std::cout << "SHA256: "
         << compute_hash_ex<CryptoPP::SHA256>(path) << std::endl;
      std::cout << "MD5: "
         << compute_hash_ex<CryptoPP::Weak::MD5>(path) << std::endl;
   }
   catch (std::exception const & ex)
   {
      std::cerr << ex.what() << std::endl;
   }
}
