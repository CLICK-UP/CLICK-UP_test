#include <click/config.h>
#include <click/error.hh>
#include <click/args.hh>

#include <cstring>

#include "empty14.hh"

CLICK_DECLS

Empty14::Empty14()
	: _prefix() {}

Empty14::~Empty14() {}

int Empty14::initialize(ErrorHandler *errh)
{
	errh->message("Successfully initialized.");
	return 0;
}

int Empty14::configure(Vector<String>& conf, ErrorHandler* errh)
{
	Args args = Args(this, errh).bind(conf);
	
	if(args.read_mp("PREFIX", _prefix).execute() < 0)
		return -1;
	
	errh->message("prefix: %s", _prefix.c_str());
	return 0;
}

void Empty14::push(int i, Packet* p)
{
	click_chatter("%s packet on port %i of length %u B", _prefix.c_str(), i, p->length());
	output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Empty14)
