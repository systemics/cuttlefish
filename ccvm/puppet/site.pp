package {
  [
    'pssh',
    'iperf',
    'rsync',
    'vim',
#    'fish',
    'make',
    'cmake',
#    'openssh',
  ]: ensure => 'present'
}


#package { 'network-manager': ensure => 'purged' }
#file { "/home/sphere/.ssh":
#  ensure => directory,
#  recurse => true,
#  purge => true,
#  force => true,
#  owner => "sphere",
#  group => "sphere",
#  mode => 0600,
#  source => "puppet:///files/ssh",
#}
#
