#ifndef GGUF_TYPES_H
#define GGUF_TYPES_H

#include <iostream>

// Official gguf types - source https://github.com/ggml-org/ggml/blob/master/include/ggml.h
enum gguf_type : uint32_t
{
    GGML_TYPE_F32 = 0,
    GGML_TYPE_F16 = 1,
    GGML_TYPE_Q4_0 = 2,
    GGML_TYPE_Q4_1 = 3,
    // GGML_TYPE_Q4_2 = 4, support has been removed
    // GGML_TYPE_Q4_3 = 5, support has been removed
    GGML_TYPE_Q5_0 = 6,
    GGML_TYPE_Q5_1 = 7,
    GGML_TYPE_Q8_0 = 8,
    GGML_TYPE_Q8_1 = 9,
    GGML_TYPE_Q2_K = 10,
    GGML_TYPE_Q3_K = 11,
    GGML_TYPE_Q4_K = 12,
    GGML_TYPE_Q5_K = 13,
    GGML_TYPE_Q6_K = 14,
    GGML_TYPE_Q8_K = 15,
    GGML_TYPE_IQ2_XXS = 16,
    GGML_TYPE_IQ2_XS = 17,
    GGML_TYPE_IQ3_XXS = 18,
    GGML_TYPE_IQ1_S = 19,
    GGML_TYPE_IQ4_NL = 20,
    GGML_TYPE_IQ3_S = 21,
    GGML_TYPE_IQ2_S = 22,
    GGML_TYPE_IQ4_XS = 23,
    GGML_TYPE_I8 = 24,
    GGML_TYPE_I16 = 25,
    GGML_TYPE_I32 = 26,
    GGML_TYPE_I64 = 27,
    GGML_TYPE_F64 = 28,
    GGML_TYPE_IQ1_M = 29,
    GGML_TYPE_BF16 = 30,
    // GGML_TYPE_Q4_0_4_4 = 31, support has been removed from gguf files
    // GGML_TYPE_Q4_0_4_8 = 32,
    // GGML_TYPE_Q4_0_8_8 = 33,
    GGML_TYPE_TQ1_0 = 34,
    GGML_TYPE_TQ2_0 = 35,
    // GGML_TYPE_IQ4_NL_4_4 = 36,
    // GGML_TYPE_IQ4_NL_4_8 = 37,
    // GGML_TYPE_IQ4_NL_8_8 = 38,
    GGML_TYPE_MXFP4 = 39, // MXFP4 (1 block)
    GGML_TYPE_NVFP4 = 40, // NVFP4 (4 blocks, E4M3 scale)
    GGML_TYPE_Q1_0 = 41,
    GGML_TYPE_Q2_0 = 42,
    GGML_TYPE_COUNT = 43,
};

struct GGUFKVPair
{
    std::string key;
    gguf_type type;
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

constexpr ggml_type_traits get_gguf_type_traits(gguf_type type)
{
    switch (type)
    {
    // Standard non-quantized types (1 element per block)
    case GGML_TYPE_F32:
        return {1, 4};
    case GGML_TYPE_F16:
        return {1, 2};
    case GGML_TYPE_I8:
        return {1, 1};
    case GGML_TYPE_I16:
        return {1, 2};
    case GGML_TYPE_I32:
        return {1, 4};
    case GGML_TYPE_I64:
        return {1, 8};
    case GGML_TYPE_F64:
        return {1, 8};
    case GGML_TYPE_BF16:
        return {1, 2};

    // Legacy Quantizations (32 elements per block)
    case GGML_TYPE_Q4_0:
        return {32, 18};
    case GGML_TYPE_Q4_1:
        return {32, 20};
    case GGML_TYPE_Q5_0:
        return {32, 22};
    case GGML_TYPE_Q5_1:
        return {32, 24};
    case GGML_TYPE_Q8_0:
        return {32, 34};
    case GGML_TYPE_Q8_1:
        return {32, 36};

    // K-Quantizations (256 elements per block superblocks)
    case GGML_TYPE_Q2_K:
        return {256, 84};
    case GGML_TYPE_Q3_K:
        return {256, 110};
    case GGML_TYPE_Q4_K:
        return {256, 144};
    case GGML_TYPE_Q5_K:
        return {256, 176};
    case GGML_TYPE_Q6_K:
        return {256, 210};
    case GGML_TYPE_Q8_K:
        return {256, 256};

    // Importance Quantizations (IQ)
    case GGML_TYPE_IQ2_XXS:
        return {256, 66};
    case GGML_TYPE_IQ2_XS:
        return {256, 74};
    case GGML_TYPE_IQ3_XXS:
        return {256, 96};
    case GGML_TYPE_IQ1_S:
        return {256, 50};
    case GGML_TYPE_IQ4_NL:
        return {32, 19};
    case GGML_TYPE_IQ3_S:
        return {256, 112};
    case GGML_TYPE_IQ2_S:
        return {256, 82};
    case GGML_TYPE_IQ4_XS:
        return {256, 136};
    case GGML_TYPE_IQ1_M:
        return {256, 56};

    // Ternary Quantizations (TQ)
    case GGML_TYPE_TQ1_0:
        return {256, 64};
    case GGML_TYPE_TQ2_0:
        return {256, 66};

    // Microscaling & Native Hardware FP4 formats
    case GGML_TYPE_MXFP4:
        return {32, 17}; // 1 block scale + 16 packed bytes
    case GGML_TYPE_NVFP4:
        return {64, 36}; // 4 group scale bytes + 32 packed bytes

    // Extreme 1-bit / 2-bit classic layouts
    case GGML_TYPE_Q1_0:
        return {128, 18}; // 16 bytes packed weights + 1 f16 scale
    case GGML_TYPE_Q2_0:
        return {64, 18}; // 16 bytes packed weights + 1 f16 scale

    // Handle error states or bounds check boundaries
    case GGML_TYPE_COUNT:
    default:
        throw std::runtime_error("Unknown or explicitly unsupported GGUF type enum index.");
    }
}

#endif
