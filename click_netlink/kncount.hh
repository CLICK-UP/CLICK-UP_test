#ifndef CLICK_KNCOUNT_HH
#define CLICK_KNCOUNT_HH
#include <click/element.hh>
#include <click/string.hh>
CLICK_DECLS
class KNCount : public Element { public:

    KNCount() CLICK_COLD;
    ~KNCount() CLICK_COLD;

    const char *class_name() const              { return "KNCount"; }
    bool can_live_reconfigure() const           { return true; }
    const char *port_count() const              { return PORTS_1_1; }

    int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;

    Packet *simple_action(Packet *);

 private:

    int counts;         // How many bytes of a packet to print
};

CLICK_ENDDECLS
#endif

