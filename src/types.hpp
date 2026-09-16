#ifndef GGUF_TYPES_H
#define GGUF_TYPES_H

#include <variant>
#include <string>
#include <vector>
#include <iostream>

struct gguf_value
{
    using variant_t = std::variant<uint64_t, int64_t, double, bool, std::string, std::vector<std::shared_ptr<gguf_value>>>;

    gguf_metadata_value_type type;
    variant_t val;

    gguf_value(gguf_metadata_value_type t, variant_t v) : type(t), val(std::move(v)) {}
};

struct GGUFKVPair
{
    std::string key;
    gguf_value type;
    std::streampos stream_ptr;
};

struct gguf_context
{
    std::ifstream *file_stream;
    std::vector<GGUFKVPair> kv_pairs;
};

struct ggml_type_traits
{
    size_t block_size;
    size_t type_size;
};

enum gguf_metadata_value_type : uint32_t
{
    GGUF_UINT8 = 0,
    GGUF_INT8 = 1,
    GGUF_UINT16 = 2,
    GGUF_INT16 = 3,
    GGUF_UINT32 = 4,
    GGUF_INT32 = 5,
    GGUF_FLOAT32 = 6,
    GGUF_BOOL = 7,
    GGUF_STRING = 8,
    GGUF_ARRAY = 9,
    GGUF_UINT64 = 10,
    GGUF_INT64 = 11,
    GGUF_FLOAT64 = 12,
};

inline constexpr size_t gguf_value_size(gguf_metadata_value_type t)
{
    switch (t)
    {
    case GGUF_UINT8:
    case GGUF_INT8:
    case GGUF_BOOL:
        return 1;
    case GGUF_UINT16:
    case GGUF_INT16:
        return 2;
    case GGUF_UINT32:
    case GGUF_INT32:
    case GGUF_FLOAT32:
        return 4;
    case GGUF_UINT64:
    case GGUF_INT64:
    case GGUF_FLOAT64:
        return 8;

    case GGUF_STRING:
    case GGUF_ARRAY:
    default:
        throw std::runtime_error("gguf_value_size: type has no fixed size");
    }
}

#endif
