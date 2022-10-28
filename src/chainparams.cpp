// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, const CScript& founderOutputScript, uint32_t nTime, uint64_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward, const CAmount& founderReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 0x1f00ffff << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));

    txNew.vout.resize(2);
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    txNew.vout[1].nValue = founderReward;
    txNew.vout[1].scriptPubKey = founderOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000062b7fd1f5e, ver=4, hashPrevBlock=00000000000000, mix=fce0ae9233d6ff, nHeight=0, hashMerkleRoot=b5cfc9a2963397, nTime=1666152000, nBits=0x1f00ffff, nNonce=98245, vtx=1)
 *   CTransaction(hash=b5cfc9a2963397, ver=1, vin.size=1, vout.size=2, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 0004ffff001f01043e424243202d2031392f4f63742f32303232202d205275737369612061646d697473204b686572736f6e2074656e736520756e646572207368656c6c696e67)
 *     CTxOut(nValue=90.00000000, scriptPubKey=76a914add0c29bca7b6725)
 *     CTxOut(nValue=10.00000000, scriptPubKey=76a914bbea8e2d1ae283f2)
 *   vMerkleTree: b3f34e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint64_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward, const CAmount& founderReward, std::vector <unsigned char>& genesisOutputHex, std::vector <unsigned char>& founderOutputHex)
{
    const char* pszTimestamp = "BBC - 19/Oct/2022 - Russia admits Kherson tense under shelling";

    const CScript genesisOutputScript = CScript(genesisOutputHex.begin(), genesisOutputHex.end());
    const CScript founderOutputScript = CScript(founderOutputHex.begin(), founderOutputHex.end());

    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, founderOutputScript, nTime, nNonce, nBits, nVersion, genesisReward, founderReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.nSubsidyHalvingInterval = 1440000;
        consensus.BIP16Enabled = true;
        consensus.BIP34Enabled = true;
        consensus.BIP65Enabled = true;
        consensus.BIP66Enabled = true;
        consensus.CSVEnabled = true;
        consensus.SegwitEnabled = true;
        consensus.TaprootEnabled = true;
        consensus.powLimit = uint256S("0x0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowDifficultyBlocks = 20;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1800;
        consensus.nMinerConfirmationWindow = 2000;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000010001");
        consensus.defaultAssumeValid = uint256S("0x000062b7fd1f5e3be4b3fd39aa9209ea88c47df6668d19e41ddbaf7f0acfa63e");

        consensus.founderPercentage = 10;
        consensus.founderOutputHex = {0x76,0xa9,0x14,0x5e,0xe3,0x50,0xcb,0xff,0x1f,0xba,0x6c,0x00,0x4b,0x25,0xde,0x65,0xf5,0x37,0x54,0x47,0x00,0xee,0x7b,0x88,0xac};
        consensus.genesisOutputHex = {0x76,0xa9,0x14,0x30,0x6a,0x07,0x48,0x7c,0xc4,0x3a,0x75,0x0f,0xb2,0xbe,0x23,0x26,0x63,0x1f,0xb4,0x0a,0x57,0x8f,0xbd,0x88,0xac};

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0x28;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0x93;
        nDefaultPort = 4570;
        nPruneAfterHeight = 65000;

        m_assumed_blockchain_size = 25;
        m_assumed_chain_state_size = 5;

        genesis = CreateGenesisBlock(1666152000, 98245, 0x1f00ffff, 4, 90 * COIN, 10 * COIN, consensus.genesisOutputHex, consensus.founderOutputHex);
        consensus.hashGenesisBlock = genesis.GetHash(genesis.mix_hash);

        assert(consensus.hashGenesisBlock == uint256S("0x000062b7fd1f5e3be4b3fd39aa9209ea88c47df6668d19e41ddbaf7f0acfa63e"));
        assert(genesis.hashMerkleRoot == uint256S("0xb5cfc9a2963397f55992c7465f4b44961c2ab8b44ea7c8d2608c3e1ad417dfc1"));
        assert(genesis.mix_hash == uint256S("0xfce0ae9233d6ff6c4930784749395684e29c3257d1b72f9e9824ab680ccf5d0d"));

        // vSeeds.emplace_back("dns-primary-mainnet.labyrinthcore.org");
        // vSeeds.emplace_back("dns-secondary-mainnet.labyrinthcore.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,26);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "xb";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;

        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0x000062b7fd1f5e3be4b3fd39aa9209ea88c47df6668d19e41ddbaf7f0acfa63e")}
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.nSubsidyHalvingInterval = 1440000;
        consensus.BIP16Enabled = true;
        consensus.BIP34Enabled = true;
        consensus.BIP65Enabled = true;
        consensus.BIP66Enabled = true;
        consensus.CSVEnabled = true;
        consensus.SegwitEnabled = true;
        consensus.TaprootEnabled = true;
        consensus.powLimit = uint256S("0x000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowDifficultyBlocks =  20;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1400;
        consensus.nMinerConfirmationWindow = 2000;
        
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000001000");
        consensus.defaultAssumeValid = uint256S("0x0005fe1b4d38315b1c0145b012ac0d67e021558fc8ffb99937999f054b911d99");

        consensus.founderPercentage = 10;
        consensus.founderOutputHex = {0x76,0xa9,0x14,0xa4,0xe8,0x4e,0x46,0xe5,0x04,0xfb,0xb5,0xb6,0xa9,0x74,0xa2,0x5a,0xe1,0xa1,0x96,0xa7,0x37,0x20,0x08,0x88,0xac};
        consensus.genesisOutputHex = {0x76,0xa9,0x14,0x04,0xfa,0x9c,0x64,0x25,0xd8,0x8b,0x9c,0xe8,0x16,0xf3,0x3a,0xaf,0x1d,0x86,0x3a,0x37,0x86,0xf2,0x6d,0x88,0xac};

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x9b;
        pchMessageStart[1] = 0xf7;
        pchMessageStart[2] = 0x0e;
        pchMessageStart[3] = 0x4c;
        nDefaultPort = 14570;
        nPruneAfterHeight = 65000;

        m_assumed_blockchain_size = 10;
        m_assumed_chain_state_size = 2;

        genesis = CreateGenesisBlock(1666152000, 7913, 0x1f0fffff, 4, 90 * COIN, 10 * COIN, consensus.genesisOutputHex, consensus.founderOutputHex);
        consensus.hashGenesisBlock = genesis.GetHash(genesis.mix_hash);

        assert(consensus.hashGenesisBlock == uint256S("0x0005fe1b4d38315b1c0145b012ac0d67e021558fc8ffb99937999f054b911d99"));
        assert(genesis.hashMerkleRoot == uint256S("0xfacb86d896ce4023c13882d2bcf35e0103fb9f34a39e71252507e06ba74ff4e5"));
        assert(genesis.mix_hash == uint256S("0x8e63f2d637af14e50cf35eaf3f105fde7aa30afd3469ec6fffd1025eb49a0c1c"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // vSeeds.emplace_back("dns-primary-testnet.labyrinthcore.org");
        // vSeeds.emplace_back("dns-secondary-testnet.labyrinthcore.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,53);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tn";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;

        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0x0005fe1b4d38315b1c0145b012ac0d67e021558fc8ffb99937999f054b911d99")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams() {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.nSubsidyHalvingInterval = 1440;
        consensus.BIP16Enabled = true;
        consensus.BIP34Enabled = true;
        consensus.BIP65Enabled = true;
        consensus.BIP66Enabled = true;
        consensus.CSVEnabled = true;
        consensus.SegwitEnabled = true;
        consensus.TaprootEnabled = true;
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowDifficultyBlocks = 20;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 140;
        consensus.nMinerConfirmationWindow = 200;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000002");
        consensus.defaultAssumeValid = uint256S("0x17308639acca98f6a6264806fc792f5231ea8e5b6e3d53c4241a3dd2099a23e4");

        consensus.founderPercentage = 20;
        consensus.founderOutputHex = {0x76,0xa9,0x14,0xbb,0xea,0x8e,0x2d,0x1a,0xe2,0x83,0xf2,0x80,0x61,0xf6,0xf3,0x58,0x26,0x67,0x0d,0x66,0x84,0x81,0xfe,0x88,0xac};
        consensus.genesisOutputHex = {0x76,0xa9,0x14,0xad,0xd0,0xc2,0x9b,0xca,0x7b,0x67,0x25,0x4d,0x49,0x22,0x86,0x28,0x3f,0xf6,0x9d,0x63,0x78,0xb2,0xd6,0x88,0xac};

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0x67;
        pchMessageStart[2] = 0xc6;
        pchMessageStart[3] = 0x44;
        nDefaultPort = 24570;
        nPruneAfterHeight = 3500;

        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1666152000, 0, 0x207fffff, 4, 80 * COIN, 20 * COIN, consensus.genesisOutputHex, consensus.founderOutputHex);
        consensus.hashGenesisBlock = genesis.GetHash(genesis.mix_hash);

        assert(consensus.hashGenesisBlock == uint256S("0x17308639acca98f6a6264806fc792f5231ea8e5b6e3d53c4241a3dd2099a23e4"));
        assert(genesis.hashMerkleRoot == uint256S("0x5f2dcf8594c80eda43609b6713555d59a4a1f2d78e6443e8e34758f5951a2a52"));
        assert(genesis.mix_hash == uint256S("0x25b3e7976b7872f208fe5df287426439e3a0913eeb55b79ab99b93976cfec3d4"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,60);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rs";

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;

        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("0x17308639acca98f6a6264806fc792f5231ea8e5b6e3d53c4241a3dd2099a23e4")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
    }
};

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN) {
        return std::unique_ptr<CChainParams>(new CMainParams());
    } else if (chain == CBaseChainParams::TESTNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    } else if (chain == CBaseChainParams::REGTEST) {
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    }
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(gArgs, network);
}
