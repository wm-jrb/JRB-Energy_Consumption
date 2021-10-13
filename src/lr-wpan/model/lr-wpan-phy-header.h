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
 */

#ifndef LR_WPAN_PHY_HEADER_H
#define LR_WPAN_PHY_HEADER_H

#include <ns3/header.h>

namespace ns3 {

/**
 * \ingroup lr-wpan
 * Represents the Phy Header of a packet.
 *
 * The class will only model the 2.4 GHz with OQBSK PHY header.
 */
class LrWpanPhyHeader: public Header {

public:
	LrWpanPhyHeader(void);

	LrWpanPhyHeader(uint8_t phr);

	~LrWpanPhyHeader(void);

	/**
	 * Set the Preamble.
	 */
	void SetShrPreamble(uint32_t preamble);

	/**
	 * Set the Start of Frame Delimiter.
	 */
	void SetShrSfd(uint8_t sfd);

	/**
	 * Set the Physical Jamming Resilience.
	 */
	void SetJrb(uint8_t jrb);

	/**
	 * Set the PHY header (PHR).
	 */
	void SetPhr(uint8_t phr);

	/**
	 * Get the Pramble.
	 */
	uint32_t GetShrPreamble(void);

	/**
	 * Get the SFD.
	 */
	uint8_t GetShrSfd(void);

	/**
	 * Get the JRB.
	 */
	uint8_t GetJrb(void);

	/**
	 * Get the PHR.
	 */
	uint8_t GetPhr(void);

	/**
	 * Calculates a new Header with the given Binary Representations of phy and mac.
	 * This method is needed when creating a new Header of a jammed packet.
	 */
	LrWpanPhyHeader calculate_Header(std::string phyHeader_Binary);

	/**
	 * Returns a realistic binary representation as the frame is transmitted in the channel.
	 * Uses a most-significant-bit and -byte representation.
	 */
	std::string GetBinaryRepresentation(void);

	/**
	 * Creates a random value for the information bits of the JRB and then creates the JRB out of it.
	 */
	uint8_t calculate_JRB(void);

	/**
	 * \brief Get the type ID.
	 * \return the object TypeId
	 */
	static TypeId GetTypeId(void);
	virtual TypeId GetInstanceTypeId(void) const;

	void Print(std::ostream &os) const;
	uint32_t GetSerializedSize(void) const;
	void Serialize(Buffer::Iterator start) const;
	uint32_t Deserialize(Buffer::Iterator start);

	/**
	 * The PHY header fields:
	 */
	uint32_t lr_shrPreamble;
	uint8_t lr_shrSfd;
	uint8_t lr_phr;
	uint8_t lr_jrb;
};
//LrWpanPhyHeader

}
;
// namespace ns-3

#endif /* LR_WPAN_PHY_HEADER_H */
