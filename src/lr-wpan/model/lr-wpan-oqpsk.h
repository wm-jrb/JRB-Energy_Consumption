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

#ifndef LR_WPAN_OQPSK_H
#define LR_WPAN_OQPSK_H

#include <ns3/object.h>
#include <ns3/event-id.h>
#include <ns3/lr-wpan-phy-header.h>
#include <ns3/lr-wpan-mac-trailer.h>
#include <ns3/packet.h>
#include <ns3/simulator.h>

namespace ns3 {

/**
 * \ingroup lr-wpan
 *
 * This class models the O-QPSK modulation. For simulation purposes, we let chips be chips and only work with the packet bits (not modulated).
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

	/**
	 * Returns the binary representation of a packet as it is send over the channel (most-significant-bit-first order)
	 */
	static std::string GetBinaryRepresentation(Ptr<Packet> p);

	/**
	 * Creates a Packet from the Binary Representation.
	 */
	static Ptr<Packet> GetPacket(std::string binaryRepr);

	/**
	 * Returns the first field that was jammed.
	 * Returns empty string if no field was corrupted.
	 */
	static std::string GetFirstJammedComponent(int firstJammedBit);

	/**
	 * Method to create a binary representation of a 8-bit integer.
	 */
	static std::string toBinary(uint8_t number);

	/**
	 * Method to create a 7-bit binary representation of a 8-bit integer.
	 */
	static std::string toBinary7(uint8_t number);

	/**
	 *Method to create a binary representation of a 16-bit integer.
	 */
	static std::string toBinary(uint16_t number);

	/**
	 * Builds a string backwards:
	 * Example: hello -> olleh
	 * This method is needed to build the correct representation of a transmitting packet.
	 * It follows the most-significant-bit first order.
	 * Therefore each byte in the binary representation has to be reversed.
	 */
	static std::string reverseString(std::string toReverse);

	/**
	 * Transforms a binary string to an int.
	 */
	static int toInt(std::string binary);

	/**
	 * Returns the packet stored at position i in o_packets.
	 */
	static Ptr<Packet> GetPacket(int i);

	/**
	 * Adds a packet to o_packets, o_packetStartTime and o_packetStopTime.
	 */
	static void addPacket(Ptr<Packet> packet, Time startTime, Time stopTime);

	/**
	 *
	 * returns empty string, if no Interference was detected
	 */
	static std::string calculateInterference(void);

	/**
	 * Stores at most two packets.
	 */
	static Ptr<Packet> o_packets[];

	/**
	 * Stores Start Time of the packets.
	 */
	static double o_packetStartTime[];

	/**
	 * Stores Stop Time of the packets.
	 */
	static double o_packetStopTime[];

	/**
	 * Counts the stored packets.
	 */
	static uint32_t o_counter;

	/**
	 * Contains the last interfered binary representation.
	 */
	static std::string o_interfered_Packet_Binary;

	/**
	 * Contains the non jammed bits number of the last interfered packet.
	 */
	static int o_number_NonJammedBitsStart;

	/**
	 * Contains the jammed bits number of the last interfered packet.
	 */
	static int o_number_JammedBits;

};
}
#endif /* LR_WPAN_OQPSK_H */
