#ifndef CHUNKED_FILE_H_
#define CHUNKED_FILE_H_

#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector>
#include <memory>
#include <atomic>


class ChunkedFile {
public:
    ChunkedFile(size_t size);
    unsigned char* GetBody();
    size_t GetBodySize();
    int SaveFile(const char* path);
    int Lock();
    int Unlock();

private:
    std::atomic_char lock_;
    std::unique_ptr<unsigned char[]> body_;
    size_t body_size_;
};

#endif
