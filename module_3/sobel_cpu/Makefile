BIN=sobel
RES=cpu

CC=gcc

CFLAGS=-I/usr/include/gtk-2.0 \
	-I/usr/lib/x86_64-linux-gnu/gtk-2.0/include \
	-I/usr/include/gio-unix-2.0/ \
	-I/usr/include/cairo -I/usr/include/pango-1.0 \
	-I/usr/include/atk-1.0 \
	-I/usr/include/cairo \
	-I/usr/include/pixman-1 \
	-I/usr/include/libpng16 \
	-I/usr/include/gdk-pixbuf-2.0 \
	-I/usr/include/libpng16 \
	-I/usr/include/pango-1.0 \
	-I/usr/include/harfbuzz \
	-I/usr/include/pango-1.0 \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
	-I/usr/include/freetype2 \
	-I/usr/include/gimp-2.0 \
	-I/usr/lib64/glib-2.0/include \
	-I/usr/lib64/gtk-2.0/include \
	-I/usr/include/libpng12

LDFLAGS=-lgimpui-2.0 \
	-lgimpwidgets-2.0 \
	-lgimpmodule-2.0 \
	-lgimp-2.0 \
	-lgimpmath-2.0 \
	-lgimpconfig-2.0 \
	-lgimpcolor-2.0 \
	-lgimpbase-2.0 \
	-lgtk-x11-2.0 \
	-lgdk-x11-2.0 \
	-lpangocairo-1.0 \
	-latk-1.0 \
	-lcairo \
	-lgdk_pixbuf-2.0 \
	-lgio-2.0 \
	-lpangoft2-1.0 \
	-lpango-1.0 \
	-lgobject-2.0 \
	-lglib-2.0 \
	-lfontconfig \
	-lfreetype \
	-lm \
	-lrt

.PHONY: all clean build_${BIN}_${RES} install_${BIN}_${RES}

all: install_${BIN}_${RES}

build_${BIN}_${RES}:
	${CC} ${CFLAGS} -pthread -c -o ${BIN}_${RES}.o ${BIN}_${RES}.c
	${CC} ${BIN}_${RES}.o -o sobel_${RES} ${LDFLAGS}

install_${BIN}_${RES}: build_${BIN}_${RES}
	cp ${BIN}_${RES} '${HOME}/.gimp-2.6/plug-ins/' || true
	#cp ${BIN}_${RES} '${HOME}/.gimp-2.8/plug-ins/' || true

clean:
	rm -f ${BIN}_${RES} ${BIN}_${RES}.o
