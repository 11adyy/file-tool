#include "../include/ChunkedFile.hpp"


ChunkedFile::ChunkedFile(size_t size) : body_(std::make_unique<unsigned char[]>(size)), body_size_(size) { }

unsigned char* ChunkedFile::GetBody() {
    return body_.get();
}

int ChunkedFile::SaveFile(const char* path) {
    while (TestLock()) continue;
    LockSave();
    int fd = open(path, O_WRONLY | O_CREAT);
    if (fd < 0) return 0;
    if (pwrite(fd, GetBody(), body_size_, 0) != body_size_) {
        close(fd);
        UnlockSave();
        return 0;
    }

    close(fd);
    UnlockSave();
    return 1;
}

size_t ChunkedFile::GetBodySize() {
    return body_size_;
}

int ChunkedFile::LockSave() {
    save_lock_ = 1;
    return 1;
}

int ChunkedFile::UnlockSave() {
    save_lock_ = 0;
    return 1;
}

int ChunkedFile::TestSaveLock() {
    return save_lock_;
}

int ChunkedFile::Lock() {
    lock_++;
    return 1;
}

int ChunkedFile::Unlock() {
    lock_--;
    return 1;
}

int ChunkedFile::TestLock() {
    return lock_;
}
