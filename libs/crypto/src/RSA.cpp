#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/RSA.h>
#include <yabil/crypto/Random.h>
#include <yabil/math/Math.h>

#include <cstring>

namespace yabil::crypto::rsa
{

std::pair<PublicKey, PrivateKey> generate_keys(unsigned bits_for_key)
{
    const auto bits_per_prime = bits_for_key / 2;
    const auto p = yabil::crypto::random::random_prime(bits_per_prime);
    const auto q = yabil::crypto::random::random_prime(bits_per_prime);

    const auto n = p * q;
    const auto phi = (p - yabil::bigint::BigInt(1)) * (q - yabil::bigint::BigInt(1));

    yabil::bigint::BigInt e(2);
    while (e < phi && yabil::math::gcd(e, phi) != yabil::bigint::BigInt(1))
    {
        e += yabil::bigint::BigInt(1);
    }
    const auto d = yabil::math::mod_inverse(e, phi);
    return {{e, n}, {d, n}};
}

yabil::bigint::BigInt encrypt(uint8_t byte, const PublicKey &pub_key)
{
    return yabil::math::pow(yabil::bigint::BigInt(byte), pub_key.e, pub_key.n);
}

yabil::bigint::BigInt decrypt(const yabil::bigint::BigInt &encrypted, const PrivateKey &private_key)
{
    return yabil::math::pow(encrypted, private_key.d, private_key.n);
}

EncryptionStream::EncryptionStream(std::ostream &out, const PublicKey &pub_key) : out(out), pub_key(pub_key) {}
EncryptionStream::EncryptionStream(std::ostream &out, PublicKey &&pub_key) : out(out), pub_key(pub_key) {}

}  // namespace yabil::crypto::rsa
