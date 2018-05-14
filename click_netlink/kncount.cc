# include <click/config.h>
# include "kncount.hh"
# include <click/string.hh>
# include <click/glue.hh>
# include <click/args.hh>
# include <click/error.hh>
# include <click/straccum.hh>
#ifdef CLICK_LINUXMODULE
# include <click/cxxprotect.h>
CLICK_CXX_PROTECT
# include <linux/sched.h>
# include <linux/netlink.h>
# include <linux/kernel.h>
# include <linux/module.h>
# include <linux/types.h>
# include <net/sock.h>
CLICK_CXX_UNPROTECT
# include <click/cxxunprotect.h>
#endif
#define NETLINK_TEST 31
CLICK_DECLS

extern "C"{
  static struct sock *nl_sk = NULL;
/*  int send_to_user(void *arg){
    int size;
    struct sk_buff *skb;
    unsigned int old_tail;
    int retval;
    struct tcmsg *tcm;
    struct nlmsghdr *nlh;
    struct nlmsghdr *hrd = arg;
    tcm = (struct tcmsg *)kmalloc(sizeof(struct tcmsg), GFP_KERNEL);
    size = NLMSG_SPACE(sizeof(struct tcmsg));
    skb = alloc_skb(size, GFP_ATOMIC);
    old_tail = skb->tail;
    nlh = nlmsg_put(skb, 0, 0, 0, NLMSG_SPACE(sizeof(struct tcmsg))-sizeof(struct nlmsghdr),0);
    nlh->nlmsg_pid   = hrd->nlmsg_pid;
    tcm = nlmsg_data(nlh);
    tcm->tcm_family = AF_UNSPEC;
    tcm->tcm__pad1 = 0;
    tcm->tcm__pad2 = 0;
    tcm->tcm_ifindex = 1;
    tcm->tcm_parent = 1001;
    tcm->tcm_info = 1002;
    nlh->nlmsg_len = skb->tail - old_tail;
    NETLINK_CB(skb).portid = 0;
    NETLINK_CB(skb).dst_group = 0;
    click_chatter("[kernel space] skb->data:%s\n", (char *)NLMSG_DATA((struct nlmsghdr *)skb->data));
    retval = netlink_unicast(netlinkfd, skb, hrd->nlmsg_pid, MSG_DONTWAIT);
    click_chatter("[kernel space] netlink_unicast return: %d\n", retval);
    return 0;
  }
  void kernel_receive(struct sk_buff *__skb){
    struct sk_buff  *skb;
    struct nlmsghdr *nlh  = NULL;
    char *data = "This is eric's test message from kernel";
    skb = skb_get(__skb);
    if(skb->len >= sizeof(struct nlmsghdr)){
      nlh = (struct nlmsghdr *)skb->data;
      if((nlh->nlmsg_len >= sizeof(struct nlmsghdr))
        && (__skb->len >= nlh->nlmsg_len)){
        click_chatter("[kernel space] data receive from user are:%s\n", (char *)NLMSG_DATA(nlh));
        send_to_user(nlh);
      }
    }else{
      click_chatter("[kernel space] data receive from user are:%s\n", (char *)NLMSG_DATA(nlmsg_hdr(__skb)));
      send_to_user(data);
    }
    kfree_skb(skb);
  }
*/
  static int pid;
  static void hello_nl_recv_msg(struct sk_buff *skb) {
    struct nlmsghdr *nlh;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg="Hello from kernel";
    int res;
    click_chatter("Entering: %s\n", __FUNCTION__);
    msg_size=strlen(msg);
    nlh=(struct nlmsghdr*)skb->data;
    click_chatter("Netlink received msg payload:%s\n", (char*)nlmsg_data(nlh));
    pid = nlh->nlmsg_pid; 
    skb_out = nlmsg_new(msg_size,0);
    if(!skb_out){
      click_chatter("Failed to allocate new skb\n");
      return;
    }
    nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0); 
    NETLINK_CB(skb_out).dst_group = 0;
    strncpy(nlmsg_data(nlh),msg,msg_size);
    res=nlmsg_unicast(nl_sk,skb_out,pid);
    if(res<0){
      click_chatter("Error while sending bak to user\n");
    }
  }

  static int hello_init(void){
    struct netlink_kernel_cfg cfg;
    cfg.input = hello_nl_recv_msg;
    nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
    if(!nl_sk){
      click_chatter("can not create a netlink socket\n");
      return -1;
    }else{
      click_chatter("has created a netlink socket\n");
    }
    return 0;
  }

  static void hello_exit(void)
  {
    click_chatter("exiting hello module\n");
    netlink_kernel_release(nl_sk);
  }

}


KNCount::KNCount()
{
  counts = 0;
}

KNCount::~KNCount()
{
  hello_exit();
}

int
KNCount::configure(Vector<String> &conf, ErrorHandler* errh)
{
  click_chatter("init kernel module");
  hello_init();
  return 0;
}

Packet *
KNCount::simple_action(Packet *p)
{
  counts++;
  
  struct nlmsghdr *rep;
  struct sk_buff *skb;
  int msg_size;
  int *msg = &counts;
  int res;
  msg_size=sizeof(counts);
  skb = nlmsg_new(msg_size,0);
  if(!skb){
    click_chatter("Failed to allocate new skb\n");
    return p;
  }
  rep=nlmsg_put(skb,0,0,NLMSG_DONE,msg_size,0);
  NETLINK_CB(skb).dst_group = 0;
  memcpy(nlmsg_data(rep),msg,msg_size);
  click_chatter("nlmsg_data = %d\n", *(int *)(nlmsg_data(rep)));
  res=nlmsg_unicast(nl_sk,skb,pid);
  if(res<0){
    click_chatter("Error while sending bak to user\n");
  }

  return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(KNCount)
ELEMENT_MT_SAFE(KNCount)

