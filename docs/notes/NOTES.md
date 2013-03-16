
# Development/Deployment #

The process of developing and deploying a program on the cluster has many stages:

- Edit
  * Use your favorite editor to make a change to the source code of the program.

- Sync
  * Ensure all local headers, libraries, and configuration files are identical to those on each of the cells. This may be accomplished using [rsync][].

- Compile/Link
  * Use your cross-compiler environment to compile and link your program.

- Deploy
  * Copy all compiled binaries, libraries, configuration files, and other resources to each cell of the cluster. This may be accomplished using [murder][].
  * Arm the program by commanding each cell to run the deployed binary. This may be accomplished using [mpssh][].

- Control/Monitor
  * Send the start signal to each cell and provide control signals over the network. This may be accomplished using [0mq][] or [osc][].

- Kill
  * Send the kill signal to each cell.

- Clean
  * Ensure the program is off on each cell and that there are no problems on the cluster. 

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

[git][], [ssh][], [mpssh][], [rsync][], [ganglia][], [murder][], [puppet][]

[git]: http://git-scm.com/
[ssh]: http://en.wikipedia.org/wiki/Secure_Shell
[mpssh]: https://github.com/ndenev/mpssh
[rsync]: http://en.wikipedia.org/wiki/Rsync
[ganglia]: http://ganglia.sourceforge.net/
[murder]: https://github.com/lg/murder
[puppet]: http://en.wikipedia.org/wiki/Puppet_(software)
[0mq]: http://www.zeromq.org/
[osc]: http://en.wikipedia.org/wiki/Open_Sound_Control

<link href="http://kevinburke.bitbucket.org/markdowncss/markdown.css" rel="stylesheet"></link>
