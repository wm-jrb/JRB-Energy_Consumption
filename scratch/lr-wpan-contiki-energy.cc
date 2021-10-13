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
 * We simulate a simple scenario where a node sends a packet to another node, each second for a time of 1000000 seconds.
 * We trace the energy consumption and the time spent in the specific transceiver states.
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
#include "ns3/lr-wpan-radio-energy-model.h"
#include "ns3/basic-energy-source.h"

#include <iostream>

using namespace ns3;

int count = 10000;
/**
 * Called, when the energy of an energy source changes.
 * It is called very often, therefore it prints out just each 10000th call.
 * 
 */
static void GetTotalEnergyConsumption(std::string context, double oldValue,
		double newValue) {
	if (count == 10000) { //logs the energy levels each 10000 calls
		count = 1;
		std::cout << context << "    " << Simulator::Now()
		//<<" TotalEnergyConsumption: "
				<< "    " << newValue
				//<< " from "
				<< "    " << oldValue << "\n";
	}
	count++;
	//std::cout << "Count" << count << "\n";
}

int main(int argc, char *argv[]) {
	bool verbose = false;
	bool jrbEnabled = false; //to enable the JRB
	int payload = 0; //to determine the payload size (in bytes) of the packets
	bool e_logger = false;
	ns3::PacketMetadata::Enable();

	CommandLine cmd;

	cmd.AddValue("verbose", "turn on all log components", verbose);
	cmd.AddValue("jrb", "enable the jrb of the phys", jrbEnabled);
	cmd.AddValue("payload",
			"determine the payload size (in bytes) of the packets", payload);
	cmd.AddValue("e_logging", "log energy changes", e_logger);

	cmd.Parse(argc, argv);

	//Check if payload size is valid:
	if (!(payload >= 0 && payload <= LrWpanPhy::aMaxPhyPacketSize - 13)) { //MAC Header and footer are 13 bytes
		NS_LOG_UNCOND(
				"Error: Payload size has to be between 0 and " << LrWpanPhy::aMaxPhyPacketSize -13 << ". Please try again with valid values.");
		return 0;
	}

	//Enable Logging
	if (verbose) {
		LogComponentEnable("LrWpanCsmaCa", LOG_LEVEL_DEBUG);
		LogComponentEnable("LrWpanContikiMac", LOG_LEVEL_DEBUG);
		LogComponentEnable("LrWpanPhy", LOG_LEVEL_DEBUG);
	}
	// Create 2 nodes, and a NetDevice for each one
	Ptr<Node> n0 = CreateObject<Node>();
	Ptr<Node> n1 = CreateObject<Node>();

	Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice>();
	Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice>();

	Ptr<LrWpanContikiMac> mac0 = CreateObject<LrWpanContikiMac>();  //Change Mac
	Ptr<LrWpanContikiMac> mac1 = CreateObject<LrWpanContikiMac>();  //Change Mac

	dev0->SetMac(mac0);
	dev1->SetMac(mac1);

	dev0->SetAddress(Mac16Address("00:01"));
	dev1->SetAddress(Mac16Address("00:02"));

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

	// To complete configuration, a LrWpanNetDevice must be added to a node
	n0->AddDevice(dev0);
	n1->AddDevice(dev1);

	Ptr<LrWpanRadioEnergyModel> em0 = CreateObject<LrWpanRadioEnergyModel>();
	Ptr<LrWpanRadioEnergyModel> em1 = CreateObject<LrWpanRadioEnergyModel>();

	Ptr<BasicEnergySource> es0 = CreateObject<BasicEnergySource>();
	es0->SetSupplyVoltage(1.2); //1.2 Volt
	es0->SetInitialEnergy(11232.0); //11232 Joules energy
	Ptr<BasicEnergySource> es1 = CreateObject<BasicEnergySource>();
	es1->SetSupplyVoltage(1.2);
	es1->SetInitialEnergy(11232.0);

	es0->SetNode(n0);
	es0->AppendDeviceEnergyModel(em0);
	em0->SetEnergySource(es0);
	em0->AttachPhy(dev0->GetPhy());

	es1->SetNode(n1);
	es1->AppendDeviceEnergyModel(em1);
	em1->SetEnergySource(es1);
	em1->AttachPhy(dev1->GetPhy());

	//++++++++Enable/Disable JRB, Energy Logging++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (jrbEnabled) {
		//dev0->GetPhy()->EnableJRB();
		//dev1->GetPhy()->EnableJRB();
		LrWpanPhy::EnableJRB();

	} else {
		LrWpanPhy::DisableJRB();
	}

	if (e_logger) {
		es0->TraceConnect("RemainingEnergy", std::string("phy0"),
				MakeCallback(&GetTotalEnergyConsumption));
		es1->TraceConnect("RemainingEnergy", std::string("phy1"),
				MakeCallback(&GetTotalEnergyConsumption));
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	Ptr<ConstantPositionMobilityModel> sender0Mobility = CreateObject<
			ConstantPositionMobilityModel>();
	sender0Mobility->SetPosition(Vector(0, 0, 0));
	dev0->GetPhy()->SetMobility(sender0Mobility);
	Ptr<ConstantPositionMobilityModel> sender1Mobility = CreateObject<
			ConstantPositionMobilityModel>();
	// Configure position 10 m distance
	sender1Mobility->SetPosition(Vector(0, 10, 0)); //the distance doesn't matter in this simulation as long as the devices are in reach
	dev1->GetPhy()->SetMobility(sender1Mobility);
	Ptr<ConstantPositionMobilityModel> sender2Mobility = CreateObject<
			ConstantPositionMobilityModel>();

	McpsDataRequestParams params;
	params.m_srcAddrMode = SHORT_ADDR;
	params.m_dstAddrMode = SHORT_ADDR;
	params.m_dstPanId = 0;
	params.m_dstAddr = Mac16Address("00:01"); //address of receiving node
	params.m_msduHandle = 0;
	params.m_txOptions = TX_OPTION_NONE;	//no Acknowledgment requested

	//transmit one packet each second for one million seconds:
	for (int i = 0; i < 100; i++) {
		Ptr<Packet> p = Create<Packet>(payload); // payload can be set via parameter
		Simulator::Schedule(Seconds(i), &LrWpanMac::McpsDataRequest,
				dev1->GetMac(), params, p);
	}

	Simulator::Stop(Seconds(100));
	Simulator::Run();

	//Statistics for the analysis:
	NS_LOG_UNCOND("TX_ON Time Receiver "<< em0->trans_duration);
	NS_LOG_UNCOND("RX_ON Time Receiver "<< em0->recv_duration);
	NS_LOG_UNCOND("TRX_OFF Time Receiver "<< em0->sleep_duration);
	NS_LOG_UNCOND("Remaining Energy Receiver " << es0->GetRemainingEnergy());
	NS_LOG_UNCOND("Consumed Energy Receiver " << es0-> GetConsumedEnergy());

	NS_LOG_UNCOND("TX_ON Time Sender "<< em1->trans_duration);
	NS_LOG_UNCOND("RX_ON Time Sender "<< em1->recv_duration);
	NS_LOG_UNCOND("TRX_OFF Time Sender "<< em1->sleep_duration);
	NS_LOG_UNCOND("Remaining Energy Sender " << es1->GetRemainingEnergy());
	NS_LOG_UNCOND("Consumed Energy Sender " << es1-> GetConsumedEnergy());


	Simulator::Destroy();
	return 0;
}
