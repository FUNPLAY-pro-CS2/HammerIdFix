//
// Created by Michal Přikryl on 14.10.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <cstring>
#include <cstdlib>

#ifndef BUILD_SEED
#define BUILD_SEED 0xA3B1C2D3E4F56789ULL
#endif

static constexpr uint64_t ct_rotr(uint64_t v, unsigned r) noexcept {
    r &= 63u;
    if (r == 0) return v;
    return (v >> r) | (v << ((64 - r) & 63u));
}

namespace obf_constexpr {

template <size_t N>
struct obf_blob {
    uint8_t data[N];

    constexpr obf_blob(const char (&s)[N], uint64_t seed = (uint64_t)BUILD_SEED) : data{} {
        uint64_t r = seed ^ 0x9E3779B97F4A7C15ULL;
        for (size_t i = 0; i < N; ++i) {
            r = ct_rotr(r + (uint64_t)i * 0x9E3779B97F4A7C15ULL, (unsigned)(i & 63));
            uint8_t kb = static_cast<uint8_t>((r >> ((i & 7) * 8)) & 0xFF);
            data[i] = static_cast<uint8_t>(static_cast<uint8_t>(s[i]) ^ kb);
        }
    }

    std::string decrypt_runtime(uint64_t runtime_seed) const {
        char* buf = static_cast<char*>(std::malloc(N));
        if (!buf) return std::string{};
        uint64_t r = runtime_seed ^ 0x9E3779B97F4A7C15ULL;
        for (size_t i = 0; i < N; ++i) {
            r = ct_rotr(r + (uint64_t)i * 0x9E3779B97F4A7C15ULL, (unsigned)(i & 63));
            uint8_t kb = static_cast<uint8_t>((r >> ((i & 7) * 8)) & 0xFF);
            buf[i] = static_cast<char>(data[i] ^ kb);
        }
        std::string out(buf, N - 1);
        std::memset(buf, 0, N);
        std::free(buf);
        return out;
    }
};

#if defined(__GNUC__) || defined(__clang__)
# define OBF_NOINLINE __attribute__((noinline))
#else
# define OBF_NOINLINE
#endif

inline OBF_NOINLINE uint64_t derive_runtime_seed_from_addr() {
    uintptr_t p = reinterpret_cast<uintptr_t>(&derive_runtime_seed_from_addr);
    uint64_t seed = (uint64_t)p;
    seed ^= 0x0123456789ABCDEFULL;
    seed ^= (uint64_t)BUILD_SEED;
    seed ^= (seed << 13);
    seed ^= (seed >> 7);
    seed ^= (seed << 17);
    return seed;
}

#define OBF_LITERAL(s) ([]()->std::string { \
    static constexpr obf_constexpr::obf_blob<sizeof(s)> _blob{ s }; \
    uint64_t rt = obf_constexpr::derive_runtime_seed_from_addr(); \
    return _blob.decrypt_runtime(rt); \
}())

#define OBF_LITERAL_SAFE(s) ([]()->std::string { \
static constexpr obf_constexpr::obf_blob<sizeof(s)> _blob{ s }; \
return _blob.decrypt_runtime((uint64_t)BUILD_SEED); \
}())

} // namespace obf_constexpr
