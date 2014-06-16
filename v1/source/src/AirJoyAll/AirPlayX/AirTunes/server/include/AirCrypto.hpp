/*
* AirCrypto.hpp
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

#ifndef __AIR_CRYPTO_H__
#define __AIR_CRYPTO_H__

#include <string>

#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

namespace airjoy
{

    class AirCrypto
    {
    public:

    private:
        RSA             *m_privateKey;
        char             m_signature[1024];
        char             m_response[1024];

        char             m_rsaAesKey[1024];
        char             m_aesIv[1024];

    public:
        AirCrypto();
        virtual ~AirCrypto();

        RSA * privateKey(void);

        const char * challengeResponse(const unsigned char *challenge, 
                                       size_t challengeLength, 
                                       const unsigned char *ipAddress, 
                                       size_t ipAddressLength,
                                       const unsigned char *macAddress,
                                       size_t macAddressLength);
        
        const char * getResponseForChallenge(const unsigned char *challenge, 
                                             size_t challengeLength, 
                                             const unsigned char *ipAddress, 
                                             size_t ipAddressLength,
                                             const unsigned char *macAddress,
                                             size_t macAddressLength);

        bool setRsaAesKey(const char *key, size_t len);

        bool setAesIv(const char *iv, size_t len);

        const char * decryptData(const char *data);

        void reset(void);

    private:
        bool setPrivateKey(void);

        


    };
}

#endif // __AIR_CRYPTO_H__