// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <founder.h>

#include <chainparams.h>
#include <validation.h>

CAmount GetFounderAmount() {
    const Consensus::Params& consensusParams = Params().GetConsensus();

    CAmount blockSubsidy = GetBlockSubsidy(::ChainActive().Height() + 1, consensusParams);
    CAmount founderAmount = blockSubsidy * consensusParams.founderPercentage / 100;

    return founderAmount;
}

CScript GetFounderScript() {
    const Consensus::Params& consensusParams = Params().GetConsensus();

    CScript founderScript = CScript(consensusParams.founderOutputHex.begin(), consensusParams.founderOutputHex.end());

    return founderScript;
}

bool CheckFounderOutput(std::vector<CTxOut> coinbaseTransactionOutputs) {
    CAmount founderPaymentAmount = GetFounderAmount();
    CScript founderPaymentScript = GetFounderScript();
    
    for (CTxOut coinbaseTransactionOutput: coinbaseTransactionOutputs) {
        if (coinbaseTransactionOutput.scriptPubKey == founderPaymentScript && coinbaseTransactionOutput.nValue >= founderPaymentAmount) {
            return true;
        }
    }

    return false;
}