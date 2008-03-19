# Wild.pm - emulate shell @ARGV expansion on shells that don't
use File::DosGlob;
@ARGV = map {
              my @g = File::DosGlob::glob($_) if /[*?]/;
              @g ? @g : $_;
            } @ARGV;
1;
