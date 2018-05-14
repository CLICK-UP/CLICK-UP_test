icla :: Classifier(12/0800, -);
ifi :: Firewall;
to_extern :: FullNoteQueue -> ToDevice(p2);
FromDevice(p1) -> icla;
icla[1] -> to_extern
icla[0] -> Strip(14)
	-> CheckIPHeader
	-> ifi;
ifi[1] -> Log -> Discard;
ifi[0] -> EtherEncap(0x0800, fa:fe:ca:5d:97:6c, ee:43:35:3d:55:7c) ->to_extern;
FromDevice(p2) -> FullNoteQueue -> ToDevice(p1);
