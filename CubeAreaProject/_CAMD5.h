#pragma once
#include <string>
#include <fstream>

/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;
using std::wstring;
using std::ifstream;

class _CAMD5
{
public:
	_CAMD5();
	_CAMD5(const void * input, size_t length);
	_CAMD5(const string & str);
	_CAMD5(ifstream & in);
	void update(const void * input, size_t length);
	void update(const string & str);
	void update(ifstream & in);
	const byte * digest();
	string toString();
	wstring toWString();
	void reset();
	~_CAMD5();
private:
	void update(const byte * input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const uint32 * input, byte * output, size_t length);
	void decode(const byte * input, uint32 * output, size_t length);
	string bytesToHexString(const byte * input, size_t length);
	wstring bytesToHexWString(const byte * input, size_t length);

	/* class uncopyable */
	_CAMD5(const _CAMD5&);
	_CAMD5 & operator=(const _CAMD5 &);
	
private:
	uint32 _state[4];// state (ABCD)
	uint32 _count[2];// number of bits, module 2^64 (low-order word first)
	byte _buffer[64];// input buffer
	byte _digest[16];// message digest
	bool _finished;// calculate finished ?

	static const byte PADDING[64];// padding for calculate
	static const char HEX[16];
	static const size_t BUFFER_SIZE = 1024;
};

