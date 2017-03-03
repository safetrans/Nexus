/*******************************************************************************************
 
			(c) Hash(BEGIN(Satoshi[2010]), END(Sunny[2012])) == Videlicet[2017] ++
			
			(c) Copyright Nexus Developers 2014 - 2017
			
			http://www.opensource.org/licenses/mit-license.php
  
*******************************************************************************************/

#ifndef NEXUS_LLP_INCLUDE_NODE_H
#define NEXUS_LLP_INCLUDE_NODE_H

#include <queue>

template <typename CType> class CMajority;

class uint256;
class uint512;
class uint576;
class uint1024;

template <typename T> class mruset;

namespace Core
{
	class CBlock;
}

namespace LLP
{
	class CInv;
	class Timer;
	class CAddress;
	class DDOS_Filter;
	class MessageConnection;

	
	class CNode : public MessageConnection
	{
	public:
		
		/* Mutex for handling node inventory messages. */
		Mutex_t NODE_MUTEX;
		
		
		/* Basic Stats. */
		CAddress addrThisNode;
		std::string strNodeVersion;
		unsigned int LLP::PROTOCOL_VERSION;
		
		
		/* Positive: Node Stats. */
		unsigned int nLastMessageTime;
		unsigned int nNodeGenesisTime;
		unsigned int nNodeTotalBlocks;
		unsigned int nTotalObjectAcks;
		
		
		/* Negative: Node Stats. */
		unsigned int nTotalBans;
		unsigned int nTotalRejects;
		unsigned int nTotalBadSeeds;
		
		
		/* Node Identifier. */
		uint576 cTrustKey;
		
		
		/* Consensus Stats. */
		uint1024 hashLastCheckpoint;
		unsigned int nUnifiedOffset;
		
		
		/* Behavior Flags*/
		bool fClient;
		bool fInbound;
		bool fNetworkNode;
		
		
		/* Network Statistics. */
		unsigned int nNodeLatency; //milli-seconds
		unsigned int nLastPinging;
		
		
		/* Node Timers (Internal Statistics) */
		Timer cLatencyTimer;
		
		
		/* Unified Time Specific Variablesy. */
		CMajority<int> cTimeSamples;
		unsigned int nLastUnifiedCheck;
		
		
		/* Node Block Message Queues. */
		std::queue<Core::CBlock> queueBlock;
		
		
		/* Node Orphaned Transaction Messages. */
		std::queue<Core::CBlock> queueBlockOrphan;
		
		
		/* Node Transaction Message Queues. */
		std::queue<Core:;CTransaction> queueTransaction;
		
		
		/* Node Orphaned Transaction Messages. */
		std::queue<Core::CTransaction> queueTransactionOrphan;
		
		
		/* Known Inventory to make sure duplicate requests are not called out. */
		mruset<CInv> setInventoryKnown;
		
		
		/* Constructors for Message LLP Class. */
		CNode() : MessageConnection(){ }
		CNode( Socket_t SOCKET_IN, DDOS_Filter* DDOS_IN, bool isDDOS = false ) : MessageConnection( SOCKET_IN, DDOS_IN )
		{ ADDRESS = parse_ip(SOCKET_IN->remote_endpoint().address().to_string()); }
		
		
		/* Virtual Functions to Determine Behavior of Message LLP. */
		void Event(unsigned char EVENT, unsigned int LENGTH = 0);
		bool ProcessPacket();
		void PushVersion();
		
		
		/* Send an Address to Node. */
		void PushAddress(const CAddress& addr);

		
		/* Keep Track of the Inventory we Already have. */
		void AddInventoryKnown(const CInv& inv);

		
		/* Send Inventory We have. */
		void PushInventory(const CInv& inv);
		
		
		/* Send the DoS Score to DDOS Filter. */
		bool DoS(int nDoS, bool fReturn)
		{
			if(fDDOS)
				DDOS->rScore += nDoS;
			
			return fReturn;
		}
	};
	
	
	/* DoS Wrapper for Block Level Functions. */
	inline bool DoS(CNode* pfrom, int nDoS, bool fReturn)
	{
		if(pfrom)
			pfrom->DDOS->rScore += nDoS;
			
		return fReturn;
	}
}

#endif