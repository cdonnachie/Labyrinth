// Copyright (c) 2018-2020 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/labyrinth.h>

#include <util/translation.h>
#include <util/url.h>

#include <QCoreApplication>

#include <functional>
#include <string>

/** Translate string to current locale using Qt. */
extern const std::function<std::string(const char*)> G_TRANSLATION_FUN = [](const char* psz) {
    return QCoreApplication::translate("labyrinth-core", psz).toStdString();
};
UrlDecodeFn* const URL_DECODE = urlDecode;

int main(int argc, char* argv[]) { return GuiMain(argc, argv); }
