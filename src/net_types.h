// Copyright (c) 2019 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LABYRINTH_NET_TYPES_H
#define LABYRINTH_NET_TYPES_H

#include <map>

class CBanEntry;
class CSubNet;

using banmap_t = std::map<CSubNet, CBanEntry>;

#endif // LABYRINTH_NET_TYPES_H
