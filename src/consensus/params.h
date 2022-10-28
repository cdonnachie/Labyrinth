// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LABYRINTH_CONSENSUS_PARAMS_H
#define LABYRINTH_CONSENSUS_PARAMS_H

#include <uint256.h>
#include <limits>

namespace Consensus {

enum DeploymentPos
{
    DEPLOYMENT_TESTDUMMY,
    MAX_VERSION_BITS_DEPLOYMENTS
    // NOTE: Also add new deployments to VersionBitsDeploymentInfo in versionbits.cpp
};

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;
    /** If lock in occurs, delay activation until at least this block
     *  height.  Note that activation will only occur on a retarget
     *  boundary.
     */
    int min_activation_height{0};

    /** Constant for nTimeout very far in the future. */
    static constexpr int64_t NO_TIMEOUT = std::numeric_limits<int64_t>::max();

    /** Special value for nStartTime indicating that the deployment is always active.
     *  This is useful for testing, as it means tests don't need to deal with the activation
     *  process (which takes at least 3 BIP9 intervals). Only tests that specifically test the
     *  behaviour during activation cannot use this. */
    static constexpr int64_t ALWAYS_ACTIVE = -1;

    /** Special value for nStartTime indicating that the deployment is never active.
     *  This is useful for integrating the code changes for a new feature
     *  prior to deploying it on some or all networks. */
    static constexpr int64_t NEVER_ACTIVE = -2;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /* Boolean trigger indicating whether BIP16 is active */
    bool BIP16Enabled;
    /* Boolean trigger indicating whether BIP34 is active */
    bool BIP34Enabled;
    /* Boolean trigger indicating whether BIP65 is active */
    bool BIP65Enabled;
    /* Boolean trigger indicating whether BIP66 is active */
    bool BIP66Enabled;
    /* Boolean trigger indicating whether CSV is active */
    bool CSVEnabled;
    /* Boolean trigger indicating whether Segwit is active */
    bool SegwitEnabled;
    /* Boolean trigger indicating whether Taproot is active */
    bool TaprootEnabled;
    /**
     * Minimum blocks for miner confirmation of the total predefined blocks, used for BIP9 deployments.
     * Examples: 1800 for 90% out of 2000, 1400 for 70% out of 2000 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    BIP9Deployment vDeployments[MAX_VERSION_BITS_DEPLOYMENTS];
    /** Proof of work parameters */
    uint256 powLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowDifficultyBlocks;
    int64_t nPowTargetSpacing;
    /** The best chain should have at least this much work */
    uint256 nMinimumChainWork;
    /** By default assume that the signatures in ancestors of this block are valid */
    uint256 defaultAssumeValid;
    /** Founder reward percentage */
    int64_t founderPercentage;
    /** Founder output script */
    std::vector <unsigned char> founderOutputHex;
    /** Genesis output script */
    std::vector <unsigned char> genesisOutputHex;
};
} // namespace Consensus

#endif // LABYRINTH_CONSENSUS_PARAMS_H
