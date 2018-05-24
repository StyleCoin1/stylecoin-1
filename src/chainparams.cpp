// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
	uint8_t addr[16];
	uint16_t port;
};

#include "chainparamsseeds.h"
//
// Main network
//

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
	// It'll only connect to one or two seed nodes because once it connects,
	// it'll get a pile of addresses with newer timestamps.
	// Seed nodes are given a random 'last seen time' of between one and two
	// weeks ago.
	const int64_t nOneWeek = 7 * 24 * 60 * 60;
	for (unsigned int k = 0; k < count; ++k)
	{
		struct in_addr ip;
		unsigned int i = data[k], t;

		// -- convert to big endian
		t = (i & 0x000000ff) << 24u
			| (i & 0x0000ff00) << 8u
			| (i & 0x00ff0000) >> 8u
			| (i & 0xff000000) >> 24u;

		memcpy(&ip, &t, sizeof(ip));

		CAddress addr(CService(ip, port));
		addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
		vSeedsOut.push_back(addr);
	}
}

class CMainParams : public CChainParams {
public:
	CMainParams() {
		// The message start string is designed to be unlikely to occur in normal data.
		// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		// a large 4-byte int at any alignment.
		pchMessageStart[0] = 0xa1;
		pchMessageStart[1] = 0xbc;
		pchMessageStart[2] = 0xfe;
		pchMessageStart[3] = 0x17;
		vAlertPubKey = ParseHex("04a956420eabbb8a7106385003fef77896538a382a0dcc389ff45f3c98751d9af423a066689757666259351198a8a2a628a1fd644c3232678c5845384c744ff8d7");
		nDefaultPort = 15808;
		nRPCPort = 63928;
		bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);

		// Build the genesis block. Note that the output of the genesis coinbase cannot
		// be spent as it did not originally exist in the database.
		//
		//CBlock(hash=0000ebc8051bff80f7946f4420efb219e66f66b89fdc1df0ed8a30b428bf0033, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=12630d16a97f24b287c8c2594dda5fb98c9e6c70fc61d44191931ea2aa08dc90, nTime=1393221600, nBits=1e0fffff, nNonce=164482, vtx=1, vchBlockSig=)
		//  Coinbase(hash=59589791e1, nTime=1393221600, ver=1, vin.size=1, vout.size=1, nLockTime=0)
		//    CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a24323020466562203230313420426974636f696e2041544d7320636f6d6520746f20555341)
		//    CTxOut(empty)
		//  vMerkleTree: 59589791e1
		const char* pszTimestamp = "NyTimes.com 2018/05/23 How Muellers..";
		std::vector<CTxIn> vin;
		vin.resize(1);
		vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
		std::vector<CTxOut> vout;
		vout.resize(1);
		vout[0].SetEmpty();
		CTransaction txNew(1, 1527086153, vin, vout, 0);
		genesis.vtx.push_back(txNew);
		genesis.hashPrevBlock = 0;
		genesis.hashMerkleRoot = genesis.BuildMerkleTree();
		genesis.nVersion = 1;
		genesis.nTime = 1527086153;
		genesis.nBits = bnProofOfWorkLimit.GetCompact();
		genesis.nNonce = 160569;

		if (true  && (genesis.GetHash() != hashGenesisBlock)) {

		                // This will figure out a valid hash and Nonce if you're
		                // creating a different genesis block:
		                    uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
		                    while (genesis.GetHash() > hashTarget)
		                       {
		                           ++genesis.nNonce;
		                           if (genesis.nNonce == 0)
		                           {
		                               printf("NONCE WRAPPED, incrementing time");
		                               ++genesis.nTime;
		                           }
		                       }
		}

		hashGenesisBlock = genesis.GetHash();
		LogPrintf("genesis.nTime = %d\n", genesis.nTime);
		LogPrintf("genesis.nNonce = %d\n", genesis.nNonce);
		LogPrintf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
		LogPrintf("genesis.hashMerkleRoot = %s\n", genesis.hashMerkleRoot.ToString().c_str());

		assert(genesis.hashMerkleRoot == uint256("0x9d0721d50597fe829775ab4b574251a7818c4822eb4d505d2fb217e588c426f0"));
		assert(hashGenesisBlock == uint256("0x000059b00a8735812562b81dabdc4928ec1bab3abf3e806a942069656a002284"));

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 85);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 153);
		base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1, 40);
		base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

		vSeeds.push_back(CDNSSeedData("0", "sty-seednode-1.giize.com"));
		convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);

		nPoolMaxTransactions = 3;
		//strSporkKey = "046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";
		//strMasternodePaymentsPubKey = "046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";
		strDarksendPoolDummyAddress = "CRaVnHZizkREBg6yBzcpy6TBLn4B5GbUva";
		nLastPOWBlock = 43200;
		nPOSStartBlock = 3;
	}


	virtual const CBlock& GenesisBlock() const { return genesis; }
	virtual Network NetworkID() const { return CChainParams::MAIN; }

	virtual const vector<CAddress>& FixedSeeds() const {
		return vFixedSeeds;
	}
protected:
	CBlock genesis;
	vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
	CTestNetParams() {
		// The message start string is designed to be unlikely to occur in normal data.
		// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		// a large 4-byte int at any alignment.
		pchMessageStart[0] = 0xa1;
		pchMessageStart[1] = 0xbc;
		pchMessageStart[2] = 0xfe;
		pchMessageStart[3] = 0x17;
		bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
		vAlertPubKey = ParseHex("04a983220ea7a38a7106385003fef77896538a382a0dcc389cc45f3c98751d9af423a097789757556259351198a8aaa628a1fd644c3232678c5845384c744ff8d7");
		nDefaultPort = 15803;
		nRPCPort = 63922;
		strDataDir = "testnet";
		hashGenesisBlock = genesis.GetHash();

		// Modify the testnet genesis block so the timestamp is valid for a later start.
		genesis.nTime = 1527086153;
		genesis.nBits = bnProofOfWorkLimit.GetCompact();
		genesis.nNonce = 160569;

		if (true  && (genesis.GetHash() != hashGenesisBlock)) {

		                // This will figure out a valid hash and Nonce if you're
		                // creating a different genesis block:
		                    uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
		                    while (genesis.GetHash() > hashTarget)
		                       {
		                           ++genesis.nNonce;
		                           if (genesis.nNonce == 0)
		                           {
		                               printf("NONCE WRAPPED, incrementing time");
		                               ++genesis.nTime;
		                           }
		                       }
		}


		hashGenesisBlock = genesis.GetHash();
		LogPrintf("genesisTN.nTime = %d\n", genesis.nTime);
		LogPrintf("genesisTN.nNonce = %d\n", genesis.nNonce);
		LogPrintf("genesisTN.GetHash = %s\n", genesis.GetHash().ToString().c_str());
		LogPrintf("genesisTN.hashMerkleRoot = %s\n", genesis.hashMerkleRoot.ToString().c_str());

		assert(hashGenesisBlock == uint256("0x000059b00a8735812562b81dabdc4928ec1bab3abf3e806a942069656a002284"));

		vFixedSeeds.clear();
		vSeeds.clear();

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 127);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
		base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1, 40);
		base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

		convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);

		nLastPOWBlock = 0x7fffffff;
	}
	virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
	return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
	switch (network) {
	case CChainParams::MAIN:
		pCurrentParams = &mainParams;
		break;
	case CChainParams::TESTNET:
		pCurrentParams = &testNetParams;
		break;
	default:
		assert(false && "Unimplemented network");
		return;
	}
}

bool SelectParamsFromCommandLine() {

	bool fTestNet = GetBoolArg("-testnet", false);

	if (fTestNet) {
		SelectParams(CChainParams::TESTNET);
	}
	else {
		SelectParams(CChainParams::MAIN);
	}
	return true;
}
