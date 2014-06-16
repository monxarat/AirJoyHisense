/*
* AirCrypto.cpp
*
*
* coding.tom@gmail.com
*
* 2012-5-23
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*
*/

#include "AirCrypto.hpp"
#include "AirBase64.hpp"
#include "UtilBase64.hpp"
#include "UtilString.hpp"
#include "AirJoyTypeDef.hpp"

#include <string.h> // for memset

using namespace airjoy;

AirCrypto::AirCrypto()
    : m_privateKey(NULL)
{
    this->setPrivateKey();

    memset(m_signature, 0, sizeof(m_signature));
    memset(m_response, 0, sizeof(m_response));
    memset(m_rsaAesKey, 0, sizeof(m_rsaAesKey));
    memset(m_aesIv, 0, sizeof(m_aesIv));
}

AirCrypto::~AirCrypto()
{
    if (m_privateKey != NULL)
	    RSA_free(m_privateKey);
}

RSA * AirCrypto::privateKey(void)
{
    return m_privateKey;
}

const char * AirCrypto::challengeResponse(const unsigned char *challenge, 
                                          size_t challengeLength, 
                                          const unsigned char *ipAddress, 
                                          size_t ipAddressLength,
                                          const unsigned char *macAddress,
                                          size_t macAddressLength)
{
    if (m_privateKey == NULL)
        return NULL;

    unsigned char data[38];
    memset(data, 0, 38);
    size_t dataLength = 0;

    // response begins with the challenge random data
    if (challengeLength != 16)
        return NULL;

    memcpy(data, challenge, challengeLength);
    dataLength += challengeLength;
    
    // append ip address
    
    if (ipAddressLength > 16)
        return NULL;

    memcpy(data + dataLength, ipAddress, ipAddressLength);
    dataLength += ipAddressLength;
    
	// append mac address
    if (macAddressLength != 6)
        return NULL;

    memcpy(data + dataLength, macAddress, macAddressLength);
    dataLength += macAddressLength;
    
    if (dataLength < 32)
        data[dataLength++] = 0;

#if 1
    memset(m_signature, 0, 1024);
    size_t sizeSignature	= 0;
    sizeSignature = RSA_private_encrypt(dataLength, data, (unsigned char *)m_signature, m_privateKey, RSA_PKCS1_PADDING);
#else

    // ok，测试值是对的。
    unsigned char newData[] = {0xb3, 0x8a, 0xd, 0x4e, 
                               0x8f, 0xad, 0xec, 0x6e, 
                               0xe9, 0xfc, 0xc7, 0xfe,
                               0xc1, 0x6b, 0xc, 0xd6,
                               0xfe, 0x80, 0x0, 0x0,
                               0x0, 0x0, 0x0, 0x0,
                               0x8e, 0x2d, 0xaa, 0xff,
                               0xfe, 0x89, 0xaa, 0x5a,
                               0x8c, 0x2d, 0xaa, 0x89, 0xaa, 0x5a};
    size_t newDataLength = sizeof(newData);

    memset(m_signature, 0, 1024);
    size_t sizeSignature = RSA_private_encrypt(newDataLength, newData, (unsigned char *)m_signature, m_privateKey, RSA_PKCS1_PADDING);
#endif

    return m_signature;
}

const char * AirCrypto::getResponseForChallenge(const unsigned char *challenge, 
                                                size_t challengeLength, 
                                                const unsigned char *ipAddress, 
                                                size_t ipAddressLength,
                                                const unsigned char *macAddress,
                                                size_t macAddressLength)
{
    char *tResponse = NULL;

    unsigned char tTrim[1024];
    util::getTrimmed((const char *)challenge, challengeLength, true, true, (char *)tTrim);

    int tChallengeDecodeSize = 16;
    char *tChallenge = airbase64::decode_base64((unsigned char *)tTrim, challengeLength, &tChallengeDecodeSize);

    int tCurSize = 0;
    unsigned char tChalResp[38];

    memcpy(tChalResp, tChallenge, tChallengeDecodeSize);
    tCurSize += tChallengeDecodeSize;

    memcpy(tChalResp+tCurSize, ipAddress, ipAddressLength);
    tCurSize += ipAddressLength;

    memcpy(tChalResp+tCurSize, macAddress, macAddressLength);
    tCurSize += macAddressLength;

    int tPad = 32 - tCurSize;
    if (tPad > 0)
    {
        memset(tChalResp + tCurSize, 0, tPad);
        tCurSize += tPad;
    }

    char *tTmp = airbase64::encode_base64((unsigned char *)tChalResp, tCurSize);
    free(tTmp);

    // RSA Encrypt
    int tSize = RSA_size(m_privateKey);
    unsigned char tTo[1024];
    RSA_private_encrypt(tCurSize, (unsigned char *)tChalResp, tTo, m_privateKey, RSA_PKCS1_PADDING);

    // Wrap RSA Encrypted binary in Base64 encoding
    tResponse = airbase64::encode_base64(tTo, tSize);
    int tLen = strlen(tResponse);
    while(tLen > 1 && tResponse[tLen-1] == '=')
    {
        tResponse[tLen-1] = '\0';
    }

    memset(m_response, 0, sizeof(m_response));
    memcpy(m_response, tResponse, strlen(tResponse));
    
    free(tChallenge);
    free(tResponse);

    return m_response;
}

bool AirCrypto::setRsaAesKey(const char *key, size_t len)
{
    memset(m_rsaAesKey, 0, sizeof(m_rsaAesKey));
    memcpy(m_rsaAesKey, key, len);
    
    return true;
}

bool AirCrypto::setAesIv(const char *iv, size_t len)
{
    memset(m_aesIv, 0, sizeof(m_aesIv));
    memcpy(m_aesIv, iv, len);
    
    return true;
}

const char * AirCrypto::decryptData(const char *data)
{
    return NULL;
}

void AirCrypto::reset(void)
{
    memset(m_response, 0, sizeof(m_response));
    memset(m_rsaAesKey, 0, sizeof(m_rsaAesKey));
    memset(m_aesIv, 0, sizeof(m_aesIv));
}


//--------------------------------------------------------------------------------------
//
// Private API
//
//--------------------------------------------------------------------------------------

bool AirCrypto::setPrivateKey(void)
{
    // Thank you James Laird
    std::string key = base64::decode(
		"MIIEpQIBAAKCAQEA59dE8qLieItsH1WgjrcFRKj6eUWqi+bGLOX1HL3U3GhC/j0Qg90u3sG/1CUt"
		"wC5vOYvfDmFI6oSFXi5ELabWJmT2dKHzBJKa3k9ok+8t9ucRqMd6DZHJ2YCCLlDRKSKv6kDqnw4U"
		"wPdpOMXziC/AMj3Z/lUVX1G7WSHCAWKf1zNS1eLvqr+boEjXuBOitnZ/bDzPHrTOZz0Dew0uowxf"
		"/+sG+NCK3eQJVxqcaJ/vEHKIVd2M+5qL71yJQ+87X6oV3eaYvt3zWZYD6z5vYTcrtij2VZ9Zmni/"
		"UAaHqn9JdsBWLUEpVviYnhimNVvYFZeCXg/IdTQ+x4IRdiXNv5hEewIDAQABAoIBAQDl8Axy9XfW"
		"BLmkzkEiqoSwF0PsmVrPzH9KsnwLGH+QZlvjWd8SWYGN7u1507HvhF5N3drJoVU3O14nDY4TFQAa"
		"LlJ9VM35AApXaLyY1ERrN7u9ALKd2LUwYhM7Km539O4yUFYikE2nIPscEsA5ltpxOgUGCY7b7ez5"
		"NtD6nL1ZKauw7aNXmVAvmJTcuPxWmoktF3gDJKK2wxZuNGcJE0uFQEG4Z3BrWP7yoNuSK3dii2jm"
		"lpPHr0O/KnPQtzI3eguhe0TwUem/eYSdyzMyVx/YpwkzwtYL3sR5k0o9rKQLtvLzfAqdBxBurciz"
		"aaA/L0HIgAmOit1GJA2saMxTVPNhAoGBAPfgv1oeZxgxmotiCcMXFEQEWflzhWYTsXrhUIuz5jFu"
		"a39GLS99ZEErhLdrwj8rDDViRVJ5skOp9zFvlYAHs0xh92ji1E7V/ysnKBfsMrPkk5KSKPrnjndM"
		"oPdevWnVkgJ5jxFuNgxkOLMuG9i53B4yMvDTCRiIPMQ++N2iLDaRAoGBAO9v//mU8eVkQaoANf0Z"
		"oMjW8CN4xwWA2cSEIHkd9AfFkftuv8oyLDCG3ZAf0vrhrrtkrfa7ef+AUb69DNggq4mHQAYBp7L+"
		"k5DKzJrKuO0r+R0YbY9pZD1+/g9dVt91d6LQNepUE/yY2PP5CNoFmjedpLHMOPFdVgqDzDFxU8hL"
		"AoGBANDrr7xAJbqBjHVwIzQ4To9pb4BNeqDndk5Qe7fT3+/H1njGaC0/rXE0Qb7q5ySgnsCb3DvA"
		"cJyRM9SJ7OKlGt0FMSdJD5KG0XPIpAVNwgpXXH5MDJg09KHeh0kXo+QA6viFBi21y340NonnEfdf"
		"54PX4ZGS/Xac1UK+pLkBB+zRAoGAf0AY3H3qKS2lMEI4bzEFoHeK3G895pDaK3TFBVmD7fV0Zhov"
		"17fegFPMwOII8MisYm9ZfT2Z0s5Ro3s5rkt+nvLAdfC/PYPKzTLalpGSwomSNYJcB9HNMlmhkGzc"
		"1JnLYT4iyUyx6pcZBmCd8bD0iwY/FzcgNDaUmbX9+XDvRA0CgYEAkE7pIPlE71qvfJQgoA9em0gI"
		"LAuE4Pu13aKiJnfft7hIjbK+5kyb3TysZvoyDnb3HOKvInK7vXbKuU4ISgxB2bB3HcYzQMGsz1qJ"
		"2gG0N5hvJpzwwhbhXqFKA4zaaSrw622wDniAK5MlIE0tIAKKP4yxNGjoD2QYjhBGuhvkWKaXTyY="
        );

    long sizeKey = key.length();
    const unsigned char *pKey = (const unsigned char *)key.c_str();

    d2i_RSAPrivateKey(&m_privateKey, &pKey, sizeKey);

    return true;
}
