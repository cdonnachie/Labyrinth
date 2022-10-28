// Copyright (c) 2015-2019 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <chainparams.h>
#include <pow.h>
#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(pow_tests, BasicTestingSetup)

/* Test calculation of next difficulty target with no constraints applying */
BOOST_AUTO_TEST_CASE(get_next_work)
{
    const auto chainParams = CreateChainParams(*m_node.args, CBaseChainParams::MAIN);
    const auto chainParamsTest = CreateChainParams(*m_node.args, CBaseChainParams::TESTNET);

    CBlockIndex blockIndexPrev1 = CBlockIndex();
    blockIndexPrev1.nHeight = 30;
    blockIndexPrev1.nTime = 1663083095;
    blockIndexPrev1.nBits = 0x1f00ffff;

    CBlockIndex blockIndexPrev2 = CBlockIndex();
    blockIndexPrev2.nHeight = 29;
    blockIndexPrev2.nTime = 1663083062;
    blockIndexPrev2.nBits = 0x1f00ffff;
    blockIndexPrev1.pprev = &blockIndexPrev2;

    CBlockIndex blockIndexPrev3 = CBlockIndex();
    blockIndexPrev3.nHeight = 28;
    blockIndexPrev3.nTime = 1663083027;
    blockIndexPrev3.nBits = 0x1f00ffff;
    blockIndexPrev2.pprev = &blockIndexPrev3;

    CBlockIndex blockIndexPrev4 = CBlockIndex();
    blockIndexPrev4.nHeight = 27;
    blockIndexPrev4.nTime = 1663082988;
    blockIndexPrev4.nBits = 0x1f00ffff;
    blockIndexPrev3.pprev = &blockIndexPrev4;

    CBlockIndex blockIndexPrev5 = CBlockIndex();
    blockIndexPrev5.nHeight = 26;
    blockIndexPrev5.nTime = 1663082948;
    blockIndexPrev5.nBits = 0x1f00ffff;
    blockIndexPrev4.pprev = &blockIndexPrev5;

    CBlockIndex blockIndexPrev6 = CBlockIndex();
    blockIndexPrev6.nHeight = 25;
    blockIndexPrev6.nTime = 1663082909;
    blockIndexPrev6.nBits = 0x1f00ffff;
    blockIndexPrev5.pprev = &blockIndexPrev6;

    CBlockIndex blockIndexPrev7 = CBlockIndex();
    blockIndexPrev7.nHeight = 24;
    blockIndexPrev7.nTime = 1663082875;
    blockIndexPrev7.nBits = 0x1f00ffff;
    blockIndexPrev6.pprev = &blockIndexPrev7;

    CBlockIndex blockIndexPrev8 = CBlockIndex();
    blockIndexPrev8.nHeight = 23;
    blockIndexPrev8.nTime = 1663082843;
    blockIndexPrev8.nBits = 0x1f00ffff;
    blockIndexPrev7.pprev = &blockIndexPrev8;

    CBlockIndex blockIndexPrev9 = CBlockIndex();
    blockIndexPrev9.nHeight = 22;
    blockIndexPrev9.nTime = 1663082805;
    blockIndexPrev9.nBits = 0x1f00ffff;
    blockIndexPrev8.pprev = &blockIndexPrev9;

    CBlockIndex blockIndexPrev10 = CBlockIndex();
    blockIndexPrev10.nHeight = 21;
    blockIndexPrev10.nTime = 1663082769;
    blockIndexPrev10.nBits = 0x1f00ffff;
    blockIndexPrev9.pprev = &blockIndexPrev10;

    CBlockIndex blockIndexPrev11 = CBlockIndex();
    blockIndexPrev11.nHeight = 20;
    blockIndexPrev11.nTime = 1663082736;
    blockIndexPrev11.nBits = 0x1f00ffff;
    blockIndexPrev10.pprev = &blockIndexPrev11;

    CBlockIndex blockIndexPrev12 = CBlockIndex();
    blockIndexPrev12.nHeight = 19;
    blockIndexPrev12.nTime = 1663082696;
    blockIndexPrev12.nBits = 0x1f00ffff;
    blockIndexPrev11.pprev = &blockIndexPrev12;

    CBlockIndex blockIndexPrev13 = CBlockIndex();
    blockIndexPrev13.nHeight = 18;
    blockIndexPrev13.nTime = 1663082658;
    blockIndexPrev13.nBits = 0x1f00ffff;
    blockIndexPrev12.pprev = &blockIndexPrev13;

    CBlockIndex blockIndexPrev14 = CBlockIndex();
    blockIndexPrev14.nHeight = 17;
    blockIndexPrev14.nTime = 1663082624;
    blockIndexPrev14.nBits = 0x1f00ffff;
    blockIndexPrev13.pprev = &blockIndexPrev14;

    CBlockIndex blockIndexPrev15 = CBlockIndex();
    blockIndexPrev15.nHeight = 16;
    blockIndexPrev15.nTime = 1663082589;
    blockIndexPrev15.nBits = 0x1f00ffff;
    blockIndexPrev14.pprev = &blockIndexPrev15;

    CBlockIndex blockIndexPrev16 = CBlockIndex();
    blockIndexPrev16.nHeight = 15;
    blockIndexPrev16.nTime = 1663082557;
    blockIndexPrev16.nBits = 0x1f00ffff;
    blockIndexPrev15.pprev = &blockIndexPrev16;

    CBlockIndex blockIndexPrev17 = CBlockIndex();
    blockIndexPrev17.nHeight = 14;
    blockIndexPrev17.nTime = 1663082521;
    blockIndexPrev17.nBits = 0x1f00ffff;
    blockIndexPrev16.pprev = &blockIndexPrev17;

    CBlockIndex blockIndexPrev18 = CBlockIndex();
    blockIndexPrev18.nHeight = 13;
    blockIndexPrev18.nTime = 1663082448;
    blockIndexPrev18.nBits = 0x1f00ffff;
    blockIndexPrev17.pprev = &blockIndexPrev18;

    CBlockIndex blockIndexPrev19 = CBlockIndex();
    blockIndexPrev19.nHeight = 12;
    blockIndexPrev19.nTime = 1663082412;
    blockIndexPrev19.nBits = 0x1f00ffff;
    blockIndexPrev18.pprev = &blockIndexPrev19;

    CBlockIndex blockIndexPrev20 = CBlockIndex();
    blockIndexPrev20.nHeight = 11;
    blockIndexPrev20.nTime = 1663082373;
    blockIndexPrev20.nBits = 0x1f00ffff;
    blockIndexPrev19.pprev = &blockIndexPrev20;

    CBlockIndex blockIndexPrev21 = CBlockIndex();
    blockIndexPrev21.nHeight = 10;
    blockIndexPrev21.nTime = 1663082337;
    blockIndexPrev21.nBits = 0x1f00ffff;
    blockIndexPrev20.pprev = &blockIndexPrev21;

    CBlockIndex blockIndexPrev22 = CBlockIndex();
    blockIndexPrev22.nHeight = 9;
    blockIndexPrev22.nTime = 1663082302;
    blockIndexPrev22.nBits = 0x1f00ffff;
    blockIndexPrev21.pprev = &blockIndexPrev22;

    CBlockIndex blockIndexPrev23 = CBlockIndex();
    blockIndexPrev23.nHeight = 8;
    blockIndexPrev23.nTime = 1663082270;
    blockIndexPrev23.nBits = 0x1f00ffff;
    blockIndexPrev22.pprev = &blockIndexPrev23;

    CBlockIndex blockIndexPrev24 = CBlockIndex();
    blockIndexPrev24.nHeight = 7;
    blockIndexPrev24.nTime = 1663082231;
    blockIndexPrev24.nBits = 0x1f00ffff;
    blockIndexPrev23.pprev = &blockIndexPrev24;

    CBlockIndex blockIndexPrev25 = CBlockIndex();
    blockIndexPrev25.nHeight = 6;
    blockIndexPrev25.nTime = 1663082194;
    blockIndexPrev25.nBits = 0x1f00ffff;
    blockIndexPrev24.pprev = &blockIndexPrev25;

    CBlockIndex blockIndexPrev26 = CBlockIndex();
    blockIndexPrev26.nHeight = 5;
    blockIndexPrev26.nTime = 1663082156;
    blockIndexPrev26.nBits = 0x1f00ffff;
    blockIndexPrev25.pprev = &blockIndexPrev26;

    CBlockIndex blockIndexPrev27 = CBlockIndex();
    blockIndexPrev27.nHeight = 4;
    blockIndexPrev27.nTime = 1663082122;
    blockIndexPrev27.nBits = 0x1f00ffff;
    blockIndexPrev26.pprev = &blockIndexPrev27;

    CBlockIndex blockIndexPrev28 = CBlockIndex();
    blockIndexPrev28.nHeight = 3;
    blockIndexPrev28.nTime = 1663082091;
    blockIndexPrev28.nBits = 0x1f00ffff;
    blockIndexPrev27.pprev = &blockIndexPrev28;

    CBlockIndex blockIndexPrev29 = CBlockIndex();
    blockIndexPrev29.nHeight = 2;
    blockIndexPrev29.nTime = 1663082055;
    blockIndexPrev29.nBits = 0x1f00ffff;
    blockIndexPrev28.pprev = &blockIndexPrev29;

    CBlockIndex blockIndexPrev30 = CBlockIndex();
    blockIndexPrev30.nHeight = 1;
    blockIndexPrev30.nTime = 1663082020;
    blockIndexPrev30.nBits = 0x1f00ffff;
    blockIndexPrev29.pprev = &blockIndexPrev30;

    CBlockHeader blockHeader;
    blockHeader.nTime = 1663083131;

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev1, &blockHeader, chainParams->GetConsensus()), 0x1f009a06);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev2, &blockHeader, chainParams->GetConsensus()), 0x1f009aaa);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev3, &blockHeader, chainParams->GetConsensus()), 0x1f009aaa);

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev3, &blockHeader, chainParams->GetConsensus()), 0x1f009aaa);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev4, &blockHeader, chainParams->GetConsensus()), 0x1f00992b);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev5, &blockHeader, chainParams->GetConsensus()), 0x1f0098f5);

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev6, &blockHeader, chainParams->GetConsensus()), 0x1f009887);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev7, &blockHeader, chainParams->GetConsensus()), 0x1f009962);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev8, &blockHeader, chainParams->GetConsensus()), 0x1f0099cf);

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev9, &blockHeader, chainParams->GetConsensus()), 0x1f009851);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev10, &blockHeader, chainParams->GetConsensus()), 0x1f009851);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev11, &blockHeader, chainParams->GetConsensus()), 0x1f0098be);

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev1, &blockHeader, chainParamsTest->GetConsensus()), 0x1f009a06);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev2, &blockHeader, chainParamsTest->GetConsensus()), 0x1f009aaa);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev3, &blockHeader, chainParamsTest->GetConsensus()), 0x1f009aaa);

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev3, &blockHeader, chainParamsTest->GetConsensus()), 0x1f009aaa);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev4, &blockHeader, chainParamsTest->GetConsensus()), 0x1f00992b);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev5, &blockHeader, chainParamsTest->GetConsensus()), 0x1f0098f5);

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev6, &blockHeader, chainParamsTest->GetConsensus()), 0x1f009887);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev7, &blockHeader, chainParamsTest->GetConsensus()), 0x1f009962);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev8, &blockHeader, chainParamsTest->GetConsensus()), 0x1f0099cf);

    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev9, &blockHeader, chainParamsTest->GetConsensus()), 0x1f09fff6);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev10, &blockHeader, chainParamsTest->GetConsensus()), 0x1f09fff6);
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev11, &blockHeader, chainParamsTest->GetConsensus()), 0x1f09fff6);

    blockHeader.nTime = 1663083731;
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev1, &blockHeader, chainParamsTest->GetConsensus()), 0x1f09fff6);
    blockHeader.nTime = 1663084331;
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev1, &blockHeader, chainParamsTest->GetConsensus()), 0x1f09fff6);

    blockHeader.nTime = 1663086731;
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev1, &blockHeader, chainParamsTest->GetConsensus()), 0x1f09fff6);
    blockHeader.nTime = 1663090331;
    BOOST_CHECK_EQUAL(GetNextWorkRequired(&blockIndexPrev1, &blockHeader, chainParamsTest->GetConsensus()), 0x1f09fff6);
}

/* Test the constraint on the upper bound for next work */
BOOST_AUTO_TEST_CASE(get_next_work_pow_limit)
{
    const auto chainParams = CreateChainParams(*m_node.args, CBaseChainParams::MAIN);
    CBlockIndex pindexLast;
    pindexLast.nHeight = 0;
    pindexLast.nTime = 1663082020;
    pindexLast.nBits = 0x1f00ffff;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequired(&pindexLast, chainParams->GetConsensus()), 0x1f00ffff);
}

/* Test the constraint on the lower bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_lower_limit_actual)
{
    const auto chainParams = CreateChainParams(*m_node.args, CBaseChainParams::MAIN);
    CBlockIndex pindexLast;
    pindexLast.nHeight = 0;
    pindexLast.nTime = 1663082020;
    pindexLast.nBits = 0x1f00ffff;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequired(&pindexLast, chainParams->GetConsensus()), 0x1f00ffff);
}

/* Test the constraint on the upper bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_upper_limit_actual)
{
    const auto chainParams = CreateChainParams(*m_node.args, CBaseChainParams::MAIN);
    CBlockIndex pindexLast;
    pindexLast.nHeight = 0;
    pindexLast.nTime = 1663082020;
    pindexLast.nBits = 0x1f00ffff;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequired(&pindexLast, chainParams->GetConsensus()), 0x1f00ffff);
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_negative_target)
{
    const auto consensus = CreateChainParams(*m_node.args, CBaseChainParams::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    nBits = UintToArith256(consensus.powLimit).GetCompact(true);
    hash.SetHex("0x1");
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_overflow_target)
{
    const auto consensus = CreateChainParams(*m_node.args, CBaseChainParams::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits = ~0x00800000;
    hash.SetHex("0x1");
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_too_easy_target)
{
    const auto consensus = CreateChainParams(*m_node.args, CBaseChainParams::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    arith_uint256 nBits_arith = UintToArith256(consensus.powLimit);
    nBits_arith *= 2;
    nBits = nBits_arith.GetCompact();
    hash.SetHex("0x1");
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_biger_hash_than_target)
{
    const auto consensus = CreateChainParams(*m_node.args, CBaseChainParams::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    arith_uint256 hash_arith = UintToArith256(consensus.powLimit);
    nBits = hash_arith.GetCompact();
    hash_arith *= 2; // hash > nBits
    hash = ArithToUint256(hash_arith);
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_zero_target)
{
    const auto consensus = CreateChainParams(*m_node.args, CBaseChainParams::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    arith_uint256 hash_arith{0};
    nBits = hash_arith.GetCompact();
    hash = ArithToUint256(hash_arith);
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(GetBlockProofEquivalentTime_test)
{
    const auto chainParams = CreateChainParams(*m_node.args, CBaseChainParams::MAIN);
    std::vector<CBlockIndex> blocks(10000);
    for (int i = 0; i < 10000; i++) {
        blocks[i].pprev = i ? &blocks[i - 1] : nullptr;
        blocks[i].nHeight = i;
        blocks[i].nTime = 1269211443 + i * chainParams->GetConsensus().nPowTargetSpacing;
        blocks[i].nBits = 0x207fffff; /* target 0x7fffff000... */
        blocks[i].nChainWork = i ? blocks[i - 1].nChainWork + GetBlockProof(blocks[i - 1]) : arith_uint256(0);
    }

    for (int j = 0; j < 1000; j++) {
        CBlockIndex *p1 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p2 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p3 = &blocks[InsecureRandRange(10000)];

        int64_t tdiff = GetBlockProofEquivalentTime(*p1, *p2, *p3, chainParams->GetConsensus());
        BOOST_CHECK_EQUAL(tdiff, p1->GetBlockTime() - p2->GetBlockTime());
    }
}

void sanity_check_chainparams(const ArgsManager& args, std::string chainName)
{
    const auto chainParams = CreateChainParams(args, chainName);
    const auto consensus = chainParams->GetConsensus();

    // hash genesis is correct
    BOOST_CHECK_EQUAL(consensus.hashGenesisBlock, chainParams->GenesisBlock().GetHash());

    // genesis nBits is positive, doesn't overflow and is lower than powLimit
    arith_uint256 pow_compact;
    bool neg, over;
    pow_compact.SetCompact(chainParams->GenesisBlock().nBits, &neg, &over);
    BOOST_CHECK(!neg && pow_compact != 0);
    BOOST_CHECK(!over);
    BOOST_CHECK(UintToArith256(consensus.powLimit) >= pow_compact);
}

BOOST_AUTO_TEST_CASE(ChainParams_MAIN_sanity)
{
    sanity_check_chainparams(*m_node.args, CBaseChainParams::MAIN);
}

BOOST_AUTO_TEST_CASE(ChainParams_REGTEST_sanity)
{
    sanity_check_chainparams(*m_node.args, CBaseChainParams::REGTEST);
}

BOOST_AUTO_TEST_CASE(ChainParams_TESTNET_sanity)
{
    sanity_check_chainparams(*m_node.args, CBaseChainParams::TESTNET);
}

BOOST_AUTO_TEST_SUITE_END()
