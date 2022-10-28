// Copyright (c) 2018-2020 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <bench/nanobench.h>

#include <bech32.h>
#include <util/strencodings.h>

#include <string>
#include <vector>


static void Bech32Encode(benchmark::Bench& bench)
{
    std::vector<uint8_t> v = ParseHex("bddfa46887019416c6c7db3ca6ee0dac12a8968b4594e88460fbe79ce48d0eaf");
    std::vector<unsigned char> tmp = {0};
    tmp.reserve(1 + 32 * 8 / 5);
    ConvertBits<8, 5, true>([&](unsigned char c) { tmp.push_back(c); }, v.begin(), v.end());
    bench.batch(v.size()).unit("byte").run([&] {
        bech32::Encode(bech32::Encoding::BECH32, "bc", tmp);
    });
}


static void Bech32Decode(benchmark::Bench& bench)
{
    std::string addr = "xb1qelm82sxmt2lpcsmqvc4gy0x9pul4qwhejpglaj";
    bench.batch(addr.size()).unit("byte").run([&] {
        bech32::Decode(addr);
    });
}


BENCHMARK(Bech32Encode);
BENCHMARK(Bech32Decode);
