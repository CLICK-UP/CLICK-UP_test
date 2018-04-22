#ifndef EMPTY31_H
#define EMPTY31_H
#include <click/element.hh>
CLICK_DECLS

class Empty31 : public Element {

public:	
	Empty31();
	~Empty31();
	
	const char* class_name() const { return "Empty31"; }
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
