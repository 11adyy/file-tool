#include "../include/ChunkedFile.hpp"


ChunkedFile::ChunkedFile(size_t size) : body_(std::make_unique<unsigned char[]>(size)), body_size_(size) { }

unsigned char* ChunkedFile::GetBody() {
    return body_.get();
}

int ChunkedFile::SaveFile(const char* path) {
    while (lock_) continue;
    std::ofstream output(path);
    output << body_ << std::endl;
    output.close();
}

size_t ChunkedFile::GetBodySize() {
    return body_size_;
}

int ChunkedFile::Lock() {
    lock_ = 1;
    return lock_;
}

int ChunkedFile::Unlock() {
    lock_ = 0;
    return lock_;
}
