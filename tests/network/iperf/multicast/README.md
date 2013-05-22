

on machines with multiple interfaces, you might have to add routes

    route add 224.0.55.55 dev eth10

# on master #

    iperf -c 224.0.55.55 -u -b 100M

# on cluster #

    iperf -s -u -B 224.0.55.55 

