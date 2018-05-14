ovs-vsctl del-br vfunc-br
ovs-vsctl add-br vfunc-br
ovs-vsctl add-port vfunc-br p1 -- set interface p1 ofport_request=1
ovs-vsctl set interface p1 type=internal
ifconfig p1 "IPAddress/netmask" up
ifconfig p1 promisc
ifconfig p1 mtu 1400
ovs-vsctl add-port vfunc-br vfunc-forward -- set interface vfunc-forward type=gre options:df_default=true options:in_key=flow options:local_ip=30.0.0.2 options:out_key=flow options:remote_ip=30.0.0.1
ovs-vsctl set interface vfunc-forward ofport_request=2
ovs-ofctl add-flow vfunc-br "priority=10,in_port=1,actions=output:2"
ovs-ofctl add-flow vfunc-br "priority=10,in_port=2,actions=output:1"