//////////////////////////////////////////////////////////////////////////
// testHepMC.cc.in
//
// garren@fnal.gov, March 2006
// Use Matt's example_EventSelection to check HepMC.
// Apply an event selection to the events in testHepMC.input
// Events containing a photon of pT > 25 GeV pass the selection and are
// written to "testHepMC.out"
// Add arbitrary PDF information to the good events
// Also write events using IO_AsciiParticles and IO_ExtendedAscii
//////////////////////////////////////////////////////////////////////////
//

#include "HepMC/IO_Ascii.h"
#include "HepMC/IO_AsciiParticles.h"
#include "HepMC/IO_ExtendedAscii.h"
#include "HepMC/GenEvent.h"

// define methods and classes used by this test
#include "IsGoodEvent.h"

int main() { 
    // declare an input strategy to read the data produced with the 
    // example_MyPythia
    HepMC::IO_Ascii ascii_in("./testHepMC.input",std::ios::in);
    // declare another IO_Ascii for writing out the good events
    HepMC::IO_Ascii ascii_out("testHepMC.out",std::ios::out);
    // declare an IO_AsciiParticle for output
    HepMC::IO_AsciiParticles particle_out("testHepMCParticle.out",std::ios::out);
    // declare an instance of the event selection predicate
    IsGoodEvent is_good_event;
    //........................................EVENT LOOP
    int icount=0;
    int num_good_events=0;
    HepMC::GenEvent* evt = ascii_in.read_next_event();
    while ( evt ) {
	icount++;
	if ( icount%50==1 ) std::cout << "Processing Event Number " << icount
				      << " its # " << evt->event_number() 
				      << std::endl;
	if ( is_good_event(evt) ) {
	    ascii_out << evt;
	    particle_out << evt;
	    ++num_good_events;
	}
	
	// clean up and get next event
	delete evt;
	ascii_in >> evt;
    }
    //........................................PRINT RESULT
    std::cout << num_good_events << " out of " << icount 
	      << " processed events passed the cuts. Finished." << std::endl;
}