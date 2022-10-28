// Copyright (c) 2012-2020 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bloom.h>

#include <clientversion.h>
#include <key.h>
#include <key_io.h>
#include <merkleblock.h>
#include <primitives/block.h>
#include <random.h>
#include <serialize.h>
#include <streams.h>
#include <test/util/setup_common.h>
#include <uint256.h>
#include <util/strencodings.h>
#include <util/system.h>

#include <vector>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(bloom_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(bloom_create_insert_serialize)
{
    CBloomFilter filter(3, 0.01, 0, BLOOM_UPDATE_ALL);

    BOOST_CHECK_MESSAGE( !filter.contains(ParseHex("99108ad8ed9bb6274d3980bab5a85c048f0950c8")), "Bloom filter should be empty!");
    filter.insert(ParseHex("99108ad8ed9bb6274d3980bab5a85c048f0950c8"));
    BOOST_CHECK_MESSAGE( filter.contains(ParseHex("99108ad8ed9bb6274d3980bab5a85c048f0950c8")), "Bloom filter doesn't contain just-inserted object!");
    // One bit different in first byte
    BOOST_CHECK_MESSAGE(!filter.contains(ParseHex("19108ad8ed9bb6274d3980bab5a85c048f0950c8")), "Bloom filter contains something it shouldn't!");

    filter.insert(ParseHex("b5a2c786d9ef4658287ced5914b37a1b4aa32eee"));
    BOOST_CHECK_MESSAGE(filter.contains(ParseHex("b5a2c786d9ef4658287ced5914b37a1b4aa32eee")), "Bloom filter doesn't contain just-inserted object (2)!");

    filter.insert(ParseHex("b9300670b4c5366e95b2699e8b18bc75e5f729c5"));
    BOOST_CHECK_MESSAGE(filter.contains(ParseHex("b9300670b4c5366e95b2699e8b18bc75e5f729c5")), "Bloom filter doesn't contain just-inserted object (3)!");

    CDataStream stream(SER_NETWORK, PROTOCOL_VERSION);
    stream << filter;

    std::vector<unsigned char> vch = ParseHex("03614e9b050000000000000001");
    std::vector<char> expected(vch.size());

    for (unsigned int i = 0; i < vch.size(); i++)
        expected[i] = (char)vch[i];

    BOOST_CHECK_EQUAL_COLLECTIONS(stream.begin(), stream.end(), expected.begin(), expected.end());

    BOOST_CHECK_MESSAGE( filter.contains(ParseHex("99108ad8ed9bb6274d3980bab5a85c048f0950c8")), "Bloom filter doesn't contain just-inserted object!");
}

BOOST_AUTO_TEST_CASE(bloom_create_insert_serialize_with_tweak)
{
    // Same test as bloom_create_insert_serialize, but we add a nTweak of 100
    CBloomFilter filter(3, 0.01, 2147483649UL, BLOOM_UPDATE_ALL);

    filter.insert(ParseHex("99108ad8ed9bb6274d3980bab5a85c048f0950c8"));
    BOOST_CHECK_MESSAGE( filter.contains(ParseHex("99108ad8ed9bb6274d3980bab5a85c048f0950c8")), "Bloom filter doesn't contain just-inserted object!");
    // One bit different in first byte
    BOOST_CHECK_MESSAGE(!filter.contains(ParseHex("19108ad8ed9bb6274d3980bab5a85c048f0950c8")), "Bloom filter contains something it shouldn't!");

    filter.insert(ParseHex("b5a2c786d9ef4658287ced5914b37a1b4aa32eee"));
    BOOST_CHECK_MESSAGE(filter.contains(ParseHex("b5a2c786d9ef4658287ced5914b37a1b4aa32eee")), "Bloom filter doesn't contain just-inserted object (2)!");

    filter.insert(ParseHex("b9300670b4c5366e95b2699e8b18bc75e5f729c5"));
    BOOST_CHECK_MESSAGE(filter.contains(ParseHex("b9300670b4c5366e95b2699e8b18bc75e5f729c5")), "Bloom filter doesn't contain just-inserted object (3)!");

    CDataStream stream(SER_NETWORK, PROTOCOL_VERSION);
    stream << filter;

    std::vector<unsigned char> vch = ParseHex("03ce4299050000000100008001");
    std::vector<char> expected(vch.size());

    for (unsigned int i = 0; i < vch.size(); i++)
        expected[i] = (char)vch[i];

    BOOST_CHECK_EQUAL_COLLECTIONS(stream.begin(), stream.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(bloom_create_insert_key)
{
    std::string strSecret = std::string("5Kg1gnAjaLfKiwhhPpGS3QfRg2m6awQvaj98JCZBZQ5SuS2F15C");
    CKey key = DecodeSecret(strSecret);
    CPubKey pubkey = key.GetPubKey();
    std::vector<unsigned char> vchPubKey(pubkey.begin(), pubkey.end());

    CBloomFilter filter(2, 0.001, 0, BLOOM_UPDATE_ALL);
    filter.insert(vchPubKey);
    uint160 hash = pubkey.GetID();
    filter.insert(std::vector<unsigned char>(hash.begin(), hash.end()));

    CDataStream stream(SER_NETWORK, PROTOCOL_VERSION);
    stream << filter;

    std::vector<unsigned char> vch = ParseHex("038fc16b080000000000000001");
    std::vector<char> expected(vch.size());

    for (unsigned int i = 0; i < vch.size(); i++)
        expected[i] = (char)vch[i];

    BOOST_CHECK_EQUAL_COLLECTIONS(stream.begin(), stream.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(bloom_match)
{
    // Random test transaction (eefd9cac68db00f6b2284134e4c8c0e7d5b618c3915525de12c76cd426143fd4)
    CDataStream stream(ParseHex("0200000001e5f3ee72187314c7d86bff81f9acdc061df0608e15f651e4bf62360c687b9c1f000000006a47304402206c71e9eda8934b159ee8c1e094449658b602338868e39db8a7495b0db4af64480220165026947dfc66cbfbb9535150954b72ac590b758d8d8af05d57020b3aca6710012103049a53bd560daeebdcb7305c10f24a60d0443e71bb5c17902714246067c7f72bfdffffff0200ca9a3b000000001976a9141b2f03ec0ba306a303906c31a34d00b00a57262488acd0b66d18020000001600148c2d4a2d82e66dee428766acd7f87602712a0efc6d000000"), SER_DISK, CLIENT_VERSION);
    CTransaction tx(deserialize, stream);

    // And one which spends it (c9bedb875b1092fc19289624c4307544d65a27c399e3259517b795157943a073)
    unsigned char ch[] = { 0x02, 0x00, 0x00, 0x00, 0x01, 0xd4, 0x3f, 0x14, 0x26, 0xd4, 0x6c, 0xc7, 0x12, 0xde, 0x25, 0x55, 0x91, 0xc3, 0x18, 0xb6, 0xd5, 0xe7, 0xc0, 0xc8, 0xe4, 0x34, 0x41, 0x28, 0xb2, 0xf6, 0x00, 0xdb, 0x68, 0xac, 0x9c, 0xfd, 0xee, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x47, 0x30, 0x44, 0x02, 0x20, 0x24, 0x00, 0x37, 0xa8, 0x23, 0x84, 0x18, 0x12, 0xd9, 0x63, 0x8e, 0xf8, 0xc0, 0xf1, 0x0b, 0x36, 0x23, 0x15, 0x49, 0x63, 0xa6, 0xc0, 0x06, 0x31, 0xfd, 0x37, 0x5e, 0x03, 0xd1, 0x14, 0xb3, 0xf2, 0x02, 0x20, 0x5d, 0x06, 0x7f, 0xbb, 0xca, 0x18, 0x38, 0x92, 0xa8, 0x34, 0x46, 0xec, 0xc2, 0x78, 0xc0, 0xd7, 0x2d, 0x6f, 0x5b, 0x4b, 0xda, 0xa5, 0xa5, 0x4c, 0x40, 0x99, 0xe3, 0x0c, 0x4a, 0x01, 0x10, 0xbf, 0x01, 0x21, 0x03, 0xd3, 0x18, 0x0c, 0x31, 0xed, 0xc1, 0x75, 0x77, 0x80, 0x09, 0x72, 0xda, 0x63, 0x13, 0x24, 0x79, 0xfb, 0x54, 0x24, 0xcf, 0xcf, 0x24, 0x8f, 0x66, 0x4b, 0x6c, 0xc0, 0xf3, 0xb4, 0x3e, 0xa8, 0xf3, 0xfd, 0xff, 0xff, 0xff, 0x01, 0xe8, 0xdf, 0x97, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x19, 0x76, 0xa9, 0x14, 0xbe, 0x95, 0xbf, 0x1d, 0x1f, 0xd3, 0x4d, 0xb4, 0x6e, 0xf9, 0x63, 0xc7, 0xdb, 0x42, 0xb1, 0x81, 0xeb, 0x4f, 0xb4, 0xca, 0x88, 0xac, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00 };
    std::vector<unsigned char> vch(ch, ch + sizeof(ch) -1);
    CDataStream spendStream(vch, SER_DISK, CLIENT_VERSION);
    CTransaction spendingTx(deserialize, spendStream);

    CBloomFilter filter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(uint256S("0xeefd9cac68db00f6b2284134e4c8c0e7d5b618c3915525de12c76cd426143fd4"));
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match tx hash");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    // byte-reversed tx hash
    filter.insert(ParseHex("d43f1426d46cc712de255591c318b6d5e7c0c8e4344128b2f600db68ac9cfdee"));
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match manually serialized tx hash");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(ParseHex("304402206c71e9eda8934b159ee8c1e094449658b602338868e39db8a7495b0db4af64480220165026947dfc66cbfbb9535150954b72ac590b758d8d8af05d57020b3aca671001"));
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match input signature");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(ParseHex("03049a53bd560daeebdcb7305c10f24a60d0443e71bb5c17902714246067c7f72b"));
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match input pub key");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(ParseHex("1b2f03ec0ba306a303906c31a34d00b00a572624"));
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match output address");
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(spendingTx), "Simple Bloom filter didn't add output");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(ParseHex("8c2d4a2d82e66dee428766acd7f87602712a0efc"));
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match output address");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(COutPoint(uint256S("0x1f9c7b680c3662bfe451f6158e60f01d06dcacf981ff6bd8c714731872eef3e5"), 0));
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match COutPoint");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    COutPoint prevOutPoint(uint256S("0x1f9c7b680c3662bfe451f6158e60f01d06dcacf981ff6bd8c714731872eef3e5"), 0);
    {
        std::vector<unsigned char> data(32 + sizeof(unsigned int));
        memcpy(data.data(), prevOutPoint.hash.begin(), 32);
        memcpy(data.data()+32, &prevOutPoint.n, sizeof(unsigned int));
        filter.insert(data);
    }
    BOOST_CHECK_MESSAGE(filter.IsRelevantAndUpdate(tx), "Simple Bloom filter didn't match manually serialized COutPoint");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(uint256S("0x007f613a9f1633ca7315ad20595beb63a07f7cafed6715507f39f9d45b076b3f"));
    BOOST_CHECK_MESSAGE(!filter.IsRelevantAndUpdate(tx), "Simple Bloom filter matched random tx hash");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(ParseHex("a587ab681c231f32b5f208e97c5b975fe566d125"));
    BOOST_CHECK_MESSAGE(!filter.IsRelevantAndUpdate(tx), "Simple Bloom filter matched random address");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(COutPoint(uint256S("0x1f9c7b680c3662bfe451f6158e60f01d06dcacf981ff6bd8c714731872eef3e5"), 1));
    BOOST_CHECK_MESSAGE(!filter.IsRelevantAndUpdate(tx), "Simple Bloom filter matched COutPoint for an output we didn't care about");

    filter = CBloomFilter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    filter.insert(COutPoint(uint256S("0x007f613a9f1633ca7315ad20595beb63a07f7cafed6715507f39f9d45b076b3fs"), 0));
    BOOST_CHECK_MESSAGE(!filter.IsRelevantAndUpdate(tx), "Simple Bloom filter matched COutPoint for an output we didn't care about");
}

BOOST_AUTO_TEST_CASE(merkle_block_1)
{
    /**
    * Random test block (0000000282bbee4fa784b3ec68bb7d27c7fc4ea18e8255d759bc1ffbe488239d)
    *      with 9 txs.
    */
    CBlock block;
    CDataStream stream(ParseHex("000000208859791d91f1fdc755a31a2ceb73c0c858823bf20bdf1ab43d72257604000000c69763e28f772295303a8372096fbcbb9c12b9598785a4d099d4b02b81744cbc31b05063b9c8071d6e0000005e95590101000000c6f5e529c74492efb8d55defe066aff3e2fc3644d4bbbc322ae0e507916dba7e09010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff1a016e0431b05063083030303030303031094c61627972696e7468ffffffff0300ca9a3b000000001976a9145ee350cbff1fba6c004b25de65f537544700ee7b88acc0cf7318020000001976a91484c6881f7cbdb3a2200b594dcfd9270dc1b9e1eb88ac0000000000000000266a24aa21a9ed49be7e419b80c74055825e99553505765f37119d628244cf1d8f261e479692e40120000000000000000000000000000000000000000000000000000000000000000000000000020000000101e8a4140ae5749080180091b701375e89b1d13354843c679031e46e0f3dd336010000006a473044022022a8f50c21d66db1d85f24b7cac6c6a316a8f7279f46a2bbb9a5437c941a36f902201c2d355631a042ccc5d9727caee2b8d3e9c50cb4e95ec000b95f5f116e08edb70121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc0100000016001406ba589d91d9818813c5a7a343a506e18b1730a60000000002000000010105ae121d2aa68ea13b0bd04809a821a22641b44de8634512dbee9de45e934d010000006a47304402206e81f9d9135107fe5b7c8dcb38119486fd39d32ff63a7c8479d00dbc2e5ec4bf0220756b8f1eb16153edaf594099c5ef1bbb68d8a65debe12ec3bf8359b2de1d80ce0121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc010000001600142c1e571f447910332e1b7fbfdb0292ecf0d15e22000000000200000001e00c913e188e7f6ec4b345fcf387fa7bce7378bd0f7a928f3b8439acdbe87117010000006a47304402205fe60cb8e54703e79b3d0b514a85de6816371d1a6dbea93c5196d5b4f7df3c6302205c4a06106763481d69b1c542f319d8ffe9ffcf875c7c9285a6283bca9fd678a30121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014f56caa1763a6bbafae0753d68e7a99236bca3a1200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000189c55073612db9544c8690619d77b2d87411914ce3204754908c473e8ef4d03f010000006a4730440220098bfa37516dfb771965223c69f25a8ce043ec77183c4ca00ea48e51066f41360220053dfa462f7bf88444301b3c3552f497dbf7c53c4f9b8259694926f32b23f9890121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc010000001600145781b32310ea53af9185c160eb5ff07d5d113c2500ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000124f7ae77b35b8a6a6af1f98fd7b22b9c92b4d67b3f0e1b541e7dab2805e66b44010000006a47304402203950880fffcfb5734011a13bd687ed2b364e5e5eb26badfbc3507ce5505992ce0220232a8de014081a412f533580e42fcc988fafe66e7fb34eef0d4d9c824fd723a80121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014482b1815bb2495234089b37e4d71bfd69f7a3d9200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000178b595c382ecf7d1ef1090eb1a714c7b704141e3a4d81729b08f78066baa72a4010000006a47304402207ca2b584c0323c91bfcffadf3714fedd2f504467035c689dd48ce13ce2c94c1e0220207c18b03f089575c2f8bb8524057656f450341a02982b2e2c97cbf66c7f16b70121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc0100000016001490200e889130d1ffb1e16bcbea3d0cb16b4799a4000000000200000001b3b9d74385efb4e8bd7f72e1d999c00c79797d52b953e85fb3bce768a2310823010000006a47304402207b9d886c8b17adf2ba48e016e67198a3df3adcbf5efdd7e6e986fcaceb535c270220686a2a478c280abdbd6e402bb4fedd3a11693c3eff480f3293969f7a490268410121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc01000000160014411ec56790f0fda490312d6b90b28a594282f7410000000002000000013ebeff59bc2a93bab2b49abc5eed9086024cc5709921fed80fd40554a27160e7010000006a473044022051810355bdfeece92801b70e1b3afb90ab4e2b65e6e7002c55990e5b2f34829a022015f9001f55ba44504f27635c917e28fd4eb704ae0a529c3e51342d514b5205f30121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014e0fad10a0fd073029855db2cde664a1ba592f30200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000"), SER_NETWORK, PROTOCOL_VERSION);
    stream >> block;

    CBloomFilter filter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    // Match the last transaction
    filter.insert(uint256S("0xbf88c9e7af7c712695e02fb06612b33a38e6f494109b8259695236f486fa0dfd"));

    CMerkleBlock merkleBlock(block, filter);
    BOOST_CHECK_EQUAL(merkleBlock.header.GetHash().GetHex(), block.GetHash().GetHex());

    BOOST_CHECK_EQUAL(merkleBlock.vMatchedTxn.size(), 1U);
    std::pair<unsigned int, uint256> pair = merkleBlock.vMatchedTxn[0];

    BOOST_CHECK(merkleBlock.vMatchedTxn[0].second == uint256S("0xbf88c9e7af7c712695e02fb06612b33a38e6f494109b8259695236f486fa0dfd"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[0].first == 8);

    std::vector<uint256> vMatched;
    std::vector<unsigned int> vIndex;
    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);

    // Also match the 8th transaction
    filter.insert(uint256S("0xc7b7028dae33be9f3ec1c674426aad8c1e327084afb82555ba31ec132427e1f8"));
    merkleBlock = CMerkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 2);

    BOOST_CHECK(merkleBlock.vMatchedTxn[1] == pair);

    BOOST_CHECK(merkleBlock.vMatchedTxn[0].second == uint256S("0xc7b7028dae33be9f3ec1c674426aad8c1e327084afb82555ba31ec132427e1f8"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[0].first == 7);

    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);
}

BOOST_AUTO_TEST_CASE(merkle_block_2)
{
    /**
    * Random test block (000000203bec8474be24af71f68daf0ea58564ed398f01e959c4e628d351bfc9)
    *      with 4 txs.
    */
    CBlock block;
    CDataStream stream(ParseHex("00000020e2e9bbb2d37f664e57be2447f1214edd63fcd8168d340ca7a85e9f3426000000800ba300997eb1c0439b016a74548152f10d92e8e23d5eb7f07304d0a36d9eb754b95063e7df691d740000006f53f30204000000491e8aac19539ebc3b510f188b6bb41c981878af5fe1d46bc1e7114a5294156f04010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff1a01740454b95063083030303030303034094c61627972696e7468ffffffff0300ca9a3b000000001976a9145ee350cbff1fba6c004b25de65f537544700ee7b88aca4b07218020000001976a91484c6881f7cbdb3a2200b594dcfd9270dc1b9e1eb88ac0000000000000000266a24aa21a9edba6894953840b6ac81da1fef732082e98d011cbc192eae8344530b78c971b7230120000000000000000000000000000000000000000000000000000000000000000000000000020000000115874f8f594f177e0d7e0be73702c495889b1db0a74f6e6b8ed2b437d20141dc010000006a47304402203ca5249536f10ca893e7ae8ff7bd673217ef7e2a1e8298346fc6139a9248c99b02202f41a10975a30d347eb5ae922444d8cbb8b435b2d62476a810818c6f872f965301210226401110eedff73763201e587210d80eae210077e556e325716d84040c36c165fdffffff0200f90295000000001976a9141042e1e21e3cb7d78b0c4278a250fc5e1bc17b8888ac209e0195000000001976a914160ea53e570ac2007fb9407f4a7f1df23691a74888ac73000000020000000285501af041b60dde4022c74dcbdc071b1c3584b513da5d6b5aa9952cdafa48e5010000006a4730440220731aafb769500555628b3c8a284df858399e05e9eddd258a667861e28fb32992022025294d01ca339a439ddf66fa514967a7aa4d3d07318ea7edb9ccb4b83023b75401210374cd2a45ca682b9f4ca3ff14a56d0c6892761e79f8273b1a2f955722d4e1cc14fdffffff85501af041b60dde4022c74dcbdc071b1c3584b513da5d6b5aa9952cdafa48e5000000006a47304402207a68390e6c04874e3cca69294e1f24250920ed0b5452143a41964dae131c5bcb0220718cd947b3895ac076142c047194a0d5d7e632346ca2923a1d8c3991cbf687c001210226401110eedff73763201e587210d80eae210077e556e325716d84040c36c165fdffffff0200f90295000000001976a9141042e1e21e3cb7d78b0c4278a250fc5e1bc17b8888acc0fe0095000000001976a914160ea53e570ac2007fb9407f4a7f1df23691a74888ac73000000020000000235c47c5c4936c43a4dbf04f8d2e26120c3f0586c9836ac75ff29ec367a1225e4010000006a47304402207448e561c3a54f39c481cede290db4f9bf94bed56fc5bb514dea1153d85507cb022029a17f0fac387a069975995c9ded42a78141fa7f2c62255bdd54b46b0235b31601210374cd2a45ca682b9f4ca3ff14a56d0c6892761e79f8273b1a2f955722d4e1cc14fdffffff35c47c5c4936c43a4dbf04f8d2e26120c3f0586c9836ac75ff29ec367a1225e4000000006a47304402203dd42b223946f0f4be7612fce573104b0a151b7356ea660f1b0355f3ac63bb680220621cfcf5cc90a5e069d84790b9d0d0042c232ac252ac1abe7f882ea48d86869701210226401110eedff73763201e587210d80eae210077e556e325716d84040c36c165fdffffff0200f90295000000001976a9141042e1e21e3cb7d78b0c4278a250fc5e1bc17b8888ac605f0095000000001976a914160ea53e570ac2007fb9407f4a7f1df23691a74888ac73000000"), SER_NETWORK, PROTOCOL_VERSION);
    stream >> block;

    CBloomFilter filter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    // Match the first transaction
    filter.insert(uint256S("0xb96e896e962ee7c75bab79d3bc2305848056a00878b327b96d33df4bbe1850d6"));

    CMerkleBlock merkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 1);
    std::pair<unsigned int, uint256> pair = merkleBlock.vMatchedTxn[0];

    BOOST_CHECK(merkleBlock.vMatchedTxn[0].second == uint256S("0xb96e896e962ee7c75bab79d3bc2305848056a00878b327b96d33df4bbe1850d6"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[0].first == 0);

    std::vector<uint256> vMatched;
    std::vector<unsigned int> vIndex;
    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);

    // Match an output from the second transaction (the pubkey for address XcApmPbPNWSUa4Ly5qsboceuMNjRhoBaWZ)
    // This should match the third transaction because it spends the output matched
    // It also matches the fourth transaction, which spends to the pubkey again
    filter.insert(ParseHex("0226401110eedff73763201e587210d80eae210077e556e325716d84040c36c165"));

    merkleBlock = CMerkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 4);

    BOOST_CHECK(pair == merkleBlock.vMatchedTxn[0]);

    BOOST_CHECK(merkleBlock.vMatchedTxn[1].second == uint256S("0xe548fada2c95a95a6b5dda13b584351c1b07dccb4dc72240de0db641f01a5085"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[1].first == 1);

    BOOST_CHECK(merkleBlock.vMatchedTxn[2].second == uint256S("0xe425127a36ec29ff75ac36986c58f0c32061e2d2f804bf4d3ac436495c7cc435"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[2].first == 2);

    BOOST_CHECK(merkleBlock.vMatchedTxn[3].second == uint256S("0xde15bd23ab7224df9322345e0ebd8fecbbead3291887851a7e8048eb3ed7a111"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[3].first == 3);

    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);
}

BOOST_AUTO_TEST_CASE(merkle_block_2_with_update_none)
{
    /**
    * Random test block (0000003d03db9fd8dba0a40e42d29332fa7443246b60b4ec4f9e94b08891a7fc)
    *      with 4 txs.
    */
    CBlock block;
    CDataStream stream(ParseHex("00000020e4e1e8ddd83fae4ea0aca94a73dfb05b3569eeea3b3f4d7b094f9a3d9a000000e24cf18fad849e9767e538277147c6481ba5c0e2a10c8b725fd8183eb683fb473bc35063a2d3021e820000000d811e01090000004c4f389dccd22f113998d93f7381daf194337b946a65a1193762a9462553106104010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff1b028200043bc35063083030303030303039094c61627972696e7468ffffffff0300ca9a3b000000001976a9145ee350cbff1fba6c004b25de65f537544700ee7b88ac08b17218020000001976a91484c6881f7cbdb3a2200b594dcfd9270dc1b9e1eb88ac0000000000000000266a24aa21a9ed49b2719d912ec5b62d1868c51805718d953e3c75b17a81f8f49c1177512a002f0120000000000000000000000000000000000000000000000000000000000000000000000000020000000225d8a9de95d73265221182d127c56524eb64cf7cd8f9015cdb195c335a0921ba000000006a4730440220212e622a2f3cd1d5c14cfcd675f86fe7be72521f23f6cafc72efd75b3c8050450220608f7b223324a3a21325bd1fdab879406b4b09e588924894173f573a8c253466012103e97a7df9395510507369dcd4013a66b35b956d3c2feadbaaeedb4d23debe6249fdffffff0a68f157334256969c96648fefb4136353e8b37d2b291872d4b93a244807323c000000006a473044022039d51de49111c62d64b7befe2182abaff9f63a8005100c3fb9a2d0f52f75195202202b4a86d4f9b8c7590efacd1dc7c73ccadcef84b5d71e49d211edf51c341e37ff0121022dac6927a64b5f7e89d8f3d327734640d0988db1f437d8f1b53ad21b78a41868fdffffff02008d380c010000001976a9144967cd92a6691fe0866b43ad3d0d610680825e5a88ac049c3aa1010000001976a91428a8964ae038f8b8acf9102603523ad1a0054fab88ac810000000200000002f92d6e26d94ef30a6dbdcd116216ae8da2eeee1e3520bd7a229876e94403bb2a010000006a47304402201db216cf816f2c227a0c8f5018bf07717e36539bae7bb812a2560754f3b68d6202200541c9c91e983af9112c58caf107297ba9807ffec62dcc5930645ce368772c6d0121037f070effec91f5d060f479d8b309bffa2ec0754551c41609a42b99834f2e3901fdfffffff92d6e26d94ef30a6dbdcd116216ae8da2eeee1e3520bd7a229876e94403bb2a0000000069463043021f6f37cf2d4913811dc40621ca2a5614fafa20d6b12b9892790e1a1a7161abda022035857c5c01f295aeba2f93b0616941d41168e36061e3923fd889b4a3205b27a70121022dac6927a64b5f7e89d8f3d327734640d0988db1f437d8f1b53ad21b78a41868fdffffff01ec9672ad020000001976a91428a8964ae038f8b8acf9102603523ad1a0054fab88ac810000000200000001fdc03b241ad676abfcad14a18de1973625114add828778962bd5e4517015e147000000006a473044022047325debd996950385b7fbc94920fcfeb81184843b6ecf871f15e489e586b96902202b98b0772f4b237159c69017e4b301f81e8f0e96f0ff5bafcc160896bc2f538e0121037f070effec91f5d060f479d8b309bffa2ec0754551c41609a42b99834f2e3901fdffffff02008d380c010000001976a9144967cd92a6691fe0866b43ad3d0d610680825e5a88ac5ca439a1010000001976a91428a8964ae038f8b8acf9102603523ad1a0054fab88ac81000000"), SER_NETWORK, PROTOCOL_VERSION);
    stream >> block;

    CBloomFilter filter(10, 0.000001, 0, BLOOM_UPDATE_NONE);
    // Match the first transaction
    filter.insert(uint256S("0x58ecf257e72d0336cef647f496daf48590c01643619af6c62e0d6dada4130b14"));

    CMerkleBlock merkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 1);
    std::pair<unsigned int, uint256> pair = merkleBlock.vMatchedTxn[0];

    BOOST_CHECK(merkleBlock.vMatchedTxn[0].second == uint256S("0x58ecf257e72d0336cef647f496daf48590c01643619af6c62e0d6dada4130b14"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[0].first == 0);

    std::vector<uint256> vMatched;
    std::vector<unsigned int> vIndex;
    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);

    // Match an output from the second transaction (the pubkey for address XhNyWcknLKsnXtNawpFj3eN6J7KqjtvhJ7)
    // This should not match the third transaction though it spends the output matched
    // It will match the fourth transaction, which has another pay-to-pubkey output to the same address
    filter.insert(ParseHex("022dac6927a64b5f7e89d8f3d327734640d0988db1f437d8f1b53ad21b78a41868"));

    merkleBlock = CMerkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 3);

    BOOST_CHECK(pair == merkleBlock.vMatchedTxn[0]);

    BOOST_CHECK(merkleBlock.vMatchedTxn[1].second == uint256S("0x2abb0344e97698227abd20351eeeeea28dae166211cdbd6d0af34ed9266e2df9"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[1].first == 1);

    BOOST_CHECK(merkleBlock.vMatchedTxn[2].second == uint256S("0x47e1157051e4d52b96788782dd4a11253697e18da114adfcab76d61a243bc0fd"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[2].first == 2);

    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);
}

BOOST_AUTO_TEST_CASE(merkle_block_3_and_serialize)
{
    /**
    * Random test block (0000006d76f413d049ad998eb10aa4ac5da48da0e94c1332ad7d10f7515dc2e5)
    *      with 1 txs.
    */
    CBlock block;
    CDataStream stream(ParseHex("00000020fca79188b0949e4fecb4606b244374fa3293d2420ea4a0dbd89fdb033d0000001b68f0e2b4de832dd79243b2e372e1f88349bc996911bf071f2de5815197114f47c350633448031e830000009335c600090000009fccfa7153b619066a75fe09794172b1b2467f47d22ab6d67dc0721d50a4f1b401010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff1b0283000447c35063083030303030303039094c61627972696e7468ffffffff0300ca9a3b000000001976a9145ee350cbff1fba6c004b25de65f537544700ee7b88ac001a7118020000001976a91484c6881f7cbdb3a2200b594dcfd9270dc1b9e1eb88ac0000000000000000266a24aa21a9ede2f61c3f71d1defd3fa999dfa36953755c690689799962b48bebd836974e8cf90120000000000000000000000000000000000000000000000000000000000000000000000000"), SER_NETWORK, PROTOCOL_VERSION);
    stream >> block;

    CBloomFilter filter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    // Match the only transaction
    filter.insert(uint256S("0x4f11975181e52d1f07bf116999bc4983f8e172e3b24392d72d83deb4e2f0681b"));

    CMerkleBlock merkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 1);

    BOOST_CHECK(merkleBlock.vMatchedTxn[0].second == uint256S("0x4f11975181e52d1f07bf116999bc4983f8e172e3b24392d72d83deb4e2f0681b"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[0].first == 0);

    std::vector<uint256> vMatched;
    std::vector<unsigned int> vIndex;
    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);

    CDataStream merkleStream(SER_NETWORK, PROTOCOL_VERSION);
    merkleStream << merkleBlock;

    std::vector<unsigned char> vch = ParseHex("00000020fca79188b0949e4fecb4606b244374fa3293d2420ea4a0dbd89fdb033d0000001b68f0e2b4de832dd79243b2e372e1f88349bc996911bf071f2de5815197114f47c350633448031e830000009335c600090000009fccfa7153b619066a75fe09794172b1b2467f47d22ab6d67dc0721d50a4f1b401000000011b68f0e2b4de832dd79243b2e372e1f88349bc996911bf071f2de5815197114f0101");
    std::vector<char> expected(vch.size());

    for (unsigned int i = 0; i < vch.size(); i++)
        expected[i] = (char)vch[i];

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), merkleStream.begin(), merkleStream.end());
}

BOOST_AUTO_TEST_CASE(merkle_block_4)
{
    /**
    * Random test block (0000000282bbee4fa784b3ec68bb7d27c7fc4ea18e8255d759bc1ffbe488239d)
    *      with 9 txs.
    */
    CBlock block;
    CDataStream stream(ParseHex("000000208859791d91f1fdc755a31a2ceb73c0c858823bf20bdf1ab43d72257604000000c69763e28f772295303a8372096fbcbb9c12b9598785a4d099d4b02b81744cbc31b05063b9c8071d6e0000005e95590101000000c6f5e529c74492efb8d55defe066aff3e2fc3644d4bbbc322ae0e507916dba7e09010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff1a016e0431b05063083030303030303031094c61627972696e7468ffffffff0300ca9a3b000000001976a9145ee350cbff1fba6c004b25de65f537544700ee7b88acc0cf7318020000001976a91484c6881f7cbdb3a2200b594dcfd9270dc1b9e1eb88ac0000000000000000266a24aa21a9ed49be7e419b80c74055825e99553505765f37119d628244cf1d8f261e479692e40120000000000000000000000000000000000000000000000000000000000000000000000000020000000101e8a4140ae5749080180091b701375e89b1d13354843c679031e46e0f3dd336010000006a473044022022a8f50c21d66db1d85f24b7cac6c6a316a8f7279f46a2bbb9a5437c941a36f902201c2d355631a042ccc5d9727caee2b8d3e9c50cb4e95ec000b95f5f116e08edb70121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc0100000016001406ba589d91d9818813c5a7a343a506e18b1730a60000000002000000010105ae121d2aa68ea13b0bd04809a821a22641b44de8634512dbee9de45e934d010000006a47304402206e81f9d9135107fe5b7c8dcb38119486fd39d32ff63a7c8479d00dbc2e5ec4bf0220756b8f1eb16153edaf594099c5ef1bbb68d8a65debe12ec3bf8359b2de1d80ce0121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc010000001600142c1e571f447910332e1b7fbfdb0292ecf0d15e22000000000200000001e00c913e188e7f6ec4b345fcf387fa7bce7378bd0f7a928f3b8439acdbe87117010000006a47304402205fe60cb8e54703e79b3d0b514a85de6816371d1a6dbea93c5196d5b4f7df3c6302205c4a06106763481d69b1c542f319d8ffe9ffcf875c7c9285a6283bca9fd678a30121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014f56caa1763a6bbafae0753d68e7a99236bca3a1200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000189c55073612db9544c8690619d77b2d87411914ce3204754908c473e8ef4d03f010000006a4730440220098bfa37516dfb771965223c69f25a8ce043ec77183c4ca00ea48e51066f41360220053dfa462f7bf88444301b3c3552f497dbf7c53c4f9b8259694926f32b23f9890121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc010000001600145781b32310ea53af9185c160eb5ff07d5d113c2500ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000124f7ae77b35b8a6a6af1f98fd7b22b9c92b4d67b3f0e1b541e7dab2805e66b44010000006a47304402203950880fffcfb5734011a13bd687ed2b364e5e5eb26badfbc3507ce5505992ce0220232a8de014081a412f533580e42fcc988fafe66e7fb34eef0d4d9c824fd723a80121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014482b1815bb2495234089b37e4d71bfd69f7a3d9200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000178b595c382ecf7d1ef1090eb1a714c7b704141e3a4d81729b08f78066baa72a4010000006a47304402207ca2b584c0323c91bfcffadf3714fedd2f504467035c689dd48ce13ce2c94c1e0220207c18b03f089575c2f8bb8524057656f450341a02982b2e2c97cbf66c7f16b70121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc0100000016001490200e889130d1ffb1e16bcbea3d0cb16b4799a4000000000200000001b3b9d74385efb4e8bd7f72e1d999c00c79797d52b953e85fb3bce768a2310823010000006a47304402207b9d886c8b17adf2ba48e016e67198a3df3adcbf5efdd7e6e986fcaceb535c270220686a2a478c280abdbd6e402bb4fedd3a11693c3eff480f3293969f7a490268410121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc01000000160014411ec56790f0fda490312d6b90b28a594282f7410000000002000000013ebeff59bc2a93bab2b49abc5eed9086024cc5709921fed80fd40554a27160e7010000006a473044022051810355bdfeece92801b70e1b3afb90ab4e2b65e6e7002c55990e5b2f34829a022015f9001f55ba44504f27635c917e28fd4eb704ae0a529c3e51342d514b5205f30121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014e0fad10a0fd073029855db2cde664a1ba592f30200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000"), SER_NETWORK, PROTOCOL_VERSION);
    stream >> block;

    CBloomFilter filter(10, 0.000001, 0, BLOOM_UPDATE_ALL);
    // Match the last transaction
    filter.insert(uint256S("0xbf88c9e7af7c712695e02fb06612b33a38e6f494109b8259695236f486fa0dfd"));

    CMerkleBlock merkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 1);
    std::pair<unsigned int, uint256> pair = merkleBlock.vMatchedTxn[0];

    BOOST_CHECK(merkleBlock.vMatchedTxn[0].second == uint256S("bf88c9e7af7c712695e02fb06612b33a38e6f494109b8259695236f486fa0dfd"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[0].first == 8);

    std::vector<uint256> vMatched;
    std::vector<unsigned int> vIndex;
    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);

    // Also match the 4th transaction
    filter.insert(uint256S("0x62d1f95d3bbb96db7385d365120c30a68d76e19bd04e906b2763a8034a791e90"));
    merkleBlock = CMerkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    BOOST_CHECK(merkleBlock.vMatchedTxn.size() == 2);

    BOOST_CHECK(merkleBlock.vMatchedTxn[0].second == uint256S("0x62d1f95d3bbb96db7385d365120c30a68d76e19bd04e906b2763a8034a791e90"));
    BOOST_CHECK(merkleBlock.vMatchedTxn[0].first == 3);

    BOOST_CHECK(merkleBlock.vMatchedTxn[1] == pair);

    BOOST_CHECK(merkleBlock.txn.ExtractMatches(vMatched, vIndex) == block.hashMerkleRoot);
    BOOST_CHECK(vMatched.size() == merkleBlock.vMatchedTxn.size());
    for (unsigned int i = 0; i < vMatched.size(); i++)
        BOOST_CHECK(vMatched[i] == merkleBlock.vMatchedTxn[i].second);
}

BOOST_AUTO_TEST_CASE(merkle_block_4_test_update_none)
{
    /**
    * Random test block (0000000282bbee4fa784b3ec68bb7d27c7fc4ea18e8255d759bc1ffbe488239d)
    *      with 9 txs.
    */
    CBlock block;
    CDataStream stream(ParseHex("000000208859791d91f1fdc755a31a2ceb73c0c858823bf20bdf1ab43d72257604000000c69763e28f772295303a8372096fbcbb9c12b9598785a4d099d4b02b81744cbc31b05063b9c8071d6e0000005e95590101000000c6f5e529c74492efb8d55defe066aff3e2fc3644d4bbbc322ae0e507916dba7e09010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff1a016e0431b05063083030303030303031094c61627972696e7468ffffffff0300ca9a3b000000001976a9145ee350cbff1fba6c004b25de65f537544700ee7b88acc0cf7318020000001976a91484c6881f7cbdb3a2200b594dcfd9270dc1b9e1eb88ac0000000000000000266a24aa21a9ed49be7e419b80c74055825e99553505765f37119d628244cf1d8f261e479692e40120000000000000000000000000000000000000000000000000000000000000000000000000020000000101e8a4140ae5749080180091b701375e89b1d13354843c679031e46e0f3dd336010000006a473044022022a8f50c21d66db1d85f24b7cac6c6a316a8f7279f46a2bbb9a5437c941a36f902201c2d355631a042ccc5d9727caee2b8d3e9c50cb4e95ec000b95f5f116e08edb70121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc0100000016001406ba589d91d9818813c5a7a343a506e18b1730a60000000002000000010105ae121d2aa68ea13b0bd04809a821a22641b44de8634512dbee9de45e934d010000006a47304402206e81f9d9135107fe5b7c8dcb38119486fd39d32ff63a7c8479d00dbc2e5ec4bf0220756b8f1eb16153edaf594099c5ef1bbb68d8a65debe12ec3bf8359b2de1d80ce0121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc010000001600142c1e571f447910332e1b7fbfdb0292ecf0d15e22000000000200000001e00c913e188e7f6ec4b345fcf387fa7bce7378bd0f7a928f3b8439acdbe87117010000006a47304402205fe60cb8e54703e79b3d0b514a85de6816371d1a6dbea93c5196d5b4f7df3c6302205c4a06106763481d69b1c542f319d8ffe9ffcf875c7c9285a6283bca9fd678a30121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014f56caa1763a6bbafae0753d68e7a99236bca3a1200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000189c55073612db9544c8690619d77b2d87411914ce3204754908c473e8ef4d03f010000006a4730440220098bfa37516dfb771965223c69f25a8ce043ec77183c4ca00ea48e51066f41360220053dfa462f7bf88444301b3c3552f497dbf7c53c4f9b8259694926f32b23f9890121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc010000001600145781b32310ea53af9185c160eb5ff07d5d113c2500ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000124f7ae77b35b8a6a6af1f98fd7b22b9c92b4d67b3f0e1b541e7dab2805e66b44010000006a47304402203950880fffcfb5734011a13bd687ed2b364e5e5eb26badfbc3507ce5505992ce0220232a8de014081a412f533580e42fcc988fafe66e7fb34eef0d4d9c824fd723a80121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014482b1815bb2495234089b37e4d71bfd69f7a3d9200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000020000000178b595c382ecf7d1ef1090eb1a714c7b704141e3a4d81729b08f78066baa72a4010000006a47304402207ca2b584c0323c91bfcffadf3714fedd2f504467035c689dd48ce13ce2c94c1e0220207c18b03f089575c2f8bb8524057656f450341a02982b2e2c97cbf66c7f16b70121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc0100000016001490200e889130d1ffb1e16bcbea3d0cb16b4799a4000000000200000001b3b9d74385efb4e8bd7f72e1d999c00c79797d52b953e85fb3bce768a2310823010000006a47304402207b9d886c8b17adf2ba48e016e67198a3df3adcbf5efdd7e6e986fcaceb535c270220686a2a478c280abdbd6e402bb4fedd3a11693c3eff480f3293969f7a490268410121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac48f9d5dc01000000160014411ec56790f0fda490312d6b90b28a594282f7410000000002000000013ebeff59bc2a93bab2b49abc5eed9086024cc5709921fed80fd40554a27160e7010000006a473044022051810355bdfeece92801b70e1b3afb90ab4e2b65e6e7002c55990e5b2f34829a022015f9001f55ba44504f27635c917e28fd4eb704ae0a529c3e51342d514b5205f30121038060740d271c5463286ff3ff84e4f6813187cebbe4584e0e85506bd04f45efc2fdffffff0248f9d5dc01000000160014e0fad10a0fd073029855db2cde664a1ba592f30200ca9a3b000000001976a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac00000000"), SER_NETWORK, PROTOCOL_VERSION);
    stream >> block;

    CBloomFilter filter(10, 0.000001, 0, BLOOM_UPDATE_NONE);
    // Match the generation pubkey
    filter.insert(ParseHex("76a91484c6881f7cbdb3a2200b594dcfd9270dc1b9e1eb88ac"));
    // ...and the output address of the 4th transaction
    filter.insert(ParseHex("76a9144d5ebe5c4471ef240ecee3809546dcc7249d18dc88ac"));

    CMerkleBlock merkleBlock(block, filter);
    BOOST_CHECK(merkleBlock.header.GetHash() == block.GetHash());

    // We shouldn't match any outpoints (UPDATE_NONE)s
    BOOST_CHECK(!filter.contains(COutPoint(uint256S("0x71623505989ed6c1ce1107fd895d4c837ce33f958abbc38884fc2f8996dc03d7"), 0)));
    BOOST_CHECK(!filter.contains(COutPoint(uint256S("0xc7b7028dae33be9f3ec1c674426aad8c1e327084afb82555ba31ec132427e1f8"), 0)));
}

static std::vector<unsigned char> RandomData()
{
    uint256 r = InsecureRand256();
    return std::vector<unsigned char>(r.begin(), r.end());
}

BOOST_AUTO_TEST_CASE(rolling_bloom)
{
    SeedInsecureRand(SeedRand::ZEROS);
    g_mock_deterministic_tests = true;

    // last-100-entry, 1% false positive:
    CRollingBloomFilter rb1(100, 0.01);

    // Overfill:
    static const int DATASIZE=399;
    std::vector<unsigned char> data[DATASIZE];
    for (int i = 0; i < DATASIZE; i++) {
        data[i] = RandomData();
        rb1.insert(data[i]);
    }
    // Last 100 guaranteed to be remembered:
    for (int i = 299; i < DATASIZE; i++) {
        BOOST_CHECK(rb1.contains(data[i]));
    }

    // false positive rate is 1%, so we should get about 100 hits if
    // testing 10,000 random keys. We get worst-case false positive
    // behavior when the filter is as full as possible, which is
    // when we've inserted one minus an integer multiple of nElement*2.
    unsigned int nHits = 0;
    for (int i = 0; i < 10000; i++) {
        if (rb1.contains(RandomData()))
            ++nHits;
    }
    // Expect about 100 hits
    BOOST_CHECK_EQUAL(nHits, 75U);

    BOOST_CHECK(rb1.contains(data[DATASIZE-1]));
    rb1.reset();
    BOOST_CHECK(!rb1.contains(data[DATASIZE-1]));

    // Now roll through data, make sure last 100 entries
    // are always remembered:
    for (int i = 0; i < DATASIZE; i++) {
        if (i >= 100)
            BOOST_CHECK(rb1.contains(data[i-100]));
        rb1.insert(data[i]);
        BOOST_CHECK(rb1.contains(data[i]));
    }

    // Insert 999 more random entries:
    for (int i = 0; i < 999; i++) {
        std::vector<unsigned char> d = RandomData();
        rb1.insert(d);
        BOOST_CHECK(rb1.contains(d));
    }
    // Sanity check to make sure the filter isn't just filling up:
    nHits = 0;
    for (int i = 0; i < DATASIZE; i++) {
        if (rb1.contains(data[i]))
            ++nHits;
    }
    // Expect about 5 false positives
    BOOST_CHECK_EQUAL(nHits, 6U);

    // last-1000-entry, 0.01% false positive:
    CRollingBloomFilter rb2(1000, 0.001);
    for (int i = 0; i < DATASIZE; i++) {
        rb2.insert(data[i]);
    }
    // ... room for all of them:
    for (int i = 0; i < DATASIZE; i++) {
        BOOST_CHECK(rb2.contains(data[i]));
    }
    g_mock_deterministic_tests = false;
}

BOOST_AUTO_TEST_SUITE_END()
