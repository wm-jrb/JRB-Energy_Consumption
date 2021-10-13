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
 *  Author: kwong yin <kwong-sang.yin@boeing.com>
 */

#ifndef LR_WPAN_HEADER_H
#define LR_WPAN_HEADER_H

#include <ns3/header.h>
#include <ns3/mac16-address.h>
#include <ns3/mac64-address.h>
#include "ns3/lr-wpan-mac-header.h"

namespace ns3 {

/**
 * \ingroup lr-wpan
 * Represent the whole header of a packet, say the Phy Header and the Mac Header
 *
 * The class will only model the 2.4 GHz with OQBSK.
 */
class LrWpanHeader: public Header {

public:
	LrWpanHeader(void);

	LrWpanHeader(LrWpanMacHeader macHeader);

	~LrWpanHeader(void);

	void SetShrPreamble(uint32_t preamble);

	void SetShrSfd(uint8_t sfd);

	void SetPjr(uint8_t pjr);

	void SetPhr(uint8_t phr);

	void SetMacHeader(LrWpanMacHeader macHeader);

	uint32_t GetShrPreamble(void);

	uint8_t GetShrSfd(void);

	uint8_t GetPjr(void);

	uint8_t GetPhr(void);

	LrWpanMacHeader GetMacHeader(void);

	/**
	 * Calculates a new Header with the given Binary Representations of phy and mac.
	 * This method is needed when creating a new Header of a jammed packet.
	 */
	LrWpanHeader calculate_Header(std::string phyHeader_Binary,
			std::string macHeader_Binary);

	std::string GetBinaryRepresentation(void);

	uint8_t calculate_PJR(void);

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

//private:
	uint32_t lr_shrPreamble;
	uint8_t lr_shrSfd;
	uint8_t lr_phr;
	uint8_t lr_pjr;
	LrWpanMacHeader lr_macHeader;

};
//LrWpanHeader

}
;
// namespace ns-3

#endif /* LR_WPAN_HEADER_H */
