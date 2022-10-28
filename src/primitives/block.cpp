// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>

uint256 CBlockHeader::GetHash() const
{
    return HashMix(*this);
}

uint256 CBlockHeader::GetHash(uint256& mix_hash) const
{
    return Hash(*this, mix_hash);
}

uint256 CBlockHeader::GetHeaderHash() const
{
    CKAWPOWInput input{*this};
    return SerializeHash(input);
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, mix=%s, headerhash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        mix_hash.GetHex(),
        GetHeaderHash().GetHex(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}