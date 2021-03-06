#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include "file.h"

namespace IO {
	class FileReaderByteStreamer {
	public:
		void SetFileReader(IO::FileReader::Ptr file) throw();
		uint8_t& ReadByte();
		FileInt BytesLeft();
		void SkipBytes(int numToSkip);
		FileInt VirtualFilePosition();

		FileReaderByteStreamer();

	private:
		enum {
			CacheSize = 4096
		};

		FileInt m_fileStart;
		uint8_t m_cache[CacheSize];
		size_t m_position;
		size_t m_cacheSize;
		FileInt m_bytesLeft;
		IO::FileReader::Ptr m_reader;
	};
}

#endif
