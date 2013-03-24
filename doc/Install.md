How to install
===

From sources
---

Download sources from [Github](https://github.com/mondonc/myscreen)

[Continuous integration](http://myscreen.mondonc.org/commits) page can help you to choose good commit. 

#### Compile and Install

	make
	make install

### Normal user (non-root) installation

If you want to install MyScreen in your *${HOME}*, you can download sources and run :

	make local
	make local-install


From Debian packages
---

You can simply add one of the following lines in your sources.list :

	deb http://myscreen.mondonc.org/debian/ stable/$(ARCH)
	deb http://myscreen.mondonc.org/debian/ testing/$(ARCH)

Packages are only compilated for i386 and amd64 architectures.
