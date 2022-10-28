// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SRC_FOUNDER_PAYMENT_H_
#define SRC_FOUNDER_PAYMENT_H_

#include <string>
#include <script/standard.h>

CAmount GetFounderAmount();
CScript GetFounderScript();

bool CheckFounderOutput(std::vector<CTxOut> coinbaseTransactionOutputs);

#endif /* SRC_FOUNDER_PAYMENT_H_ */