#include <fstream>
#include "crypt.h"
#include "..\core\core.h"

void crypt_t::startup(const std::string &encFileName, const std::string &decFileName) {
	this->loadKeys(encFileName, cryptKeys_e::_encrypt);
	this->loadKeys(decFileName, cryptKeys_e::_decrypt);
}

void crypt_t::loadKeys(std::string keysFileName, cryptKeys_e::type_t keysType) {
	std::ifstream keysFile(keysFileName.c_str(), std::ios::in | std::ios::binary);

	if(!keysFile.fail()) {
		unsigned short header = 0;
		keysFile.read(reinterpret_cast<char*>(&header), sizeof(header));

		size_t keysFileSize = 0;
		keysFile.read(reinterpret_cast<char*>(&keysFileSize), sizeof(keysFileSize));

		if(header == 0x1112 && keysFileSize == 54) {
			unsigned int temp[4] = {0};
			unsigned int fileXOR[4] = {0x3F08A79B, 0xE25CC287, 0x93D27AB9, 0x20DEA7BF};

			keysFile.read(reinterpret_cast<char*>(&temp), sizeof(unsigned int) * 4);
			for(int i = 0; i < 4; ++i) {
				if(keysType == cryptKeys_e::_decrypt) {
					m_decryptionModulus[i] = fileXOR[i] ^ temp[i];
				} else if(keysType == cryptKeys_e::_encrypt) {
					m_encryptionModulus[i] = fileXOR[i] ^ temp[i];
				}
			}

			keysFile.read(reinterpret_cast<char*>(&temp), sizeof(unsigned int) * 4);
			for(int i = 0; i < 4; ++i) {
				if(keysType == cryptKeys_e::_decrypt) {
					this->m_decryptionKeys[i] = fileXOR[i] ^ temp[i];
				} else if(keysType == cryptKeys_e::_encrypt) {
					this->m_encryptionKeys[i] = fileXOR[i] ^ temp[i];
				}
			}

			keysFile.read(reinterpret_cast<char*>(&temp), sizeof(unsigned int) * 4);
			for(int i = 0; i < 4; ++i) {
				if(keysType == cryptKeys_e::_decrypt) {
					this->m_decryptionXORKeys[i] = fileXOR[i] ^ temp[i];
				} else if(keysType == cryptKeys_e::_encrypt) {
					this->m_encryptionXORKeys[i] = fileXOR[i] ^ temp[i];
				}
			}

			keysFile.close();
		} else {
			keysFile.close();

			eMUCore::exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << "[crypt_t::loadKeys()] Keys file [" << keysFileName << "] is corrupted.";
			throw e;
		}
	} else {
		eMUCore::exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << "[crypt_t::loadKeys()] Could not open [" << keysFileName << "] file.";
		throw e;
	}
}

size_t crypt_t::encrypt(unsigned char *buff, const unsigned char *src, size_t srcSize) const {
	size_t decryptVal = ((srcSize + 7) / 8);
	size_t encryptSize = (decryptVal + decryptVal * 4) * 2 + decryptVal;

	if(buff != 0) {
		size_t srcSize2 = srcSize;
		for(size_t i = 0; i < srcSize; i += 8, srcSize2 -= 8, buff += 11) {
			size_t blockSize = srcSize2;

			if(srcSize2 >= 8) {
				blockSize = 8;
			}

			this->encryptBlock(buff, src + i, blockSize);
		}
	}

	return encryptSize;
}

void crypt_t::encryptBlock(unsigned char *buff, const unsigned char *src, size_t blockSize) const {
	unsigned int encBuff[4];
	unsigned int encValue = 0;

	memset(buff, 0, 11);

	for(int i = 0; i < 4; ++i) {
		encBuff[i] = ((m_encryptionXORKeys[i] ^ (reinterpret_cast<const unsigned short*>(src))[i] ^ encValue) * m_encryptionKeys[i]) % m_encryptionModulus[i];
		encValue = encBuff[i] & 0xFFFF;
	}

	for(int i = 0; i < 3; ++i) {
		encBuff[i] = encBuff[i] ^ m_encryptionXORKeys[i] ^ (encBuff[i + 1] & 0xFFFF);
	}

	size_t bitPos = 0;

	for(int i = 0; i < 4; ++i) {
		bitPos = this->addBits(buff, bitPos, reinterpret_cast<unsigned char*>(&encBuff[i]), 0, 16);
		bitPos = this->addBits(buff, bitPos, reinterpret_cast<unsigned char*>(&encBuff[i]), 22, 2);
	}

	unsigned char checksum = 0xF8;
	
	for(int i = 0; i < 8; ++i) {
		checksum ^= src[i];
	}

	(reinterpret_cast<unsigned char*>(&encValue))[1] = checksum; 
	(reinterpret_cast<unsigned char*>(&encValue))[0] = checksum ^ static_cast<unsigned char>(blockSize) ^ 0x3D; 

	this->addBits(buff, bitPos, reinterpret_cast<unsigned char*>(&encValue), 0, 16);
}

size_t crypt_t::addBits(unsigned char *buff, size_t addBitPos, const unsigned char *src, size_t srcBitPos, size_t size) const {
	size_t srcBitsCount = size + srcBitPos;
	size_t tempBuffSize = ((srcBitsCount - 1) >> 3) + (1 - (srcBitPos >> 3));

	unsigned char *tempBuff = new unsigned char[tempBuffSize + 1];
	memset(tempBuff, 0, tempBuffSize + 1);
	memcpy(tempBuff, src + (srcBitPos >> 3), tempBuffSize);

	if((srcBitsCount % 8) != 0) {
		tempBuff[tempBuffSize - 1] &= 0xFF << (8 - (srcBitsCount % 8));
	}

	int shiftLeft = static_cast<int>(srcBitPos) % 8;
	this->shift(tempBuff, tempBuffSize, -shiftLeft);

	int shiftRight = static_cast<int>(addBitPos) % 8;
	this->shift(tempBuff, tempBuffSize + 1, shiftRight);

	size_t newTempBuffSize = ((shiftRight <= shiftLeft) ? 0 : 1) + tempBuffSize;
	unsigned char *tempBuffDist = buff + (addBitPos >> 3);

	for(size_t i = 0; i < newTempBuffSize; ++i) {
		tempBuffDist[i] |= tempBuff[i];
	}

	delete [] tempBuff;

	return addBitPos + size;
}

void crypt_t::shift(unsigned char *buff, size_t size, int shiftVal) const {
	if(shiftVal != 0) {
		if(shiftVal > 0) {
			if((size - 1) > 0) {
				for(int i = static_cast<int>((size - 1)); i > 0; --i) {
					buff[i] = (buff[i - 1] << (8 - shiftVal)) | (buff[i] >> shiftVal);
				}
			}

			buff[0] >>=  shiftVal;
		} else {
			shiftVal = -shiftVal;

			if((size - 1) > 0) {
				for(unsigned int i = 0; i < (size - 1); ++i) {
					buff[i] = (buff[i + 1] >> (8 - shiftVal)) | (buff[i] << shiftVal);
				}
			}

			buff[size - 1] <<= shiftVal;
		}
	}
}

int crypt_t::decrypt(unsigned char *buff, const unsigned char *src, size_t srcSize) const {
	if(buff != 0) {
		int decryptResult = 0;
		size_t decSize = 0;

		if(srcSize > 0) {
			while(decSize < srcSize) {
				int decryptBlockResult = this->decryptBlock(buff, src);

				if(decryptResult < 0) {
					return decryptResult;
				}

				decryptResult += decryptBlockResult;
				decSize += 11;
				src += 11;
				buff += 8;
			}
		}

		return decryptResult;
	} else {
		return static_cast<int>(srcSize * 8 / 11);
	}
}

int crypt_t::decryptBlock(unsigned char *buff, const unsigned char *src) const {
	memset(buff, 0, 8);
	unsigned int decBuff[4]= {0};
	size_t bitPos = 0;

	for(int i = 0; i < 4; ++i) {
		this->addBits(reinterpret_cast<unsigned char*>(&decBuff[i]), 0, src, bitPos, 16);
		bitPos += 16;

		this->addBits(reinterpret_cast<unsigned char*>(&decBuff[i]), 22, src, bitPos, 2);
		bitPos += 2;
	}

	for(int i = 2; i >= 0; --i) {
		decBuff[i] = decBuff[i] ^ m_decryptionXORKeys[i] ^ (decBuff[i + 1] & 0xFFFF);
	}

	unsigned int temp = 0, temp1 = 0;

	for(int i = 0; i < 4; ++i) {
		temp1 = ((m_decryptionKeys[i] * decBuff[i]) % m_decryptionModulus[i]) ^ m_decryptionXORKeys[i] ^ temp;
		temp = decBuff[i] & 0xFFFF;
		(reinterpret_cast<unsigned short*>(buff))[i] = temp1;
	}

	decBuff[0] = 0;
	this->addBits(reinterpret_cast<unsigned char*>(&decBuff[0]), 0, src, bitPos, 16);
	(reinterpret_cast<unsigned char*>(decBuff))[0] = (reinterpret_cast<unsigned char*>(decBuff))[1] ^ (reinterpret_cast<unsigned char*>(decBuff))[0] ^ 0x3D;

	unsigned char checksum = 0xF8;

	for(int i = 0; i < 8; ++i) {
		checksum ^= buff[i];
	}

	if(checksum != (reinterpret_cast<unsigned char*>(decBuff))[1]) {
		return -1;
	} else {
		return (reinterpret_cast<unsigned char*>(decBuff))[0];
	}
}

void crypt_t::extract(unsigned char *buff, size_t buffSize) const {
	size_t end = buff[0] == 0xC1 ? 2 : 3;

	for(size_t i = buffSize - 1; i != end; --i) {
		buff[i] ^= buff[i - 1] ^ c_extractionKeys[(i & sizeof(c_extractionKeys) - 1)];
	}
}