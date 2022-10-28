// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LABYRINTH_QT_LABYRINTHADDRESSVALIDATOR_H
#define LABYRINTH_QT_LABYRINTHADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class LabyrinthAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit LabyrinthAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Labyrinth address widget validator, checks for a valid labyrinth address.
 */
class LabyrinthAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit LabyrinthAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // LABYRINTH_QT_LABYRINTHADDRESSVALIDATOR_H
