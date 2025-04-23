#include "ChunkedFile.hpp"

#include <cstddef>
#include <vector>


class FileCollector {
public:
    int CollectFile(unsigned int file_id, size_t file_size);
    int OnNewChunk(unsigned int file_id, size_t offset, std::vector<unsigned char>&& chunk);
    ChunkedFile GetFile(unsigned int file_id);
};
