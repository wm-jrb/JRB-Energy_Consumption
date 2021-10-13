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
 * Author:
 *  Tobias Sesterhenn
 */

#ifndef LR_WPAN_OQPSK_H
#define LR_WPAN_OQPSK_H

#include <ns3/object.h>
#include <ns3/event-id.h>
#include <ns3/lr-wpan-header.h>
#include <ns3/lr-wpan-mac-trailer.h>
#include <ns3/packet.h>
#include <ns3/simulator.h>

namespace ns3 {

/**
 * \ingroup lr-wpan
 *
 * This class models the oqpsk modulation. For simulation purposes, we let chips be chips and only work with the packet bits (not modulated)
 */
class LrWpanOqpsk: public Object {

public:
	/**
	 * Get the type ID.
	 *
	 * \return the object TypeId
	 */
	static TypeId GetTypeId(void);

	/**
	 * Default constructor.
	 */
	LrWpanOqpsk(void);
	virtual ~LrWpanOqpsk(void);

	static std::string GetBinaryRepresentation(Ptr<Packet> p);
	static Ptr<Packet> GetPacket(std::string binaryRepr);
	static std::string GetFirstJammedComponent(int firstJammedBit);

	static std::string toBinary(uint8_t number);
	static std::string toBinary7(uint8_t number);

	static std::string toBinary(uint16_t number);

	static int toInt(std::string binary);

	static Ptr<Packet> GetPacket(int i);

	static void addPacket(Ptr<Packet> packet, Time startTime, Time stopTime);

	/**
	 *
	 * returns empty string, if no Interference was detected
	 */
	static std::string calculateInterference(void);

//private:
	static Ptr<Packet> o_packets[];
	static double o_packetStartTime[];
	static double o_packetStopTime[];
	static uint32_t o_counter;
	static std::string o_interfered_Packet_Binary;
	static int o_number_NonJammedBitsStart;
	static int o_number_JammedBits;

};
}
#endif /* LR_WPAN_OQPSK_H */
