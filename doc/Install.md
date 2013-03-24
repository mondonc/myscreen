How to install
===

From source
---

### Normal user

If you want to install MyScreen in your ${HOME}, you can download source and run :

 make local
 make local-install


Debian package
---
Or, you can add the following line in your sources.list :

 deb http://svn.gna.org/svn/myscreen/build-area ./

Packages compilated to i386 and amd64 architectures.


How to use ?
===

Use of GNU/Screen
---

Myscreen uses GNU/Screen window manager with the same default shortkeys.

### Shortkeys

 C^A C : New tabs</li>
 C^A A : Switch between two tabs </li>
 C^A [0-9] : Go to tab [0-9]</li>
 C^A " : Display a list of tabs</li>
 C^A k : Close current tab</li>
 C^A d : Detach GNU/Screen session (You can rattach it with [my]screen -r)</li>


<?php include("manpage"); ?>
