#include <cstddef>
#include <vector>


class ChunkedFile {
public:
    ChunkedFile();
    std::vector<unsigned char> GetBody();
private:
    std::vector<unsigned char> body;
};
