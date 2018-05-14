#ifndef LOG_H
#define LOG_H
#include <click/element.hh>
#include <cstring>

CLICK_DECLS

class Log : public Element {

public:	
	Log();
	~Log();
	
	const char* class_name() const { return "Log"; }
	const char* port_count() const { return "1/1"; }
	const char* processing() const { return PUSH; }
	const char* flow_code()  const { return "x/y"; }
	
	int configure(Vector<String>& conf, ErrorHandler* errh);
	void push(int i, Packet* p);

	

private:
	
};

CLICK_ENDDECLS

#endif