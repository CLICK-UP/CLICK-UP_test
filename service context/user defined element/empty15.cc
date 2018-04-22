#include <click/config.h>
#include <click/error.hh>
#include <click/args.hh>

#include <cstring>

#include "empty15.hh"

CLICK_DECLS

Empty15::Empty15()
	: _prefix() {}

Empty15::~Empty15() {}

int Empty15::initialize(ErrorHandler *errh)
{
	errh->message("Successfully initialized.");
	return 0;
}

int Empty15::configure(Vector<String>& conf, ErrorHandler* errh)
{
	Args args = Args(this, errh).bind(conf);
	
	if(args.read_mp("PREFIX", _prefix).execute() < 0)
		return -1;
	
	errh->message("prefix: %s", _prefix.c_str());
	return 0;
}

void Empty15::push(int i, Packet* p)
{
	click_chatter("%s packet on port %i of length %u B", _prefix.c_str(), i, p->length());
	output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Empty15)
