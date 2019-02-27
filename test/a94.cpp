#include <iostream>
#include <string>
#include <assert.h>

#include "rsa.h"
#include "aes.h"
#include "hex.h"
#include "files.h"
#include "osrng.h"

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

void encode(
   fs::path const & filepath,
   CryptoPP::BufferedTransformation const & bt)
{
   CryptoPP::FileSink file(filepath.c_str());

   bt.CopyTo(file);
   file.MessageEnd();
}

void encode_private_key(
   fs::path const & filepath,
   CryptoPP::RSA::PrivateKey const & key)
{
   CryptoPP::ByteQueue queue;
   key.DEREncodePrivateKey(queue);

   encode(filepath, queue);
}

void encode_public_key(
   fs::path const & filepath,
   CryptoPP::RSA::PublicKey const & key)
{
   CryptoPP::ByteQueue queue;
   key.DEREncodePublicKey(queue);

   encode(filepath, queue);
}

void decode(
   fs::path const & filepath,
   CryptoPP::BufferedTransformation& bt)
{
   CryptoPP::FileSource file(filepath.c_str(), true);

   file.TransferTo(bt);
   bt.MessageEnd();
}

void decode_private_key(
   fs::path const & filepath,
   CryptoPP::RSA::PrivateKey& key)
{
   CryptoPP::ByteQueue queue;

   decode(filepath, queue);
   key.BERDecodePrivateKey(queue, false, queue.MaxRetrievable());
}

void decode_public_key(
   fs::path const & filepath,
   CryptoPP::RSA::PublicKey& key)
{
   CryptoPP::ByteQueue queue;

   decode(filepath, queue);
   key.BERDecodePublicKey(queue, false, queue.MaxRetrievable());
}

void rsa_sign_file(
   fs::path const & filepath,
   fs::path const & privateKeyPath,
   fs::path const & signaturePath,
   CryptoPP::RandomNumberGenerator& rng)
{
   CryptoPP::RSA::PrivateKey privateKey;
   decode_private_key(privateKeyPath, privateKey);

   CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

   CryptoPP::FileSource fileSource(
      filepath.c_str(),
      true,
      new CryptoPP::SignerFilter(
         rng,
         signer,
         new CryptoPP::FileSink(
            signaturePath.c_str())));
}

bool rsa_verify_file(
   fs::path const & filepath,
   fs::path const & publicKeyPath,
   fs::path const & signaturePath)
{
   CryptoPP::RSA::PublicKey publicKey;
   decode_public_key(publicKeyPath.c_str(), publicKey);

   CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);

   CryptoPP::FileSource signatureFile(
      signaturePath.c_str(),
      true);

   if (signatureFile.MaxRetrievable() != verifier.SignatureLength())
      return false;

   CryptoPP::SecByteBlock signature(verifier.SignatureLength());
   signatureFile.Get(signature, signature.size());

   auto* verifierFilter = new CryptoPP::SignatureVerificationFilter(verifier);
   verifierFilter->Put(signature, verifier.SignatureLength());

   CryptoPP::FileSource fileSource(
      filepath.c_str(),
      true,
      verifierFilter);

   return verifierFilter->GetLastResult();
}

void generate_keys(
   fs::path const & privateKeyPath,
   fs::path const & publicKeyPath,
   CryptoPP::RandomNumberGenerator& rng)
{
   try
   {
      CryptoPP::RSA::PrivateKey rsaPrivate;
      rsaPrivate.GenerateRandomWithKeySize(rng, 3072);

      CryptoPP::RSA::PublicKey rsaPublic(rsaPrivate);

      encode_private_key(privateKeyPath, rsaPrivate);
      encode_public_key(publicKeyPath, rsaPublic);
   }
   catch (CryptoPP::Exception const & e)
   {
      std::cerr << e.what() << std::endl;
   }
}

int main()
{
   CryptoPP::AutoSeededRandomPool rng;

   generate_keys(
      "rsa-private.key",
      "rsa-public.key",
      rng);

   rsa_sign_file(
      "sample.txt",
      "rsa-private.key",
      "sample.sign",
      rng);

   auto success = rsa_verify_file(
      "sample.txt",
      "rsa-public.key",
      "sample.sign");

   assert(success);
}
