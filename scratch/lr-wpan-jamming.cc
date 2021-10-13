/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * 
 */

/*
 * We simulate a Reactive Jammer, which has a reaction time of 250 microseconds.
 * The jammer jams a legitimate packet. The LrWpanPhy detects the interference
 * and displays some statistics.
 */
#include <ns3/log.h>
#include <ns3/core-module.h>
#include <ns3/lr-wpan-module.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/simulator.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/packet.h>

#include <iostream>

using namespace ns3;

/**
 * This callback is called after a Mcps has successfully received a
 * frame and wants to deliver it to the higher layer.
 */
static void DataIndication(McpsDataIndicationParams params, Ptr<Packet> p) {
	NS_LOG_UNCOND(
			"Received packet of size " << p->GetSize () << "\n Packet content: " << p->ToString() <<"\n");
}

/**
 * This callback is called after a McpsDataRequest has been called from
 * the higher layer.
 */
static void DataConfirm(McpsDataConfirmParams params) {
	//NS_LOG_UNCOND("LrWpanMcpsDataConfirmStatus = " << params.m_status);
}

/**
 * Logs, when a PHY instance starts transmitting a packet.
 */
static void TransmissionBegin(std::string context, Ptr<const Packet> packet) {
	packet->EnablePrinting();
	NS_LOG_UNCOND(
			Simulator::Now().GetSeconds() << "s: "<<context << " PACKET TRANSMITTING...:" << "\n" << packet->ToString() << "\n");
}

static void TxSignalParams(std::string context,
		Ptr<SpectrumSignalParameters> params) {
	Ptr<LrWpanPhy> phyphy = DynamicCast<LrWpanPhy>(params->txPhy);
	NS_LOG_UNCOND(
			Simulator::Now().GetSeconds() << "s: "<<context << " Header: " << phyphy->ToString());

}

/**
 * Logs, when a PHY instance receives a packet in the StartRx() method.
 */
static void ReceptionBegin(std::string context, Ptr<const Packet> packet) {
	packet->EnablePrinting();
	NS_LOG_UNCOND(
			Simulator::Now().GetSeconds() << "s: "<<context << " PACKET RECEIVING...\n");

}

/**
 * Logs, when a Packet has been sent by the MAC instance.
 */
static void MacSentPkt(std::string context, Ptr<const Packet> packet,
		uint8_t num_retries, uint8_t csma_backoffs) {
	packet->EnablePrinting();
	NS_LOG_UNCOND(
			Simulator::Now().GetSeconds() << "s: "<<context << ": Packet transmitted.\n");
}

int main(int argc, char *argv[]) {

	GlobalValue::Bind("ChecksumEnabled", BooleanValue(true));
	bool verbose = false;
	bool jrb_enabled = false;
	bool early_jrb = false;
	int o_payload = 100;
	int j_payload = 1;

	CommandLine cmd;

	cmd.AddValue("verbose", "turn on all log components", verbose);
	cmd.AddValue("jrb", "enable JRB", jrb_enabled);
	cmd.AddValue("early-jrb", "define jrb position (before or after phr)",
			early_jrb);
	cmd.AddValue("o_payload", "define the payload size of an ordinary packet",
			o_payload);
	cmd.AddValue("j_payload", "define the payload size of a jamming packet",
			j_payload);

	cmd.Parse(argc, argv);

	//Check if payload sizes are valid:
	if (!(o_payload >= 0 && o_payload <= LrWpanPhy::aMaxPhyPacketSize - 13
			&& j_payload >= 0 && j_payload <= LrWpanPhy::aMaxPhyPacketSize - 13)) {
		NS_LOG_UNCOND(
				"Error: Payload sizes have to be between 0 and " << LrWpanPhy::aMaxPhyPacketSize -13 << ". Please try again with valid values.");
		return 0;
	}

	LrWpanHelper lrWpanHelper;
	if (verbose) {
		lrWpanHelper.EnableLogComponents();
	}

	// Create 3 nodes, and a NetDevice for each one
	Ptr<Node> n0 = CreateObject<Node>();
	Ptr<Node> n1 = CreateObject<Node>();
	Ptr<Node> n2 = CreateObject<Node>();

	Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice>();
	Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice>();
	Ptr<LrWpanNetDevice> dev2 = CreateObject<LrWpanNetDevice>();

	dev0->SetAddress(Mac16Address("00:01"));
	dev1->SetAddress(Mac16Address("00:02"));
	dev2->SetAddress(Mac16Address("00:03"));

	// Each device must be attached to the same channel
	Ptr<SingleModelSpectrumChannel> channel = CreateObject<
			SingleModelSpectrumChannel>();
	Ptr<LogDistancePropagationLossModel> propModel = CreateObject<
			LogDistancePropagationLossModel>();
	Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<
			ConstantSpeedPropagationDelayModel>();
	channel->AddPropagationLossModel(propModel);
	channel->SetPropagationDelayModel(delayModel);

	dev0->SetChannel(channel);
	dev1->SetChannel(channel);
	dev2->SetChannel(channel);

	// To complete configuration, a LrWpanNetDevice must be added to a node
	n0->AddDevice(dev0);
	n1->AddDevice(dev1);
	n2->AddDevice(dev2);

	dev0->GetPhy()->TraceConnect("PhyTxBegin", std::string("phy0"),
			MakeCallback(&TransmissionBegin));
	dev1->GetPhy()->TraceConnect("PhyTxBegin", std::string("phy1"),
			MakeCallback(&TransmissionBegin));
	dev2->GetPhy()->TraceConnect("PhyTxBegin", std::string("jammer"),
			MakeCallback(&TransmissionBegin));
	dev0->GetPhy()->TraceConnect("PhyRxBegin", std::string("phy0"),
			MakeCallback(&ReceptionBegin));
	dev1->GetPhy()->TraceConnect("PhyRxBegin", std::string("phy1"),
			MakeCallback(&ReceptionBegin));
	dev2->GetPhy()->TraceConnect("PhyRxBegin", std::string("jammer"),
			MakeCallback(&ReceptionBegin));
	dev0->GetChannel()->TraceConnect("TxSigParams", std::string("phy0"),
			MakeCallback(&TxSignalParams));
	dev1->GetChannel()->TraceConnect("TxSigParams", std::string("phy1"),
			MakeCallback(&TxSignalParams));
	dev2->GetChannel()->TraceConnect("TxSigParams", std::string("jammer"),
			MakeCallback(&TxSignalParams));
	dev0->GetMac()->TraceConnect("MacSentPkt", std::string("phy0"),
			MakeCallback(&MacSentPkt));
	dev1->GetMac()->TraceConnect("MacSentPkt", std::string("phy1"),
			MakeCallback(&MacSentPkt));
	dev2->GetMac()->TraceConnect("MacSentPkt", std::string("jammer"),
			MakeCallback(&MacSentPkt));

	Ptr<ConstantPositionMobilityModel> sender0Mobility = CreateObject<
			ConstantPositionMobilityModel>();
	sender0Mobility->SetPosition(Vector(0, 0, 0));
	dev0->GetPhy()->SetMobility(sender0Mobility);
	//dev0->GetPhy()->EnableJRB();
	Ptr<ConstantPositionMobilityModel> sender1Mobility = CreateObject<
			ConstantPositionMobilityModel>();
	// Configure position 10 m distance
	sender1Mobility->SetPosition(Vector(0, 10, 0));
	dev1->GetPhy()->SetMobility(sender1Mobility);
	//dev1->GetPhy()->EnableJRB();
	Ptr<ConstantPositionMobilityModel> sender2Mobility = CreateObject<
			ConstantPositionMobilityModel>();
	// Configure position 10 m distance
	sender2Mobility->SetPosition(Vector(0, 5, 0));
	dev2->GetPhy()->SetMobility(sender2Mobility);
	//dev2->GetPhy()->EnableJRB();
	McpsDataConfirmCallback cb0;
	cb0 = MakeCallback(&DataConfirm);
	dev0->GetMac()->SetMcpsDataConfirmCallback(cb0);

	McpsDataIndicationCallback cb1;
	cb1 = MakeCallback(&DataIndication);
	dev0->GetMac()->SetMcpsDataIndicationCallback(cb1);

	McpsDataConfirmCallback cb2;
	cb2 = MakeCallback(&DataConfirm);
	dev1->GetMac()->SetMcpsDataConfirmCallback(cb2);

	McpsDataIndicationCallback cb3;
	cb3 = MakeCallback(&DataIndication);
	dev1->GetMac()->SetMcpsDataIndicationCallback(cb3);

	McpsDataConfirmCallback cb4;
	cb4 = MakeCallback(&DataConfirm);
	dev2->GetMac()->SetMcpsDataConfirmCallback(cb4);

	McpsDataIndicationCallback cb5;
	cb5 = MakeCallback(&DataIndication);
	dev2->GetMac()->SetMcpsDataIndicationCallback(cb5);

	//--------------Enabling JRB----------------------------------------
	if (jrb_enabled) {
		LrWpanPhy::EnableJRB();
		if (early_jrb) {
			LrWpanPhy::SetEarlyJRB(true);
		} else {
			LrWpanPhy::SetEarlyJRB(false);
		}
	} else {
		LrWpanPhy::DisableJRB();
	}

	//------------Disabling CSMA/CA-------------------------------------
	dev0->GetMac()->GetCsmaCa()->setCsmaEnabled(false);
	dev1->GetMac()->GetCsmaCa()->setCsmaEnabled(false);
	dev2->GetMac()->GetCsmaCa()->setCsmaEnabled(false);

	//Tracing
	lrWpanHelper.EnablePcapAll(std::string("lr-wpan-data"), true);
	AsciiTraceHelper ascii;
	Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream("lr-wpan-data.tr");
	lrWpanHelper.EnableAsciiAll(stream);

	// legitimate Packet creation
	Ptr<Packet> p0 = Create<Packet>(o_payload); // o_payload bytes of dummy data
	McpsDataRequestParams params;
	params.m_srcAddrMode = SHORT_ADDR;
	params.m_dstAddrMode = SHORT_ADDR;
	params.m_dstPanId = 0;
	params.m_dstAddr = Mac16Address("00:02");
	params.m_msduHandle = 0;
	params.m_txOptions = TX_OPTION_NONE;

	// Transmission of legitimate packet
	Simulator::Schedule(Seconds(0.1), &LrWpanMac::McpsDataRequest,
			dev0->GetMac(), params, p0);

	// jamming Packet creation
	Ptr<Packet> p1 = Create<Packet>(j_payload);
	params.m_dstAddr = Mac16Address("00:02");

	// Transmission of jamming packet
	Simulator::Schedule(Seconds(0.10025), &LrWpanMac::McpsDataRequest,
			dev2->GetMac(), params, p1); //250 mu s to react

	Simulator::Run();

	Simulator::Destroy();
	return 0;
}
