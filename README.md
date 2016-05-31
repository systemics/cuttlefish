# Cuttlefish

Cuttlefish is an extremely low-cost, low-power, hyper-resolution interactive
audio-visual Display Cluster based on the Raspberry Pi. It is developed in the
Systemics Lab at UCSB by Karl Yerkes and Pablo Colapinto under the supervision
of Marko Peljhan.

Find the latest version of [Cuttlefish][].

# Getting Started with Cuttlefish

example/

	xBasicApp.cpp: using cuttlebone and gfx, demonstrates bare bones use of ctl\_app.h
	xRawApp.cpp: using cuttlebone and gfx, demonstrates the "guts" of ctl\_app.h
	xField.cpp: using cuttlebone and gfx and versor to warp a field of points
	xAgents.cpp: using cuttlebone and gfx and versor to simulatie a swarm (controlled by a simulator machine)


## Download and Install Dependencies

You'll need to download and install some software before you start developing
with Cuttlefish. We use [Git][] to manage source code and [VirtualBox][] and
[Vagrant][] to support our development environment.

- [Download/Install - Git][]
- [Download/Install - VirtualBox][]
- [Download/Install - Vagrant][]

## Clone the Cuttlefish working directory and start the development environment

    git clone https://github.com/systemics/cuttlefish.git
    cd cuttlefish
    git submodule init
    git submodule update
    cd rpcc
    vagrant up
    vagrant ssh

## Configure Hardware and Network


[-------------------------------------------------------------------------]:
[Git]: http://git-scm.com
[Download/Install - Git]: http://git-scm.com/download
[VirtualBox]:https://www.virtualbox.org
[Download/Install - VirtualBox]:https://www.virtualbox.org/wiki/Downloads
[Vagrant]: http://www.vagrantup.com
[Download/Install - Vagrant]: http://www.vagrantup.com/downloads.html
[Cuttlefish]: https://github.com/systemics/cuttlefish

