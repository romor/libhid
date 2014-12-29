libhid
======

This is just a fork of https://github.com/chad3814/libhid where I tried to resolve some build issues and the SWIG Python bindings.
chad3814/libhid is also just a fork of the original development at svn://svn.debian.org/libhid/trunk, which seems to be inactive since several years.
So, we are both not the maintainer and did not really touch the code at all.

If you want to build:
 - ensure to have the following packages installed: autoconf, automake, libtool, libusb-dev, xsltproc, docbook-xsl, gcc, debhelper, pkg-config
 - for using SWIG, you also need: swig, python3-dev, python3
 - to build, run the autogen.sh in the top-level directory followed by make, and potentially make install

See https://github.com/chad3814/libhid/blob/master/README.svn for the original README from svn.
Also the original license keeps valid.
