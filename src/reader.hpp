#ifndef GGUF_READER_H
#define GGUF_READER_H

#include <fstream>
#include <iostream>
#include "types.hpp"

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

        // Reading value type
        uint32_t val_type_id = 0;
        ctx.file_stream->read(reinterpret_cast<char *>(&val_type_id), 4);
        gguf_type val_type = static_cast<gguf_type>(val_type_id);

        // TODO: Reading value. We need to make a function
        // that has a switch statement which handles reading
        // each value differently, according to its type
    }
}

#endif