% MYSCREEN(1) MyScreen User Manuals
NAME
===

myscreen - run GNU/Screen providing tab bar and several system statistics

SYNOPSIS
===

**myscreen** [ **restart** | **stop** ] [ **screen_options** ]

DESCRIPTION
===

*MyScreen* is a program that runs GNU/Screen adding a tab bar and running myscreen-stats in order to display system statistics.
It is written in C and it is expected to consume very few resources.

**MyScreen** includes one GNU/Screen configuration file and myscreen-stats binary.
The configuration file of GNU/Screen allows to enable hardstatus bar (at the bottom of the screen) in the
manner of tab support in certain graphical terminal emulators.

The myscreen-stats program provides several information like
the number of users connected, the uptime, the upload and download rates, the battery state, 
the wifi quality, the loadaverage, the number of processes, the cpus rates, the disks rates,
the ram and swap percentages.

**To configure myscreen, you should copy /etc/myscreenrc file to ~/.myscreenrc**

COMMANDS
===

*MyScreen* has the following commands:

- **restart**: Restart generation of stats (reloading configuration file). 
- **stop**: stop stats generation. 

COMMAND-LINE OPTIONS
===
All options are passed to GNU/Screen (see screen(1) for details). 

MODULES
===

*MyScreen* is composed of modules:

- **Users**: Displays the number of users connected (using getutent() function). 
- **Uptime**: Displays uptime of the system (using sysinfo() function).  
- **Network**: Displays upload (\^[rate] [unit]) and download rate (v[rate] [unit]). When myscreen starts or when you reload it, the selected device is displayed. Rates are calculated with /proc/net/dev file and device is selected with /proc/net/route file.
- **Battery**: Displays the battery level and charging state.  S|PERCENTAGE% ; S can be a '"' (Full) without percentage, a '_' (Discharging), or a '~' (Charging). Percentage and state are determinated with /sys/class/power_supply/BAT0/uevent file.
- **Wifi**: Displays wifi signal quality percentage.  Quality is provided by /proc/net/wireless file.
- **Reboot**: Displays '@' (blinking if supported) if reboot is required. It tests existence of /var/run/reboot-required file. 
- **Loadaverage**: Displays the load average. (using sysinfo() function)
- **Processes**: Displays the number of processes. (using sysinfo()) 
- **Cpu**: Diplays cpu rates : average%(cpu0%,cpu1%,...). Percentages are calculated with /proc/stat file. Rates include user space and kernel mode cpu time.
- **Disk**: Displays disks rates of reading and writing together. If no activity detected, it displays the device name. (using /proc/diskstats file)
- **Ram**: Displays the percentage of used ram without cache. (using sysinfo() function)
- **Swap**: Displays the percentage of used swap if activated. (using sysinfo() function)



FILES
===
**/etc/myscreenrc** configuration of *myscreen*
**$HOME/.myscreenrc** read instead of /etc/myscreenrc 
**/etc/myscreen-screenrc** screen configuration file.

SEE ALSO
===
**screen (1)**

AUTHORS
===

MyScreen was written by Clément Mondon <clement.mondon@gmail.com>.

This manual page was written by Clément Mondon <clement.mondon@gmail.com>, for the Debian project (and may be used by others).

