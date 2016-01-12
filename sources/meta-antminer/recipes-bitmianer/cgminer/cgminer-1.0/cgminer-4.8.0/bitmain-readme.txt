######################################################################################
#                                                                                    #
#          BitMain setup and build instructions (on mingw32/Windows):                #
#                                                                                    #
######################################################################################

**************************************************************************************
* Build cgminer.exe                                                                  *
**************************************************************************************
Run the MinGW MSYS shell
(Start Icon/keyboard key ==> All Programs ==> MinGW ==> MinGW Shell).
Change the working directory to your CGMiner project folder.
Example: cd cgminer-2.1.2 [Enter Key] if you are unsure then type "ls -la"
Another way is to type "cd cg" and then press the tab key; It will auto fill.
Type the lines below one at a time. Look for problems after each one before going on
to the next.

      adl.sh (optional - see below)
      autoreconf -fvi
      CFLAGS="-O2 -msse2" ./configure (additional config options, see below)
      make
      strip cgminer.exe  <== only do this if you are not compiling for debugging
      
For bitmain mode:
      autoreconf -fvi
      CFLAGS="-O2 -msse2" ./configure --enable-bmsc 
      make

**************************************************************************************
* Some ./configure options                                                           *
**************************************************************************************
--enable-cpumining      Build with cpu mining support(default disabled)
--disable-opencl        Override detection and disable building with opencl
--disable-adl           Override detection and disable building with adl
--enable-bitforce       Compile support for BitForce FPGAs(default disabled)
--enable-icarus         Compile support for Icarus Board(default disabled)
--enable-bitmain        Compile support for BitMain Devices(default disabled)
--enable-modminer       Compile support for ModMiner FPGAs(default disabled)
--enable-ztex           Compile support for Ztex Board(default disabled)
--enable-scrypt         Compile support for scrypt litecoin mining (default disabled)
--without-curses        Compile support for curses TUI (default enabled)
--without-libudev       Autodetect FPGAs using libudev (default enabled)
--enable-forcecombo     Allow combinations of drivers not intended to be built together(default disabled)

**************************************************************************************
* Run cgminer for bitmain mode                                                       *
**************************************************************************************
BitMain options:
--bitmain-options baud:miner_count:asic_count:timeout:frequency

For example:
cgminer --bitmain-options 115200:24:10:30:300 -o http://stratum.btcguild.com:3333 -u xlc1985_1 -p abc123456 -D

######################################################################################
#                                                                                    #
#       BitMain setup and build instructions (on mingw32/Windows) complete           #
#                                                                                    #
######################################################################################
