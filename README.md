This repository provides the code for the simulation of the energy consumption.

The Installation_Guide file provides the necessary steps to get the ns3 started.

The `scratch` file contains different scripts for several investigation purposes.
You can focus on the `lr-wpan-contiki-energy.cc` script, as it performs the energy simulation mentioned in our paper.

The possible command line arguments for the simulation are as follows:

|Argument |   Description              |   Value   |  Default  |
|---------|----------------------------|-----------|-----------|
|   jrb   | enable the JRB            | true/false |   false   |
| payload | payload size in bytes     |   0-114    |     0     | 
|e_logging| log the energy changes    | true/false |   false   |
| verbose | turn on all log components| true/false |   false   |


The e_logging argument causes the corresponding energy consumption to be displayed for
each change of state of the radio transceiver. We present a possible program call, where the
JRB is intended to be enabled and the packets have a payload size of 42 bytes:
`python2.7 ./waf --run "scratch/lr-wpan-contiki-energy --jrb=true --payload=42"`
