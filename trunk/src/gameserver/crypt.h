#ifndef eMU_GAMESERVER_CRYPT_H
#define eMU_GAMESERVER_CRYPT_H

#include <string>

const unsigned char	c_extractionKeys[32] = {0xE7, 0x6D, 0x3A, 0x89, 0xBC, 0xB2, 0x9F, 0x73,
											0x23, 0xA8, 0xFE, 0xB6, 0x49, 0x5D, 0x39, 0x5D,
											0x8A, 0xCB, 0x63, 0x8D, 0xEA, 0x7D, 0x2B, 0x5F,
											0xC3, 0xB1, 0xE9, 0x83, 0x29, 0x51, 0xE8, 0x56};

class crypt_t {
public:
	enum KEYS_TYPE {
		_ENCRYPTION_KEYS = 0,
		_DECRYPTION_KEYS
	};

	crypt_t() {}

	void startup(const std::string &encFileName, const std::string &decFileName);
	size_t encrypt(unsigned char *buff, const unsigned char *src, size_t srcSize) const;
	int decrypt(unsigned char *buff, const unsigned char *src, size_t srcSize) const;
	void extract(unsigned char *buff, size_t buffSize) const;

private:
	crypt_t(const crypt_t&);
	crypt_t& operator=(const crypt_t&);

	void loadKeys(std::string keysFileName, KEYS_TYPE keysType);
	void encryptBlock(unsigned char *buff, const unsigned char *src, size_t blockSize) const;
	int decryptBlock(unsigned char *buff, const unsigned char *src) const;

	size_t addBits(unsigned char *buff, size_t addBitPos, const unsigned char *src, size_t srcBitPos, size_t size) const;
	void shift(unsigned char *buff, size_t size, int shiftVal) const;

	unsigned int	m_encryptionKeys[4];
	unsigned int	m_encryptionXORKeys[4];
	unsigned int	m_encryptionModulus[4];

	unsigned int	m_decryptionKeys[4];
	unsigned int	m_decryptionXORKeys[4];
	unsigned int	m_decryptionModulus[4];
};

#endif // eMU_GAMESERVER_CRYPT_H