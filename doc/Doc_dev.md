<?php 
/*Copyright 2009,2010,2011 Clément Mondon 
 This file is part of project myscreen.

Myscreen is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Myscreen is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Myscreen.  If not, see http://www.gnu.org/licenses/. 
 */
?>


<h1>How to install ?</h1>

<h2>Debian package</h2>
<p>
You can download Debian binary package <a href="http://svn.gna.org/svn/myscreen/build-area">here</a>. 
<br/>
</p> <p>
Or, you can add the following line in your sources.list :
<br/>
<pre>
deb http://svn.gna.org/svn/myscreen/build-area ./
</pre>
</p>
<br/>
Packages compilated to i386 and amd64 architectures.

<h2>Normal user</h2>

<p>If you want to install MyScreen in your ${HOME}, you can download source and run :
<pre>
make local
make local-install
</pre>
</p>


<h1>How to use ?</h1>
<h2>Use of GNU/Screen</h2>

Myscreen uses GNU/Screen window manager with the same default shortkeys.

<h3> Shortkeys</h3>
<p>
<pre>
C^A C : New tabs</li>
C^A A : Switch between two tabs </li>
C^A [0-9] : Go to tab [0-9]</li>
C^A " : Display a list of tabs</li>
C^A k : Close current tab</li>
C^A d : Detach GNU/Screen session (You can rattach it with [my]screen -r)</li>
</pre>
</p>


<?php include("manpage"); ?>

<h1>Sources</h1>
Source documentation: <a href="doc/">here</a>.
