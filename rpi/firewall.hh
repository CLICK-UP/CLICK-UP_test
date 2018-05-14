#ifndef FIREWALL_H
#define FIREWALL_H
#include <click/element.hh>
#include <click/straccum.hh>
#include <click/string.hh>
#include <click/hashmap.hh>
#include <click/ipflowid.hh>
#include <clicknet/tcp.h>
#include <cstring>

CLICK_DECLS

class Firewall : public Element {

public:	
	Firewall();
	~Firewall();
	
	const char* class_name() const { return "Firewall"; }
	const char* port_count() const { return "1/2"; }
	const char* processing() const { return PUSH; }
	const char* flow_code()  const { return "x/y"; }
	
	int configure(Vector<String>& conf, ErrorHandler* errh);
	void push(int i, Packet* p);

	int closedSel(uint8_t th_flags, IPFlowID flowid);
int listenSel(uint8_t th_flags, IPFlowID flowid);
int synRcvdSel(uint8_t th_flag, IPFlowID flowid);
int synSentSel(uint8_t th_flags, IPFlowID flowid);
int establishedSel(uint8_t th_flags, IPFlowID flowid);
int finWait1Sel(uint8_t th_flags, IPFlowID flowid);
int closeWaitSel(uint8_t th_flags, IPFlowID flowid);

private:
	HashMap<String, int> passMap;
HashMap<String, int> connectMap;

};

CLICK_ENDDECLS

#endif