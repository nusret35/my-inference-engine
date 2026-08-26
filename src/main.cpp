#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void read_gguf_metadata_counts(std::ifstream &file, uint32_t &version, uint32_t &tensor_count, uint64_t &metadata_count)
{
    // 4 bytes
    char magic[4];
    file.read(magic, 4);

    // 4 bytes
    file.read(reinterpret_cast<char *>(&version), 4);

    // 8 bytes
    file.read(reinterpret_cast<char *>(&tensor_count), 8);

    // 8 bytes
    file.read(reinterpret_cast<char *>(&metadata_count), 8);
}

void read_value(uint32_t type)
{
    // read value according to its type
}

void read_gguf_metadata(std::ifstream &file) {}

int main()
{

    std::ifstream file("../Qwen3-0.6B-Q8_0.gguf", std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // 4 bytes
    char magic[4];

    // 4 bytes
    uint32_t version;

    // 8 bytes
    uint32_t tensor_count;

    // 8 bytes
    uint64_t metadata_count;

    read_gguf_metadata_counts(file, version, tensor_count, metadata_count);

    std::cout << "Magic: "
              << magic[0] << magic[1]
              << magic[2] << magic[3] << '\n';

    std::cout << "Version: " << version << '\n';
    std::cout << "Tensors: " << tensor_count << '\n';
    std::cout << "Metadata: " << metadata_count << '\n';

    return 0;
}
