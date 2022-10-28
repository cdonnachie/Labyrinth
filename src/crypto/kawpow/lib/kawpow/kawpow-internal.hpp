// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <crypto/kawpow/include/kawpow/kawpow.hpp>
#include <crypto/kawpow/lib/kawpow/endianness.hpp>

#include <memory>
#include <vector>

extern "C" struct kawpow_epoch_context_full : kawpow_epoch_context
{
    kawpow_hash1024* full_dataset;

    constexpr kawpow_epoch_context_full(int epoch, int light_num_items,
        const kawpow_hash512* light, const uint32_t* l1, int dataset_num_items,
        kawpow_hash1024* dataset) noexcept
      : kawpow_epoch_context{epoch, light_num_items, light, l1, dataset_num_items},
        full_dataset{dataset}
    {}
};

namespace kawpow
{
inline bool is_less_or_equal(const hash256& a, const hash256& b) noexcept
{
    for (size_t i = 0; i < (sizeof(a) / sizeof(a.word64s[0])); ++i)
    {
        if (be::uint64(a.word64s[i]) > be::uint64(b.word64s[i]))
            return false;
        if (be::uint64(a.word64s[i]) < be::uint64(b.word64s[i]))
            return true;
    }
    return true;
}

inline bool is_equal(const hash256& a, const hash256& b) noexcept
{
    return std::memcmp(a.bytes, b.bytes, sizeof(a)) == 0;
}

void build_light_cache(hash512 cache[], int num_items, const hash256& seed) noexcept;

hash512 calculate_dataset_item_512(const epoch_context& context, int64_t index) noexcept;
hash1024 calculate_dataset_item_1024(const epoch_context& context, uint32_t index) noexcept;
hash2048 calculate_dataset_item_2048(const epoch_context& context, uint32_t index) noexcept;

namespace generic
{
using hash_fn_512 = hash512 (*)(const uint8_t* data, size_t size);
using build_light_cache_fn = void (*)(hash512 cache[], int num_items, const hash256& seed);

void build_light_cache(
    hash_fn_512 hash_fn, hash512 cache[], int num_items, const hash256& seed) noexcept;

epoch_context_full* create_epoch_context(
    build_light_cache_fn build_fn, int epoch_number, bool full) noexcept;

}  // namespace generic

}  // namespace kawpow
