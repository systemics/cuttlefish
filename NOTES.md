
Troubles
  Internationalization and Localization
    In our Ljubljana workshop many participants had international keyboards
    and/or used non-english character sets. Our linux-based development
    environment added to the trouble because it was configured for English/US
    only.

  Windows SSH
    About half of workshop participants used windows, which does not have a
    command line ssh client or bash. This made it impossible for these
    participants to do "vagrant ssh". We installed and configured putty, but
    there was still a problem with ssh keys. We need a solution for windows
    users.

  Updating/Managing Cross-Compiling Virtual Machine (ccvm)
    We would make changes to the ccvm to fix things, but these would only be
    applied to select instances. We wanted *all* the ccvms to change.
    Supposedly this should be doable with puppet and some extra stuff in the
    vagrant config.

