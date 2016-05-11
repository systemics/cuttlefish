# Cuttlefish

Cuttlefish is an extremely low-cost, low-power, hyper-resolution interactive
audio-visual Display Cluster based on the Raspberry Pi, with an
NVidia Jetson TK 1 thrown in for good measure.

A highly collaborative project, Cuttlefish includes open source code developed
by Karl Yerkes and Pablo Colapinto at UCSB with the support of both the Systemics Lab
(directed by Marko Peljhan) and the AlloSphere Research Facility
(directed by JoAnn Kuchera-Morin), and two Robert W. Deutsch Foundation fellowships.


Find the latest version of [Cuttlefish](github.com/systemics/cuttlefish.git).

Current Library Components:

* Cuttlebone (state synchronization)
* GFX (Embedded Graphics Library)
* Gamma (Generic Synthesis Library)
* Versor (Geometric Algebra Library)


# Getting Started with Cuttlefish

example/

	xBasicApp.cpp: using cuttlebone and gfx, demonstrates bare bones use of ctl\_app.h
	xRawApp.cpp: using cuttlebone and gfx, demonstrates the "guts" of ctl\_app.h
	xField.cpp: using cuttlebone and gfx and versor to warp a field of points
	xAgents.cpp: using cuttlebone and gfx and versor to simulatie a swarm (controlled by a simulator machine)


## Download and Install Dependencies

You'll need to download and install some software before you start developing
with Cuttlefish. We use [Git][] to manage source code and run raspberry-pi
cross compiling tools

- [Download/Install - Git][]


## Clone the Cuttlefish working directory and start the development environment

    git clone https://github.com/systemics/cuttlefish.git
    cd cuttlefish
    git submodule init
    git submodule update
    cd cross
    ./get_xtools


## Configure Hardware and Network


[-------------------------------------------------------------------------]:
[Git]: http://git-scm.com
[Download/Install - Git]: http://git-scm.com/download
[Cuttlefish]: https://github.com/systemics/cuttlefish
