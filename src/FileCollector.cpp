#include "../include/FileCollector.hpp"


int FileCollector::CollectFile(unsigned int file_id, size_t file_size) {
    // Generate file instance in RAM
    return 1;
}

int FileCollector::OnNewChunk(unsigned int file_id, size_t offset, std::vector<unsigned char>&& chunk) {
    /*
    Lock file_id and insert chunk
    For wait free alghoritm, we can avoid lcking at every time we get some info.
    We can lock only bounds of current usage instead whole array.
    */

    /*
    Here we get bounds of locked area. Left bound is offset, right - offset + chunk_size.
    */
    unsigned int start = offset;
    unsigned int chunk_size = chunk.size();
    ChunkedFile file = FileCollector::GetFile(file_id);

    std::vector<unsigned char> file_body = file.GetBody();
    int file_end = file_body.size();

    /*
    Base case if file body is smaller then offset.
    In this case we should add padding to vector to fit size.
    */
    while (file_end++ < offset) {
        file_body.push_back(0);
    }

    /*
    First we put data to allocated space to the end.
    */
    while (start < file_end && (start - offset) < chunk_size) {
        file_body[start] = chunk.at(start - offset);
        start++;
    }

    /*
    Also another base case if we don't enoght place for store data.
    That means that we should allocate a couple of space.
    */
    while (file_end++ < offset + chunk_size && (start - offset) < chunk_size) {
        file_body.push_back(chunk.at(start - offset));
        start++;
    }

    return 1;
}

ChunkedFile FileCollector::GetFile(unsigned int file_id) {
    // Return chunked file
    return ChunkedFile();
}
