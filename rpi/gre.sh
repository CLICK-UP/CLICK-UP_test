ovs-vsctl del-br clickup
ovs-vsctl add-br clickup
ovs-vsctl add-port clickup p1 -- set interface p1 type=internal ofport_request=1
ovs-vsctl add-port clickup p2 -- set interface p2 type=internal ofport_request=2
ifconfig p1 0 up
ifconfig p2 0 up
ifconfig p1 promisc
ifconfig p2 promisc
ifconfig p1 mtu 1400
ifconfig p2 mtu 1400
ovs-vsctl add-port clickup clickup-forward -- set interface clickup-forward type=gre options:in_key=flow options:local_ip=192.168.1.13 options:out_key=flow options:remote_ip=192.168.1.41
ovs-vsctl add-port clickup clickup-backward -- set interface clickup-backward type=gre options:in_key=flow options:local_ip=192.168.1.13 options:out_key=flow options:remote_ip=192.168.1.42
ovs-vsctl set interface clickup-forward ofport_request=3
ovs-vsctl set interface clickup-backward ofport_request=4
ovs-ofctl add-flow clickup "priority=10,in_port=3,actions=output:1"
ovs-ofctl add-flow clickup "priority=10,in_port=1,actions=output:3"
ovs-ofctl add-flow clickup "priority=10,in_port=4,actions=output:2"
ovs-ofctl add-flow clickup "priority=10,in_port=2,actions=output:4"
