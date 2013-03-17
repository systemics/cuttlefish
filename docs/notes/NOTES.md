
# Development/Deployment #

The process of developing and deploying a program on the cluster has many stages:

- Edit
  * Use your favorite editor to make a change to the source code of the program.

- Sync (optional)
  * Ensure all local headers, libraries, and configuration files are identical to those on each of the cells. This may be accomplished using [rsync][]. This only needs to be done after some library or package is installed to the cluster cells.

- Compile/Link
  * Use your cross-compiler environment to compile and link your program.

- Deploy
  * Copy all compiled binaries, libraries, configuration files, and other resources to each cell of the cluster. This may be accomplished using [murder][].
  * Arm the program by commanding each cell to run the deployed binary. This may be accomplished using [mpssh][].

- Control/Monitor
  * Send the start signal to each cell and provide control signals over the network. This may be accomplished using [0mq][] or [osc][].

- Kill/Clean
  * Send the kill signal to each cell.
  * Ensure the program is off on each cell and that there are no problems on the cluster. 

...

# Questions #

Here's some open questions:

- How do we measure the synchronization the displays?
  * I'm considering installing light sensors at the bottom left corner of each display and alternating black/white in a 10 by 10 pixel square. Then looking at the "square waves" on an oscilloscope or logic analyser.
  * Another idea is tapping into the DVI cable electrically somehow.
  * The easiest thing would be if we could query the GPU for the state of the HDMI/DVI signal or get a callback somehow.

- Is there a way to influence or even know the HDMI/DVI/LCD clock rate?

- How do we measure the synchronization the clocks on each cell?
  * Each cell runs the Network Time Protocol daemon ([ntpd][]). This daemon may have configuration options that tighten-up the timing of the clocks in the cluster. There also may be a tool that uses some fancy algorithm to measure the sync between ntp-sync'd clocks.

- Is there a faster than 10/100 ethernet way to distribute state to each of the cells?
  * Maybe it will eventually be possible to use the [CSI-2][] interface, but as of today the camera interface is not supported.

- How is Edgar Berdahl's [Satellite CCRMA][] image different than the mainstream image?
  * We need to email Edgar and ask or maybe do some diff on the entire filesystem to determine this.

- Is it possible or useful to increase the [MTU][] of the network?
  * Most switches, routers, and computers fragment network data into ~1400 byte chunks at the MAC/PHYS network layer, so if your application sends packets larger that one [MTU][], there will be overhead on the MAC/PHYS layer as your big packets get fragmented. In special cases, it is possible to configure your network with an [MTU][] larger than ~1400 bytes. Is this possible in our system? Would it help or hurt?

- How long does glReadPixels(...) take?

- Will [murder][] work easily from within the cross-compiler environment (a virtual machine)?

...

# Performance #

TBD: need to test all this stuff.

- Network
- CPU
- Graphics
- Audio

...

# Tools/Packages #

Common cluster operations such as development, deployment, configuation, startup, shutdown, calibration, and maintainance are facilitated by a set of command-line tools and open technologies. Most of these tools are available from the package manager, but some are not.

Available from the package manager:

- [git][] for versioning source code
- [ssh][] for authentication with the cluster and with git repos
- [rsync][] for copying files from the cluster in support of cross-compiling
- [ganglia][] for monitoring each cell in the cluster
- [puppet][] for managing operating system configuration for each cell

Package names:

- git, screen, ganglia-webfrontend, puppet-master, screen, rsync, puppet, gmetad, ganglia-monitor

Not available from the package manager:

- [mpssh][] for commanding all cells in the cluster at once
- [murder][] for distributing data to the cluster via BitTorrent

...

# References #

[git][], [ssh][], [mpssh][], [rsync][], [ganglia][], [murder][], [puppet][],
[0mq][], [osc][], [CSI-2][], [ntpd][], [Satellite CCRMA][], [MTU][]

[git]: http://git-scm.com/
[ssh]: http://en.wikipedia.org/wiki/Secure_Shell
[mpssh]: https://github.com/ndenev/mpssh
[rsync]: http://en.wikipedia.org/wiki/Rsync
[ganglia]: http://ganglia.sourceforge.net/
[murder]: https://github.com/lg/murder
[puppet]: http://en.wikipedia.org/wiki/Puppet_(software)
[0mq]: http://www.zeromq.org/
[osc]: http://en.wikipedia.org/wiki/Open_Sound_Control
[CSI-2]: http://www.mipi.org/specifications/camera-interface#CSI2
[ntpd]: http://en.wikipedia.org/wiki/Ntpd
[Satellite CCRMA]: https://ccrma.stanford.edu/~eberdahl/Satellite
[MTU]: http://en.wikipedia.org/wiki/Maximum_transmission_unit

<link href="http://kevinburke.bitbucket.org/markdowncss/markdown.css" rel="stylesheet"></link>
