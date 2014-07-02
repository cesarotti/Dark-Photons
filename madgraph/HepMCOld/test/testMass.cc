//////////////////////////////////////////////////////////////////////////
// testMass.cc.in
//
// garren@fnal.gov, March 2006
// Use Matt's example_EventSelection to check HepMC.
// Apply an event selection to the events in testMass.input
// Events containing a photon of pT > 25 GeV pass the selection and are
// written to "testMass.dat"
// Add arbitrary PDF information to the good events
// Also write events using IO_AsciiParticles and IO_ExtendedAscii
//////////////////////////////////////////////////////////////////////////
//

#include "HepMC/IO_ExtendedAscii.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Version.h"

// define methods and classes used by this test
#include "IsGoodEvent.h"

void massInfo( const HepMC::GenEvent* );

int main() { 
    // read and process the input file
    {
	// declare an input strategy to read the data produced with the 
	// example_MyPythia
	HepMC::IO_ExtendedAscii ascii_in("./testHepMCExtended.input",std::ios::in);
	// declare another IO_ExtendedAscii for output
	HepMC::IO_ExtendedAscii ascii_out("testHepMCExtended.dat",std::ios::out);
	// declare an instance of the event selection predicate
	IsGoodEvent is_good_event;
    // send version to standard output
    HepMC::version();
	//........................................EVENT LOOP
	int icount=0;
	int num_good_events=0;
        double x=0., y=0., z=0.;
	HepMC::GenEvent* evt = ascii_in.read_next_event();
	while ( evt ) {
	    icount++;
	    if ( icount%50==1 ) std::cout << "Processing Event Number " << icount
					  << " its # " << evt->event_number() 
					  << std::endl;
	    if ( is_good_event(evt) ) {
		if (num_good_events == 0 ) {
		    // add some arbitrary PDF information
		    x = 0.1 * icount;
		    y = 0.13 * icount;
		    z = 0.012 * icount;
		    HepMC::PdfInfo pdf( 11, 12, x, y, z, 0.11, 0.34);
		    evt->set_pdf_info(pdf);
		}
		ascii_out << evt;
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
    // now read the file we just created
    {
	// declare an input strategy 
	HepMC::IO_ExtendedAscii xin("testHepMCExtended.dat",std::ios::in);
	// declare another IO_ExtendedAscii for output
	HepMC::IO_ExtendedAscii xout("testMass.dat",std::ios::out);
	//........................................EVENT LOOP
	int ixin=0;
	HepMC::GenEvent* evt = xin.read_next_event();
	while ( evt ) {
	    ixin++;
	    xout << evt;
	    // look at mass info
	    massInfo(evt);

	    // clean up and get next event
	    delete evt;
	    xin >> evt;
	}
	//........................................PRINT RESULT
	std::cout << ixin 
		  << " events in the second pass. Finished." << std::endl;
    }
}

void massInfo( const HepMC::GenEvent* e )
{
   double gm, m, d;
   for ( HepMC::GenEvent::particle_const_iterator p = e->particles_begin(); p != e->particles_end();
         ++p ) {
	 
       gm = (*p)->generated_mass();
       m = (*p)->momentum().m();
       d = fabs(m-gm);
       if( d > 1.0e-5 ) {
	    std::cout << "Event " << e->event_number()
	              << " Particle " << (*p)->barcode() 
	              << " " << (*p)->pdg_id()
        	      << " generated mass " << gm
        	      << " mass from momentum " << m 
		      << " difference " << d << std::endl;
       } 
   }   
}