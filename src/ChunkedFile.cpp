#include "../include/ChunkedFile.hpp"


ChunkedFile::ChunkedFile(size_t size) : body(std::make_unique<unsigned char[]>(size)), body_size(size) { }

unsigned char* ChunkedFile::GetBody() {
    return body.get();
}

size_t ChunkedFile::GetBodySize() {
    return body_size;
}
