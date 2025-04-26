#include "../include/FileCollector.hpp"


int FileCollector::CollectFile(unsigned int file_id, size_t file_size) {
    files[file_id] = std::make_unique<ChunkedFile>(file_size);
    return 1;
}

int FileCollector::OnNewChunk(unsigned int file_id, size_t offset, std::vector<unsigned char>&& chunk) {
    unsigned int start = offset;
    unsigned int chunk_size = chunk.size();
    ChunkedFile* file = GetFile(file_id);
    while (file->TestSaveLock()) continue;

    if (file) {
        file->Lock();
        unsigned char* file_body = file->GetBody();
        std::copy(chunk.begin(), chunk.end(), file_body + offset);
        file->Unlock();
        return 1;
    }

    return 0;
}

ChunkedFile* FileCollector::GetFile(unsigned int file_id) {
    auto it = files.find(file_id);
    if (it == files.end()) return nullptr;
    return it->second.get();
}
