/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author:  Vishwesh Rege <vrege2012@gmail.com>
 */
#include <ns3/test.h>
#include <ns3/log.h>

#include <ns3/lr-wpan-module.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/simulator.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/packet.h>


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("lr-wpan-nullmac-test");

class LrWpanNullMacTestCase : public TestCase
{
public:
  LrWpanNullMacTestCase ();
  virtual ~LrWpanNullMacTestCase ();

private:
  virtual void DoRun (void);
};

LrWpanNullMacTestCase::LrWpanNullMacTestCase ()
  : TestCase ("Test the 802.15.4 MAC")
{
}

LrWpanNullMacTestCase::~LrWpanNullMacTestCase ()
{
}

void
LrWpanNullMacTestCase::DoRun (void)
{

}

// ==============================================================================
class LrWpanNullMacTestSuite : public TestSuite
{
public:
  LrWpanNullMacTestSuite ();
};

LrWpanNullMacTestSuite::LrWpanNullMacTestSuite ()
  : TestSuite ("lr-wpan-nullmac", UNIT)
{
  AddTestCase (new LrWpanNullMacTestCase, TestCase::QUICK);
}

static LrWpanNullMacTestSuite lrWpanNullMacTestSuite;
