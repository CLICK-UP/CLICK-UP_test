#include <click/config.h>
#include <click/error.hh>
#include <click/args.hh>
#include "firewall.hh"
CLICK_DECLS

#define CLOSED_STATE 0
#define LISTEN_STATE 1
#define SYN_RCVD_STATE 2
#define ESTABLISHED_STATE 3
#define SYN_SENT_STATE 4
#define CLOSE_WAIT_STATE 5
#define FIN_WAIT_1_STATE 6
#define TIME_WAIT_STATE 7
#define LAST_ACK_STATE 8

Firewall::Firewall() {
	FILE *connectStateFp = NULL;
char connectBuff[255];
connectStateFp = fopen("./connectmap.state", "r");
if(connectStateFp != NULL){
while(!feof(connectStateFp)){
fgets(connectBuff, 255, (FILE*)connectStateFp);
String str(connectBuff);
if(str.length() > 6){
connectMap.insert(str.substring(0,str.length()-3), str.at(str.length()-2)-'0');
}else{
break;
}
}
}
FILE *passStateFp = NULL;
char passBuff[255];
passStateFp = fopen("./passmap.state", "r");
if(passStateFp != NULL){
while(!feof(passStateFp)){
fgets(passBuff, 255, (FILE*)passStateFp);
String str(passBuff);
if(str.length() > 6){
passMap.insert(str.substring(0,str.length()-3), str.at(str.length()-2)-'0');
}else{
break;
}
}
}

}

Firewall::~Firewall() {
	StringAccum connectStateSa;
for (HashMap<String, int>::iterator it = connectMap.begin(); it != connectMap.end() ; ++it)
{
connectStateSa << it.key() << " " << it.value() << '\n';
}
FILE *connectStateFp = NULL;
connectStateFp = fopen("./connectmap.state", "w");
fputs(connectStateSa.c_str(), connectStateFp);
fclose(connectStateFp);
StringAccum passStateSa;
for (HashMap<String, int>::iterator it = passMap.begin(); it != passMap.end() ; ++it)
{
passStateSa << it.key() << " " << it.value() << '\n';
}
FILE *passStateFp = NULL;
passStateFp = fopen("./passmap.state", "w");
fputs(passStateSa.c_str(), passStateFp);
fclose(passStateFp);

}

int Firewall::configure(Vector<String>& conf, ErrorHandler* errh)
{
	if (Args(conf, this, errh)

.complete() < 0)
	return -1;
	return 0;
}

int Firewall::closedSel(uint8_t th_flags, IPFlowID flowid){
int res;
switch(th_flags){
case TH_SYN : {
connectMap.insert(flowid.unparse(), SYN_SENT_STATE);
IPFlowID retFlowid = flowid.reverse();
connectMap.insert(retFlowid.unparse(), LISTEN_STATE);
res = 0;
break;
}
default : {
res = 2;
break;
}
}
return res;
}
int Firewall::listenSel(uint8_t th_flags, IPFlowID flowid){
int res;
switch(th_flags){
case TH_SYN|TH_ACK : {
connectMap.insert(flowid.unparse(), SYN_RCVD_STATE);
res = 0;
break;
}
default : {
res = 2;
break;
}
}
return res;
}
int Firewall::synRcvdSel(uint8_t th_flags, IPFlowID flowid){
int res;
switch(th_flags){
case TH_ACK : {
connectMap.insert(flowid.unparse(), ESTABLISHED_STATE);
res = 1;
break;
}
case TH_PUSH|TH_ACK : {
connectMap.insert(flowid.unparse(), ESTABLISHED_STATE);
res = 1;
break;
}
case TH_FIN : {
connectMap.insert(flowid.unparse(), FIN_WAIT_1_STATE);
res = 0;
break;
}
default : {
res = 2;
break;
}
}
return res;
}
int Firewall::synSentSel(uint8_t th_flags, IPFlowID flowid){
int res;
switch(th_flags){
case TH_ACK : {
connectMap.insert(flowid.unparse(), ESTABLISHED_STATE);
res = 1;
break;
}
case TH_PUSH|TH_ACK : {
connectMap.insert(flowid.unparse(), ESTABLISHED_STATE);
res = 1;
break;
}
default : {
res = 2;
break;
}
}
return res;
}
int Firewall::establishedSel(uint8_t th_flags, IPFlowID flowid){
int res;
switch(th_flags){
case TH_FIN : {
connectMap.insert(flowid.unparse(), FIN_WAIT_1_STATE);
res = 0;
break;
}
case TH_FIN|TH_ACK : {
connectMap.insert(flowid.unparse(), FIN_WAIT_1_STATE);
res = 0;
break;
}
case TH_ACK : {
IPFlowID retFlowid = flowid.reverse();
if(connectMap.find(retFlowid.unparse()) == FIN_WAIT_1_STATE){
connectMap.insert(flowid.unparse(), CLOSE_WAIT_STATE);
res = 0;
}
break;
}
default : {
res = 2;
break;
}
}
return res;
}
int Firewall::finWait1Sel(uint8_t th_flags, IPFlowID flowid){
int res;
switch(th_flags){
case TH_ACK : {
connectMap.insert(flowid.unparse(), TIME_WAIT_STATE);
res = 0;
break;
}
default : {
res = 2;
break;
}
}
return res;
}
int Firewall::closeWaitSel(uint8_t th_flags, IPFlowID flowid){
int res;
switch(th_flags){
case TH_FIN : {
connectMap.insert(flowid.unparse(), LAST_ACK_STATE);
res = 0;
}
default : {
res = 2;
break;
}
}
return res;
}


void Firewall::push(int i, Packet* p_in)
{
	int n;
	WritablePacket *p = p_in->uniqueify();
click_tcp *tcp = p->tcp_header();
IPFlowID flowid(p);
int passState = passMap.find(flowid.unparse() + tcp->th_flags);
int connectState = connectMap.find(flowid.unparse());
int op = -1;

	switch(passState){
case 1 : output(0).push(p); return;
case 2 : break;
}
switch(connectState){
case CLOSED_STATE : op = closedSel(tcp->th_flags, flowid); break;
case LISTEN_STATE : op = listenSel(tcp->th_flags, flowid); break;
case SYN_RCVD_STATE : op = synRcvdSel(tcp->th_flags, flowid); break;
case SYN_SENT_STATE : op = synSentSel(tcp->th_flags, flowid); break;
case ESTABLISHED_STATE : op = establishedSel(tcp->th_flags, flowid); break;
case FIN_WAIT_1_STATE : op = finWait1Sel(tcp->th_flags, flowid); break;
case CLOSE_WAIT_STATE : op = closeWaitSel(tcp->th_flags, flowid); break;
}
switch(op){
case 0 : break;
case 1 : passMap.insert(flowid.unparse()+TH_ACK, 1); passMap.insert(flowid.unparse()+(TH_ACK|TH_PUSH), 1); n=0; break;
case 2 : n=1; break;
}

	output(n).push(p);
	
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Firewall)



