// Copyright (c) 2013-2018 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <hash.h>
#include <crypto/common.h>
#include <crypto/hmac_sha512.h>
#include <crypto/kawpow/include/kawpow/progpow.hpp>

#include <string>

inline uint32_t ROTL32(uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

unsigned int MurmurHash3(unsigned int nHashSeed, Span<const unsigned char> vDataToHash)
{
    uint32_t h1 = nHashSeed;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const int nblocks = vDataToHash.size() / 4;
    const uint8_t* blocks = vDataToHash.data();

    for (int i = 0; i < nblocks; ++i) {
        uint32_t k1 = ReadLE32(blocks + i*4);

        k1 *= c1;
        k1 = ROTL32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    const uint8_t* tail = vDataToHash.data() + nblocks * 4;

    uint32_t k1 = 0;

    switch (vDataToHash.size() & 3) {
        case 3:
            k1 ^= tail[2] << 16;
        case 2:
            k1 ^= tail[1] << 8;
        case 1:
            k1 ^= tail[0];
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
    }

    h1 ^= vDataToHash.size();
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

void BIP32Hash(const ChainCode &chainCode, unsigned int nChild, unsigned char header, const unsigned char data[32], unsigned char output[64])
{
    unsigned char num[4];
    num[0] = (nChild >> 24) & 0xFF;
    num[1] = (nChild >> 16) & 0xFF;
    num[2] = (nChild >>  8) & 0xFF;
    num[3] = (nChild >>  0) & 0xFF;
    CHMAC_SHA512(chainCode.begin(), chainCode.size()).Write(&header, 1).Write(data, 32).Write(num, 4).Finalize(output);
}

uint256 SHA256Uint256(const uint256& input)
{
    uint256 result;
    CSHA256().Write(input.begin(), 32).Finalize(result.begin());
    return result;
}

uint256 Hash(const CBlockHeader& blockHeader, uint256& mix_hash)
{
    static kawpow::epoch_context_ptr context{nullptr, nullptr};

    // Get the context from the block height
    const auto epoch_number = kawpow::get_epoch_number(blockHeader.nHeight);

    if (!context || context->epoch_number != epoch_number)
        context = kawpow::create_epoch_context(epoch_number);

    // Build the header_hash
    uint256 nHeaderHash = blockHeader.GetHeaderHash();
    const auto header_hash = to_hash256(nHeaderHash.GetHex());

    // ProgPow hash
    const auto result = progpow::hash(*context, blockHeader.nHeight, header_hash, blockHeader.nNonce);

    mix_hash = uint256S(to_hex(result.mix_hash));
    return uint256S(to_hex(result.final_hash));
}

uint256 HashMix(const CBlockHeader& blockHeader)
{
    // Build the header_hash
    uint256 nHeaderHash = blockHeader.GetHeaderHash();
    const auto header_hash = to_hash256(nHeaderHash.GetHex());

    // ProgPow hash
    const auto result = progpow::hash_no_verify(blockHeader.nHeight, header_hash, to_hash256(blockHeader.mix_hash.GetHex()), blockHeader.nNonce);

    return uint256S(to_hex(result));
}

CHashWriter TaggedHash(const std::string& tag)
{
    CHashWriter writer(SER_GETHASH, 0);
    uint256 taghash;
    CSHA256().Write((const unsigned char*)tag.data(), tag.size()).Finalize(taghash.begin());
    writer << taghash << taghash;
    return writer;
}