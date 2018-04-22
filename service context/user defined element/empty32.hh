#ifndef EMPTY32_H
#define EMPTY32_H
#include <click/element.hh>
CLICK_DECLS

class Empty32 : public Element {

public:	
	Empty32();
	~Empty32();
	
	const char* class_name() const { return "Empty32"; }
	const char* port_count() const { return PORTS_1_1; }
	const char* processing() const { return PUSH; }
	const char* flow_code()  const { return "x/y"; }
	
	int initialize(ErrorHandler *errh);
	int configure(Vector<String>& conf, ErrorHandler* errh);
	void push(int i, Packet* p);

private:
	String _prefix;
};

CLICK_ENDDECLS

#endif
