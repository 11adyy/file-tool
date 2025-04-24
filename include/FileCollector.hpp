#ifndef FILE_COLLECTOR_H_
#define FILE_COLLECTOR_H_

#include "ChunkedFile.hpp"

#include <cstddef>
#include <vector>
#include <unordered_map>
#include <memory>


class FileCollector {
public:
    int CollectFile(unsigned int file_id, size_t file_size);
    int OnNewChunk(unsigned int file_id, size_t offset, std::vector<unsigned char>&& chunk);
    ChunkedFile* GetFile(unsigned int file_id);

private:
    std::unordered_map<unsigned int, std::unique_ptr<ChunkedFile>> files;
};

#endif
