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
 */

#include "ns3/lr-wpan-oqpsk.h"
#include <ns3/log.h>
#include <ns3/lr-wpan-phy-header.h>
#include <ns3/lr-wpan-mac-header.h>
#include <ns3/lr-wpan-mac-trailer.h>
#include <ns3/lr-wpan-phy.h>
#include <ns3/packet.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("LrWpanOqpsk");

NS_OBJECT_ENSURE_REGISTERED(LrWpanOqpsk);

Ptr<Packet> LrWpanOqpsk::o_packets[2]; //first one is legitimate, second one is Jammer
double LrWpanOqpsk::o_packetStartTime[2];
double LrWpanOqpsk::o_packetStopTime[2];
uint32_t LrWpanOqpsk::o_counter;
std::string LrWpanOqpsk::o_interfered_Packet_Binary;
int LrWpanOqpsk::o_number_NonJammedBitsStart = 0;
int LrWpanOqpsk::o_number_JammedBits = 0;

TypeId LrWpanOqpsk::GetTypeId(void) {
	static TypeId tid =
			TypeId("ns3::LrWpanOqpsk").SetParent<Object>().SetGroupName(
					"LrWpan").AddConstructor<LrWpanOqpsk>();
	return tid;
}

LrWpanOqpsk::LrWpanOqpsk() {

}

LrWpanOqpsk::~LrWpanOqpsk() {

}

/**
 * Adds a packet, which is transmitted at a given time and for a given duration.
 */
void LrWpanOqpsk::addPacket(Ptr<Packet> packet, Time startTime, Time duration) {
	o_counter = o_counter < 2 ? o_counter : 0;
	if (o_counter == 0) {
		o_interfered_Packet_Binary = "";
	}

	o_packets[o_counter] = packet;
	o_packetStartTime[o_counter] = startTime.GetSeconds();

	o_packetStopTime[o_counter] = startTime.GetSeconds()
			+ duration.GetSeconds();

	NS_LOG_DEBUG("START TIME: "<< o_packetStartTime[o_counter]);
	NS_LOG_DEBUG("STOP TIME: "<< o_packetStopTime[o_counter]);
	NS_LOG_DEBUG("DURATION: "<< duration);
	o_counter++;
}

Ptr<Packet> LrWpanOqpsk::GetPacket(int i) {
	return o_packets[i];
}

std::string LrWpanOqpsk::calculateInterference() {
	if (o_counter < 2) {
		NS_LOG_UNCOND("Interference can not be calculated without 2 packets.");
		return "";
	}

	Ptr<Packet> p1_packet = o_packets[0];
	Ptr<Packet> p2_packet = o_packets[1];
	std::string p1_binary = GetBinaryRepresentation(p1_packet);
	std::string p2_binary = GetBinaryRepresentation(p2_packet);
	double p1_start = o_packetStartTime[0];
	double p2_start = o_packetStartTime[1];
	double p1_stop = o_packetStopTime[0];
	double p2_stop = o_packetStopTime[1];

	double p1_transmitDuration = p1_stop - p1_start;
	double p2_transmitDuration = p2_stop - p2_start;
	double p1_TimePerBit = p1_transmitDuration / (p1_packet->GetSize() * 8); //size is in bytes
	NS_LOG_DEBUG("TIME PER BIT: "<<p1_TimePerBit);
	//p1_TimePerBit = 0.000004;
	double deltaStart_p1p2 = p2_start - p1_start;
	int number_nonJammed_Bits_Start = deltaStart_p1p2 / p1_TimePerBit;

	if (p1_stop < p2_start) {
		NS_LOG_DEBUG("First packet was not interfered");
		o_counter = 1; //first packet was not interfered, but second one may be interfered from the next packet
		o_interfered_Packet_Binary = p1_binary; //no interference
		o_number_NonJammedBitsStart = p1_binary.length();

		//make second packet to new first packet
		o_packets[0] = o_packets[1];
		o_packetStartTime[0] = o_packetStartTime[1];
		o_packetStopTime[0] = o_packetStopTime[1];

		return "";
	}

	int number_Jammed_Bits;
	if (p2_stop >= p1_stop) {
		number_Jammed_Bits = p1_binary.length() - number_nonJammed_Bits_Start;
	} else {
		number_Jammed_Bits = (int) (p2_transmitDuration / p1_TimePerBit + 0.5);
	}

	//switch the bits from the first jammed component
	int index = number_nonJammed_Bits_Start;
	int i = number_Jammed_Bits;
	while (i > 0 && index < p1_binary.length()) {
		if (p1_binary.at(index) == ('1')) {
			p1_binary.replace(index, 1, "0");
		} else {
			p1_binary.replace(index, 1, "1");
		}
		index++;
		i--;
	}

	o_counter = 0; //after interference we don't need both packets anymore
	o_interfered_Packet_Binary = p1_binary;
	o_number_NonJammedBitsStart = number_nonJammed_Bits_Start;
	o_number_JammedBits = number_Jammed_Bits;

	NS_LOG_DEBUG("\nTimePerBit: " <<p1_TimePerBit);
	NS_LOG_DEBUG("\nNonJammedBitsStart: " <<number_nonJammed_Bits_Start);
	NS_LOG_DEBUG("\nNumberJammedBits: " <<number_Jammed_Bits);

	return o_interfered_Packet_Binary;

}

//-----------------------------------------------------------------------------------------------------------------------

std::string LrWpanOqpsk::GetBinaryRepresentation(Ptr<Packet> p) {
	LrWpanPhyHeader phyHeader;
	p->RemoveHeader(phyHeader);
	LrWpanMacHeader macHeader;
	p->PeekHeader(macHeader);

	p->AddHeader(phyHeader);

	LrWpanMacTrailer trailer;
	p->PeekTrailer(trailer);

	uint32_t payload_size = p->GetSize() - phyHeader.GetSerializedSize() - 11 //this is the MAC header size in our simulation scenario
			- trailer.GetSerializedSize();

	std::string repr = "";
	repr += phyHeader.GetBinaryRepresentation();
	repr += macHeader.GetBinaryRepresentation();
	for (int i = 0; i < payload_size; i++) {
		repr += "00000000";
	}
	repr += toBinary(trailer.GetFcs());

	NS_LOG_DEBUG("PACKET: " << p->ToString());
	NS_LOG_DEBUG("BINARY LENGTH: " << repr.length());
	NS_LOG_DEBUG("Packet LENGTH: " << p->GetSize());
	NS_LOG_DEBUG("PAYLOAD LENGTH: "<<payload_size);
	NS_LOG_DEBUG(
			"PHY_HEADER LENGTH: "<<phyHeader.GetBinaryRepresentation().length()/8);
	NS_LOG_DEBUG(
			"MAC_HEADER LENGTH: "<< macHeader.GetBinaryRepresentation().length()/8);

	NS_LOG_DEBUG("TRAILER LENGTH: "<<trailer.GetSerializedSize());

	return repr;
}

Ptr<Packet> LrWpanOqpsk::GetPacket(std::string binaryRepr) {

	NS_LOG_DEBUG("GetPacket reversed:\n" << binaryRepr);

	int binaryLength = binaryRepr.length();
	//last 16 Bits are FCS
	std::string fcs_Binary = binaryRepr.substr(binaryLength - 16, 16);

	std::string phyHeader_Binary;
	std::string macHeader_Binary;
	if (LrWpanPhy::IsJrbEnabled()) {
		phyHeader_Binary = binaryRepr.substr(0, 56); //phy Header has 7 Bytes (4 preamble, 1 SFD, 1 JRB, 1 PHR)
		macHeader_Binary = binaryRepr.substr(56, 88); //mac Header has 11 Bytes
	} else {
		phyHeader_Binary = binaryRepr.substr(0, 48); //phy Header has 6 Bytes (4 preamble, 1 SFD, 0 JRB, 1 PHR)
		macHeader_Binary = binaryRepr.substr(48, 88); //mac Header has 11 Bytes
	}
	int payloadLength = binaryLength - fcs_Binary.length()
			- phyHeader_Binary.length() - macHeader_Binary.length();

	int fcs = toInt(fcs_Binary);
	LrWpanMacTrailer trailer;
	trailer.EnableFcs(true);
	trailer.SetFcs((uint16_t) fcs);

	Ptr<Packet> packet = Create<Packet>(payloadLength / 8);
	packet->AddTrailer(trailer);

	LrWpanPhyHeader phyHeader;
	phyHeader = phyHeader.calculate_Header(phyHeader_Binary);

	LrWpanMacHeader macHeader;
	macHeader = macHeader.calculate_MacHeader(macHeader_Binary);

	packet->AddHeader(macHeader);
	packet->AddHeader(phyHeader);

	return packet;
}

std::string LrWpanOqpsk::GetFirstJammedComponent(int firstJammedBit) {
	//handle PHY
	if (LrWpanPhy::IsJrbEnabled()) {
		if (firstJammedBit < 32) {
			return "Preamble";
		}
		if (firstJammedBit < 40) {
			return "SFD";
		}
		if (firstJammedBit < 48) {
			if (LrWpanPhy::IsEarlyJRB()) {
				return "JRB";
			} else {
				return "PHR";
			}
		}
		if (firstJammedBit < 56) {
			if (LrWpanPhy::IsEarlyJRB()) {
				return "PHR";
			} else {
				return "JRB";
			}
		}
		firstJammedBit -= 8; //MAC Header can now be handled, no matter if JRB enabled or not

	} else {
		if (firstJammedBit < 32) {
			return "Preamble";
		}
		if (firstJammedBit < 40) {
			return "SFD";
		}
		if (firstJammedBit < 48) {
			return "PHR";
		}
	}
	firstJammedBit -= 48;
	//handle MAC
	if (firstJammedBit < 1) {
		return "Frame Control (first Reserved bit)";
	}
	if (firstJammedBit < 2) {
		return "Frame Control (PAN ID Compression)";
	}
	if (firstJammedBit < 3) {
		return "Frame Control (Ack. Request)";
	}
	if (firstJammedBit < 4) {
		return "Frame Control (Frame Pending)";
	}
	if (firstJammedBit < 5) {
		return "Frame Control (Security Enabled)";
	}
	if (firstJammedBit < 8) {
		return "Frame Control (Frame Type)";
	}
	if (firstJammedBit < 10) {
		return "Frame Control (Source Addressing Mode)";
	}
	if (firstJammedBit < 12) {
		return "Frame Control (Frame Version)";
	}
	if (firstJammedBit < 14) {
		return "Frame Control (Destination Addressing Mode)";
	}
	if(firstJammedBit < 16){
		return "Frame Control (last Reserved bits)";
	}

	firstJammedBit -= 15;
	if (firstJammedBit < 8) {
		return "Sequence Number";
	}
	if (firstJammedBit < 24) {
		return "Destination PAN Identifier";
	}
	if (firstJammedBit < 40) {
		return "Destination Address";
	}
	if (firstJammedBit < 56) {
		return "Source PAN Identifier";
	}
	if (firstJammedBit < 72) {
		return "Source Address";
	}

	return "Payload or FCS";
}

std::string LrWpanOqpsk::toBinary(uint8_t number) {
	std::string res = "";
	while (number > 0) {
		res = std::to_string(number % 2) + res;
		number /= 2;
	}
	while (res.length() < 8) {
		res = "0" + res;
	}
	return res;
}

std::string LrWpanOqpsk::toBinary7(uint8_t number) {
	std::string res = "";
	while (number > 0) {
		res = std::to_string(number % 2) + res;
		number /= 2;
	}
	while (res.length() < 7) {
		res = "0" + res;
	}
	return res;
}

std::string LrWpanOqpsk::toBinary(uint16_t number) {
	std::string res = "";
	while (number > 0) {
		res = std::to_string(number % 2) + res;
		number /= 2;
	}
	while (res.length() < 16) {
		res = "0" + res;
	}
	return res;
}

int LrWpanOqpsk::toInt(std::string binary) {
	return std::stoi(binary, nullptr, 2);
}

std::string LrWpanOqpsk::reverseString(std::string toReverse) {
	std::string reversed = "";
	for (int i = 0; i < toReverse.length(); i++) {
		reversed = toReverse.at(i) + reversed;
	}
	return reversed;
}

} //namespace ns3
