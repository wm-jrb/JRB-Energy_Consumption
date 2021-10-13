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
#include "lr-wpan-phy-header.h"
#include <ns3/core-module.h>
#include <ns3/lr-wpan-oqpsk.h>
#include <ns3/lr-wpan-phy.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("LrWpanPhyHeader");

NS_OBJECT_ENSURE_REGISTERED(LrWpanPhyHeader);

LrWpanPhyHeader::LrWpanPhyHeader() {
	//SetMacHeader();
	uint8_t x = 0;
	SetShrPreamble(x);
	SetShrSfd(0b10100111);
	if (LrWpanPhy::IsJrbEnabled()) {
		SetJrb(calculate_JRB());
	} else {
		SetJrb(0);
	}
	SetJrb(x);
}

LrWpanPhyHeader::LrWpanPhyHeader(uint8_t phr) {
	SetShrPreamble(0);
	SetShrSfd(0b10100111);
	SetPhr(phr);
	if (LrWpanPhy::IsJrbEnabled()) {
		SetJrb(calculate_JRB());
	} else {
		SetJrb(0);
	}
}

LrWpanPhyHeader::~LrWpanPhyHeader() {
}

/**
 * Calculates the Physical Jamming Resistance
 * creates a number consisting of 6 random bits and adds two parity bits to the number
 */
uint8_t LrWpanPhyHeader::calculate_JRB(void) {

	RngSeedManager::SetSeed(Simulator::Now().GetFemtoSeconds() % 64 + 1); //); //64 JRB values possible

	Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
	uint8_t jrb = x->GetInteger(0, 64); //only 6 bits possible

	//jrb1 example: 101-011 : first 3 bits for first checksum, remaining 3 for second checksum
	uint8_t first = jrb >> 3;
	uint8_t second = jrb % 0b1000;

	bool parity1 = (first % 0b10) ^ ((first >> 1) % 0b10)
			^ ((first >> 2) % 0b10);
	bool parity2 = (second % 0b10) ^ ((second >> 1) % 0b10)
			^ ((second >> 2) % 0b10);

	jrb = (jrb << 1) + parity1;
	jrb = (jrb << 1) + parity2;

	return jrb;
}

void LrWpanPhyHeader::SetShrPreamble(uint32_t preamble) {
	lr_shrPreamble = preamble;
}

void LrWpanPhyHeader::SetShrSfd(uint8_t sfd) {
	lr_shrSfd = sfd;
}

void LrWpanPhyHeader::SetJrb(uint8_t jrb) {
	lr_jrb = jrb;
}

void LrWpanPhyHeader::SetPhr(uint8_t phr) {
	lr_phr = phr;
}

uint32_t LrWpanPhyHeader::GetShrPreamble(void) {
	return lr_shrPreamble;
}

uint8_t LrWpanPhyHeader::GetShrSfd(void) {
	return lr_shrSfd;
}

uint8_t LrWpanPhyHeader::GetJrb(void) {
	return lr_jrb;
}

uint8_t LrWpanPhyHeader::GetPhr(void) {
	return lr_phr;
}

LrWpanPhyHeader LrWpanPhyHeader::calculate_Header(
		std::string phyHeader_Binary) {

	LrWpanPhyHeader header;

	uint32_t preamble = LrWpanOqpsk::toInt(phyHeader_Binary.substr(0, 32));
	header.SetShrPreamble(preamble);
	uint8_t sfd = LrWpanOqpsk::toInt(phyHeader_Binary.substr(32, 8)); //reverse the SFD so that we store it least significant bit first
	header.SetShrSfd(sfd);
	uint8_t jrb;
	uint8_t phr;
	if (LrWpanPhy::IsJrbEnabled()) { //header has jrb integrated

		if (LrWpanPhy::IsEarlyJRB()) {
			jrb = LrWpanOqpsk::toInt(phyHeader_Binary.substr(40, 8));
			phr = LrWpanOqpsk::toInt(phyHeader_Binary.substr(48, 8)); //reserved bit is due to modulation before the phr
		} else {
			phr = LrWpanOqpsk::toInt(phyHeader_Binary.substr(40, 8)); //reserved bit is due to modulation before the phr
			jrb = LrWpanOqpsk::toInt(phyHeader_Binary.substr(48, 8));
		}
		header.SetJrb(jrb);
		header.SetPhr(phr);
	} else { //length is 48 (without jrb)
		phr = LrWpanOqpsk::toInt(phyHeader_Binary.substr(40, 7)); //last bit is reserved
		header.SetPhr(phr);
	}
	return header;
}

TypeId LrWpanPhyHeader::GetTypeId(void) {
	static TypeId tid =
			TypeId("ns3::LrWpanPhyHeader").SetParent<Header>().SetGroupName(
					"LrWpan").AddConstructor<LrWpanPhyHeader>();
	return tid;
}

TypeId LrWpanPhyHeader::GetInstanceTypeId(void) const {
	return GetTypeId();
}

std::string LrWpanPhyHeader::GetBinaryRepresentation(void) {
	std::string repr = "";
	//add preamble:
	repr += "00000000000000000000000000000000"; //32 zeros
	//add sfd
	repr += LrWpanOqpsk::toBinary(lr_shrSfd);
	//add jrb if enabled
	if (LrWpanPhy::IsJrbEnabled()) {
		if (LrWpanPhy::IsEarlyJRB()) {
			repr += LrWpanOqpsk::toBinary(lr_jrb);
			//add phr: phr has 7 bits and one reserved bit
			repr += "0";
			repr += LrWpanOqpsk::toBinary7(lr_phr);
		} else {
			//add phr: phr has 7 bits and one reserved bit
			repr += "0";
			repr += LrWpanOqpsk::toBinary7(lr_phr);
			repr += LrWpanOqpsk::toBinary(lr_jrb);
		}
	} else {
		repr += LrWpanOqpsk::toBinary7(lr_phr);
		repr += "0";
	}
	return repr;
}

void LrWpanPhyHeader::Print(std::ostream &os) const {

	os << "  Preamble = " << (uint32_t) lr_shrPreamble << ", SFD = "
			<< (uint32_t) lr_shrSfd;
	if (LrWpanPhy::IsJrbEnabled()) {
		if (LrWpanPhy::IsEarlyJRB()) {
			os << ", JRB = " << (uint32_t) lr_jrb;
			os << ", PHR = " << (uint32_t) lr_phr;
		} else {
			os << ", PHR = " << (uint32_t) lr_phr;
			os << ", JRB = " << (uint32_t) lr_jrb;
		}
	} else {
		os << ", PHR = " << (uint32_t) lr_phr << "\n";
	}
}

/**
 * Only for 2.4 GHz OQBSK Phy
 */
uint32_t LrWpanPhyHeader::GetSerializedSize(void) const {
	/*
	 * Each Phy header will have
	 * preamble		: 4 octets
	 * sfd			: 1 octet
	 * jrb			: 1 octet if enabled
	 * phr			: 1 octet
	 */

	uint32_t size = 6;
	if (LrWpanPhy::IsJrbEnabled()) {
		size += 1;
	}
	return (size);
}

void LrWpanPhyHeader::Serialize(Buffer::Iterator start) const {
	Buffer::Iterator i = start;
	i.WriteU32(lr_shrPreamble);
	i.WriteU8(lr_shrSfd);

	if (LrWpanPhy::IsJrbEnabled()) {
		if (LrWpanPhy::IsEarlyJRB()) {
			i.WriteU8(lr_jrb);
			i.WriteU8(lr_phr);
		} else {
			i.WriteU8(lr_phr);
			i.WriteU8(lr_jrb);
		}

	} else {
		i.WriteU8(lr_phr);
	}
}

uint32_t LrWpanPhyHeader::Deserialize(Buffer::Iterator start) {

	Buffer::Iterator i = start;
	SetShrPreamble(i.ReadU32());
	SetShrSfd(i.ReadU8());
	if (LrWpanPhy::IsJrbEnabled()) {
		if (LrWpanPhy::IsEarlyJRB()) {
			SetJrb(i.ReadU8());
			SetPhr(i.ReadU8());
		} else {
			SetPhr(i.ReadU8());
			SetJrb(i.ReadU8());
		}
	} else {
		SetPhr(i.ReadU8());
	}
	return i.GetDistanceFrom(start);
}

// ----------------------------------------------------------------------------------------------------------

}//namespace ns3

