#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/RSA.h>

#include <sstream>

using namespace yabil::crypto;
using namespace yabil::bigint;

class RSA_tests : public ::testing::Test
{
};

TEST_F(RSA_tests, canGeneratePrivateAndPublicKey)
{
    const auto [private_key, pub_key] = rsa::generate_keys(256);

    ASSERT_TRUE(private_key.n == pub_key.n);
    ASSERT_FALSE(private_key.e.is_zero());
    ASSERT_FALSE(pub_key.d.is_zero());
}

TEST_F(RSA_tests, canEncryptSingleCharacter)
{
    const BigInt n{33}, e{7};
    const rsa::PublicKey pub_key{e, n};

    const char message = 'a';
    const auto encrypted = rsa::encrypt(message, pub_key);

    ASSERT_LT(encrypted, n);
    ASSERT_TRUE(encrypted.is_int64());
    ASSERT_EQ(encrypted.to_int(), 4);
}

TEST_F(RSA_tests, canEncryptMessage)
{
    const BigInt n{33}, e{7};
    const rsa::PublicKey pub_key{e, n};

    std::ostringstream os;
    rsa::EncryptionStreamWrapper encryption_stream(os, pub_key);
    encryption_stream << "This is message to encode: " << 120 << '.';

    ASSERT_NE(os.str().size(), 0);
}

TEST_F(RSA_tests, canDecryptEncryptedCharacter)
{
    const BigInt n{119}, e{5}, d{77};
    const rsa::PublicKey pub_key{e, n};
    const rsa::PrivateKey private_key{d, n};

    const std::string message = "abcdefgh";

    for (char c : message)
    {
        const auto encrypted = rsa::encrypt(c, pub_key);
        const auto decrypted = rsa::decrypt(encrypted, private_key);

        ASSERT_TRUE(decrypted.is_int64());
        ASSERT_EQ(c, decrypted.to_int());
    }
}

TEST_F(RSA_tests, canDecryptEncryptedMessage)
{
    const BigInt n{119}, e{5}, d{77};
    const rsa::PublicKey pub_key{e, n};
    const rsa::PrivateKey private_key{d, n};

    std::ostringstream os;
    rsa::EncryptionStreamWrapper encryption_stream(os, pub_key);

    const std::string msg = "This is message to encode:";
    encryption_stream << msg;

    std::istringstream in(os.str());
    rsa::DecryptionStreamWrapper decryption_stream(in, private_key);

    const std::string result = decryption_stream.read_all();
    ASSERT_EQ(result, msg);
}

TEST_F(RSA_tests, canEncryptAndDecryptFromDifferentSources)
{
    const BigInt n{119}, e{5}, d{77};
    const rsa::PublicKey pub_key{e, n};
    const rsa::PrivateKey private_key{d, n};

    std::ostringstream os;
    rsa::EncryptionStreamWrapper encryption_stream(os, pub_key);
    encryption_stream << "Message: " << 12 << ' ' << '.';

    std::istringstream in(os.str());
    rsa::DecryptionStreamWrapper decryption_stream(in, private_key);

    std::string message;
    int number;
    char chr;

    decryption_stream >> message >> number >> chr;
    ASSERT_EQ("Message:", message);
    ASSERT_EQ(12, number);
    ASSERT_EQ('.', chr);
}
