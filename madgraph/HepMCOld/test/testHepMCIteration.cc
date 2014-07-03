//////////////////////////////////////////////////////////////////////////
// testHepMCIteration.cc.in
//
// garren@fnal.gov, May 2007
// Use Matt's example_EventSelection along with example_UsingIterators 
// to check HepMC iteration.
// Apply an event selection to the events in testHepMC.input
// Events containing a photon of pT > 25 GeV pass the selection.
// Use iterators on these events.
//////////////////////////////////////////////////////////////////////////

#include <list>

#include "HepMC/IO_Ascii.h"
#include "HepMC/IO_AsciiParticles.h"
#include "HepMC/IO_ExtendedAscii.h"
#include "HepMC/GenEvent.h"

// define methods and classes used by this test
#include "IsGoodEvent.h"
#include "testHepMCIteration.h"

bool findW( HepMC::GenEvent* evt, std::ofstream& os);
bool simpleIter( HepMC::GenEvent* evt, std::ofstream& os );

int main() { 
    // declare an input strategy to read the data produced with the 
    // example_MyPythia
    HepMC::IO_Ascii ascii_in("./testHepMC.input",std::ios::in);
    // declare an instance of the event selection predicate
    IsGoodEvent is_good_event;
    // define an output stream
    std::ofstream os( "testHepMCIteration.dat" );
    //........................................EVENT LOOP
    int icount=0;
    int num_good_events=0;
    HepMC::GenEvent* evt = ascii_in.read_next_event();
    HepMC::GenEvent* evcopy;
    while ( evt ) {
	icount++;
	if ( icount%50==1 ) std::cout << "Processing Event Number " << icount
				      << " its # " << evt->event_number() 
				      << std::endl;
	// icount of 100 should be the last event
	if ( icount==100 ) std::cout << "Processing Event Number " << icount
				      << " its # " << evt->event_number() 
				      << std::endl;
        evcopy = evt;
	if ( is_good_event(evcopy) ) {
	    os << "Event " << evcopy->event_number() << " is good " << std::endl;
	    ++num_good_events;
	    // test iterators
	    simpleIter( evcopy, os );
            findW( evcopy, os );
	}
	evcopy->clear();
	
	// clean up and get next event
	delete evt;
	evt = ascii_in.read_next_event();
    }
    //........................................PRINT RESULT
    std::cout << num_good_events << " out of " << icount 
	      << " processed events passed the cuts. Finished." << std::endl;
}

bool simpleIter( HepMC::GenEvent* evt, std::ofstream& os )
{
	// use GenEvent::vertex_iterator to fill a list of all 
	// vertices in the event
	std::list<HepMC::GenVertex*> allvertices;
	for ( HepMC::GenEvent::vertex_iterator v = evt->vertices_begin();
	      v != evt->vertices_end(); ++v ) {
	    allvertices.push_back(*v);
	}

	// we could do the same thing with the STL algorithm copy
	std::list<HepMC::GenVertex*> allvertices2;
	copy( evt->vertices_begin(), evt->vertices_end(), 
	      back_inserter(allvertices2) );

	// fill a list of all final state particles in the event, by requiring
	// that each particle satisfyies the IsFinalState predicate
	IsFinalState isfinal;
	std::list<HepMC::GenParticle*> finalstateparticles;
	for ( HepMC::GenEvent::particle_iterator p = evt->particles_begin();
	      p != evt->particles_end(); ++p ) {
	    if ( isfinal(*p) ) finalstateparticles.push_back(*p);
	}

	// an STL-like algorithm called HepMC::copy_if is provided in the
	// GenEvent.h header to do this sort of operation more easily,
	// you could get the identical results as above by using:
	std::list<HepMC::GenParticle*> finalstateparticles2;
	HepMC::copy_if( evt->particles_begin(), evt->particles_end(), 
			back_inserter(finalstateparticles2), IsFinalState() );

	// print all photons in the event that satisfy the IsPhoton criteria
	os << "photons in event " << evt->event_number() << ":" << std::endl;
	for ( HepMC::GenEvent::particle_iterator p = evt->particles_begin();
	      p != evt->particles_end(); ++p ) {
	    if ( IsPhoton(*p) ) (*p)->print( os );
	}
	return true;
}

bool findW( HepMC::GenEvent* evt, std::ofstream& os )
{
    int num_W=0;
	// use GenEvent::particle_iterator to find all W's in the event,
	// then 
	// (1) for each W user the GenVertex::particle_iterator with a range of
	//     parents to return and print the immediate mothers of these W's.
	// (2) for each W user the GenVertex::particle_iterator with a range of
	//     descendants to return and print all descendants of these W's.
	for ( HepMC::GenEvent::particle_iterator p = evt->particles_begin();
	      p != evt->particles_end(); ++p ) {
	    if ( IsWBoson(*p) ) {
	        ++num_W;
		os << "A W boson has been found in event: " << evt->event_number() << std::endl;
		(*p)->print( os );
		// return all parents
		// we do this by pointing to the production vertex of the W 
		// particle and asking for all particle parents of that vertex
		os << "\t Its parents are: " << std::endl;
		if ( (*p)->production_vertex() ) {
		    for ( HepMC::GenVertex::particle_iterator mother 
			      = (*p)->production_vertex()->
			      particles_begin(HepMC::parents);
			  mother != (*p)->production_vertex()->
			      particles_end(HepMC::parents); 
			  ++mother ) {
			os << "\t";
			(*mother)->print( os );
		    }
		}
		// return all descendants
		// we do this by pointing to the end vertex of the W 
		// particle and asking for all particle descendants of that vertex
		os << "\t\t Its descendants are: " << std::endl;
		if ( (*p)->end_vertex() ) {
		    for ( HepMC::GenVertex::particle_iterator des 
			      =(*p)->end_vertex()->
			      particles_begin(HepMC::descendants);
			  des != (*p)->end_vertex()->
			      particles_end(HepMC::descendants);
			  ++des ) {
			os << "\t\t";
			(*des)->print( os );
		    }
		}
	    }
	}
	return true;
}
