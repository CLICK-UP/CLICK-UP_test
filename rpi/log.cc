#include <click/config.h>
#include <click/error.hh>
#include <click/args.hh>
#include "log.hh"
CLICK_DECLS



Log::Log() {
	
}

Log::~Log() {
	
}

int Log::configure(Vector<String>& conf, ErrorHandler* errh)
{
	if (Args(conf, this, errh)

.complete() < 0)
	return -1;
	return 0;
}



void Log::push(int i, Packet* p_in)
{
	int n;
	WritablePacket *p = p_in->uniqueify();

	click_chatter("packet on port %i of length %u B",  i, p->length());
	output(n).push(p);
	
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Log)



