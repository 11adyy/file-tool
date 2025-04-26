#include <gtest/gtest.h>
#include "../include/FileCollector.hpp"
#include <thread>
#include <vector>
#include <cstring>

// утилита для заполнения буфера символами
static std::vector<unsigned char> make_chunk(char start, size_t len) {
    std::vector<unsigned char> v(len);
    for (size_t i = 0; i < len; ++i) v[i] = static_cast<unsigned char>(start + i);
    return v;
}

TEST(FileCollector_Basic, CollectAndGetFile) {
    FileCollector fc;
    EXPECT_EQ(fc.CollectFile(1, 5), 1);
    auto ptr = fc.GetFile(1);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->GetBodySize(), 5u);
}

TEST(FileCollector_Invalid, OnNewChunkWithoutCollect) {
    FileCollector fc;
    auto chunk = make_chunk('A', 3);
    EXPECT_EQ(fc.OnNewChunk(42, 0, std::move(chunk)), 0);
    EXPECT_EQ(fc.GetFile(42), nullptr);
}

TEST(FileCollector_SingleThread, OverlappingChunks) {
    FileCollector fc;
    const size_t N = 10;
    fc.CollectFile(7, N);

    // [A,B,C,D]
    fc.OnNewChunk(7, 0, make_chunk('A', 4));
    //        [C,D,E,F]
    fc.OnNewChunk(7, 2, make_chunk('C', 4));
    //              [G,H,I,J]
    fc.OnNewChunk(7, 6, make_chunk('G', 4));

    auto cf = fc.GetFile(7);
    ASSERT_NE(cf, nullptr);
    auto body = cf->GetBody();
    std::string expected = "ABCDEFGHIJ";
    std::string actual(reinterpret_cast<char*>(body), N);
    EXPECT_EQ(actual, expected);
}

TEST(FileCollector_ThreadSafety, ConcurrentWrites) {
    FileCollector fc;
    const size_t N = 1000;
    fc.CollectFile(5, N);

    auto worker = [&](size_t offset, char ch){
        auto chunk = make_chunk(ch, 100);
        fc.OnNewChunk(5, offset, std::move(chunk));
    };

    // запустим 10 потоков, каждый пишет свой участок
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker, i * 100, 'a' + i * 10);
    }
    for (auto& t : threads) t.join();

    auto cf = fc.GetFile(5);
    ASSERT_NE(cf, nullptr);
    auto body = cf->GetBody();
    // проверим каждый сегмент
    for (int i = 0; i < 10; ++i) {
        char expected = static_cast<char>('a' + i * 10);
        for (size_t j = 0; j < 100; ++j) {
            EXPECT_EQ(body[i * 100 + j], expected)
                << "Mismatch at byte " << (i * 100 + j);
        }
    }
}
