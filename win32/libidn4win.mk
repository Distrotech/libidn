REL=libidn-1.20
TGZ=$(REL).tar.gz
URL=ftp://ftp.gnu.org/gnu/libidn/$(TGZ)

all: libidn4win32 libidn4win64

libidn4win:
	rm -rf tmp && mkdir tmp && cd tmp && \
	wget $(URL) && \
	tar xfa $(TGZ) && \
	cd $(REL) && \
	./configure --host=$(HOST) --build=x86_64-unknown-linux-gnu --prefix=$(PWD)/tmp/root && \
	make install && \
	cd .. && \
	cd root && \
	zip -r ../../$(REL)-win$(ARCH).zip *

libidn4win32:
	$(MAKE) -f libidn4win.mk libidn4win ARCH=32 HOST=i686-w64-mingw32

libidn4win64:
	$(MAKE) -f libidn4win.mk libidn4win ARCH=64 HOST=x86_64-w64-mingw32
