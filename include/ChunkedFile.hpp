#ifndef CHUNKED_FILE_H_
#define CHUNKED_FILE_H_

#include <cstddef>
#include <vector>
#include <memory>


class ChunkedFile {
public:
    ChunkedFile(size_t size);
    unsigned char* GetBody();
    size_t GetBodySize();

private:
    std::unique_ptr<unsigned char[]> body;
    size_t body_size;
};

#endif
