// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LABYRINTH_WALLET_SALVAGE_H
#define LABYRINTH_WALLET_SALVAGE_H

#include <fs.h>
#include <streams.h>

struct bilingual_str;

bool RecoverDatabaseFile(const fs::path& file_path, bilingual_str& error, std::vector<bilingual_str>& warnings);

#endif // LABYRINTH_WALLET_SALVAGE_H
