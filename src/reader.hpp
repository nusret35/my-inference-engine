#ifndef GGUF_READER_H
#define GGUF_READER_H

#include <fstream>
#include <iostream>
#include "types.hpp"
#include <variant>
#include <cstdint>

gguf_value read_value(gguf_context &ctx)
{
    // Reading value type
    uint32_t val_type_id = 0;
    ctx.file_stream->read(reinterpret_cast<char *>(&val_type_id), 4);
    auto val_type = static_cast<gguf_metadata_value_type>(val_type_id);
    switch (val_type)
    {
    case gguf_metadata_value_type::GGUF_UINT8:
    case gguf_metadata_value_type::GGUF_BOOL:
    {
        uint64_t v = 0;
        ctx.file_stream->read(reinterpret_cast<char *>(&v), 1);
        return gguf_value(val_type, v);
    }
    case GGUF_UINT16:
    case GGUF_INT16:
    {
        uint64_t v = 0;
        ctx.file_stream->read(reinterpret_cast<char *>(&v), 2);
        return gguf_value(val_type, v);
    }
    case GGUF_FLOAT32:
    case GGUF_UINT32:
    {
        uint64_t v = 0;
        ctx.file_stream->read(reinterpret_cast<char *>(&v), 4);
        return gguf_value(val_type, v);
    }
    case GGUF_STRING:
    {
        uint64_t string_len = 0;
        ctx.file_stream->read(reinterpret_cast<char *>(&string_len), 8);
        std::string buf(string_len, '\0');
        ctx.file_stream->read(&buf[0], string_len);
        return gguf_value(val_type, buf);
    }
    case GGUF_ARRAY:
    {
        std::vector<std::shared_ptr<gguf_value>> v;
        v = read_array(ctx);
        return gguf_value(val_type, v);
    }
    default:
    {
        uint64_t v = 0;
        ctx.file_stream->read(reinterpret_cast<char *>(&v), 8);
        return gguf_value(val_type, v);
    }
    }
}

std::vector<std::shared_ptr<gguf_value>> read_array(gguf_context &ctx)
{
    uint32_t count = 0;
    uint32_t elem_type_id = 0;
    std::vector<std::shared_ptr<gguf_value>> elements;

    ctx.file_stream->read(reinterpret_cast<char *>(&count), 4);
    ctx.file_stream->read(reinterpret_cast<char *>(&elem_type_id), 4);
    if (elem_type_id == gguf_metadata_value_type::GGUF_ARRAY)
    {
        return read_array(ctx);
    }

    for (int i = 0; i < count; i++)
    {
        gguf_value value = read_value(ctx);
        elements.push_back(std::make_shared<gguf_value>(value));
    }
    return elements;
}

void read_kv_pairs(gguf_context &ctx, uint32_t metadata_count)
{
    for (int i = 0; i < metadata_count - 1; i++)
    {
        // Length of the key
        uint64_t key_len = 0;
        ctx.file_stream->read(reinterpret_cast<char *>(&key_len), 8);

        // Reading key
        std::string key_name(key_len, '\0');
        ctx.file_stream->read(&key_name[0], key_len);
        read_value(ctx);
    }
}

#endif
