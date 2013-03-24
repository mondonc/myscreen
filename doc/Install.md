How to install
===

From sources
---

Download sources from [Github](https://github.com/mondonc/myscreen)

	make
	make install

### Normal user (non-root) installation

If you want to install MyScreen in your ${HOME}, you can download sources and run :

	make local
	make local-install


From Debian packages
---

Or, you can add the following lines in your sources.list :

	deb http://myscreen.mondonc.org/debian/ stable/$(ARCH)
	deb http://myscreen.mondonc.org/debian/ testing/$(ARCH)

Packages are compilated for i386 and amd64 architectures.
