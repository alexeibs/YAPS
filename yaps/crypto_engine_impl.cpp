#include "crypto_engine_impl.h"

#include <QString>

#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/twofish.h>

namespace yaps {
namespace {

const unsigned int YAPS_KEY_SIZE = CryptoPP::SHA256::DIGESTSIZE;
const unsigned int YAPS_NOISE_SIZE = 4096;
typedef byte Key[YAPS_KEY_SIZE];

const int GENERATED_PASSWORD_SIZE = 4;

volatile int g_volatileInt = '0';

int volatileInt() {
  if (++g_volatileInt > '9')
    g_volatileInt = '0';
  return g_volatileInt;
}

void eraseKey(Key& key) {
  memset((void*)&key[0], volatileInt(), YAPS_KEY_SIZE);
}

template<typename StringType>
void eraseString(StringType& s) {
  for (int i = 0, imax = s.size(); i < imax; i++)
    s[i] = volatileInt();
  s.clear();
}

void makeKeyFromPassword(const QString& password, Key& key) {
  QByteArray passwordUtf8 = password.toUtf8();
  CryptoPP::SHA256 hash;
  hash.CalculateDigest(key, (const byte*)passwordUtf8.data(), passwordUtf8.size());
}

void surroundWithNoise(std::string& s, size_t outputSize) {
  if (s.size() + 1 >= outputSize)
    return;
  size_t freeSpace = outputSize - s.size();

  // generate random noise
  std::string buffer(outputSize, '\0');
  CryptoPP::AutoSeededRandomPool randomPool;
  randomPool.GenerateBlock((byte*)&buffer[0], outputSize);
  // replace all zero bytes
  for (size_t i = 0; i < outputSize; ++i) {
    while (buffer[i] == '\0')
      buffer[i] = (char)randomPool.GenerateByte();
  }

  // find random position for string 's'
  size_t randomNumber;
  randomPool.GenerateBlock((byte*)&randomNumber, sizeof(randomNumber));
  size_t right = randomNumber % (freeSpace - 1) + 1;
  size_t left = freeSpace - right;

  // copy s into buffer and put zero delimiters
  memcpy(&buffer[left], &s[0], s.size());
  buffer[outputSize - right] = '\0';
  if (left > 0)
    buffer[left - 1] = '\0';

  // replace s by buffer and erase original string
  buffer.swap(s);
  eraseString(buffer);
}

void cleanStringFromNoise(std::string& s) {
  size_t right = s.find_last_of('\0');
  if (right == std::string::npos || right == 0)
    return;

  size_t left = s.find_last_of('\0', right - 1);
  if (left == std::string::npos)
    left = 0;

  if (right == left + 1)
    return;

  std::string buffer = s.substr(left + 1, right - left - 1);
  buffer.swap(s);
  eraseString(buffer);
}

void twofishEncrypt(const QString& input, QString& output, const Key& key) {
  std::string inputStd = input.toStdString();
  surroundWithNoise(inputStd, YAPS_NOISE_SIZE);
  std::string outputStd;

  byte iv[CryptoPP::Twofish::BLOCKSIZE];
  memset(iv, 0x01, CryptoPP::Twofish::BLOCKSIZE);

  CryptoPP::CFB_Mode<CryptoPP::Twofish>::Encryption cipher(key, YAPS_KEY_SIZE, iv);
  CryptoPP::StringSource(inputStd, true,
    new CryptoPP::StreamTransformationFilter(cipher,
      new CryptoPP::Base64Encoder(
        new CryptoPP::StringSink(outputStd), false
      ) // Base64Encoder
    ) // StreamTransformationFilter
  ); // StringSource

  eraseString(output);
  output = std::move(QString::fromStdString(outputStd));
  eraseString(inputStd);
  eraseString(outputStd);
}

void twofishDecrypt(const QString& input, QString& output, const Key& key) {
  std::string inputStd = input.toStdString();
  std::string outputStd;

  byte iv[CryptoPP::Twofish::BLOCKSIZE];
  memset(iv, 0x01, CryptoPP::Twofish::BLOCKSIZE);

  CryptoPP::CFB_Mode<CryptoPP::Twofish>::Decryption cipher(key, YAPS_KEY_SIZE, iv);
  CryptoPP::StringSource(inputStd, true,
    new CryptoPP::Base64Decoder(
      new CryptoPP::StreamTransformationFilter(cipher,
        new CryptoPP::StringSink(outputStd)
      ) // StreamTransformationFilter
    ) // Base64Decoder
  ); // StringSource

  cleanStringFromNoise(outputStd);
  eraseString(output);
  output = std::move(QString::fromStdString(outputStd));
  eraseString(inputStd);
  eraseString(outputStd);
}

//static void testTwofish(const QString& password, const QString& message)
//{
//    Key key, key2;
//    makeKeyFromPassword(password, key);
//    memcpy(key2, key, sizeof(key));
//    QString encrypted, decrypted;
//    twofishEncrypt(message, encrypted, key);
//    twofishDecrypt(encrypted, decrypted, key2);
//    qDebug() << message;
//    qDebug() << encrypted;
//    qDebug() << decrypted;
//}

void generateRandomPassword(QString& password, int length) {
  // Generate a random block
  CryptoPP::AutoSeededRandomPool randomPool;
  CryptoPP::SecByteBlock block(length * 3);
  randomPool.GenerateBlock(block, block.size());

  // make password using base64
  std::string output;
  CryptoPP::Base64Encoder base64(new CryptoPP::StringSink(output), false);
  base64.Put(block, block.size());
  base64.MessageEnd();

  password = std::move(QString::fromStdString(output));
  eraseString(output);
  password.replace('+', '-');
  password.replace('/', '_');
}

} // namespace

void CryptoEngineImpl::encrypt(const QString& password, const QString& input, QString& output) {
  Key key;
  makeKeyFromPassword(password, key);
  twofishEncrypt(input, output, key);
  eraseKey(key);
}

void CryptoEngineImpl::decrypt(const QString& password, const QString& input, QString& output) {
  Key key;
  makeKeyFromPassword(password, key);
  twofishDecrypt(input, output, key);
  eraseKey(key);
}

void CryptoEngineImpl::generatePassword(QString& password) {
  generateRandomPassword(password, GENERATED_PASSWORD_SIZE);
}

void eraseString(QString& stringToErase) {
  eraseString<QString>(stringToErase);
}

}  // namespace yaps
