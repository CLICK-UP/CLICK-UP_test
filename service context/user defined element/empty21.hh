#ifndef EMPTY21_H
#define EMPTY21_H
#include <click/element.hh>
CLICK_DECLS

class Empty21 : public Element {

public:	
	Empty21();
	~Empty21();
	
	const char* class_name() const { return "Empty21"; }
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
