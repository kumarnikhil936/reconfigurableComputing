BIN=gray
RES=fpga

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
	-lrt \
	-lmaxeleros \
	-lgray_max_object

MAXOSDIR_SIM=/upb/departments/agce/modules/software/maxeler/compiler/Maxcompiler-2014.2_vectis/lib/maxeleros-sim
MAXOSDIR_DFE=/opt/maxeler/maxeleros/

.PHONY: all clean build_${BIN}_${RES} install_${BIN}_${RES}

all: install_${BIN}_${RES}

build_${BIN}_${RES}:
	${CC} ${CFLAGS} -pthread -c -o ${BIN}_${RES}.o ${BIN}_${RES}.c
	${CC} ${BIN}_${RES}.o -o ${BIN}_${RES} -L${MAXOSDIR_DFE}/lib/ -L/home/fabianwt/workspace/GrayFPGA/RunRules/simulation/binaries ${LDFLAGS}

install_${BIN}_${RES}: build_${BIN}_${RES}
	cp ${BIN}_${RES} '${HOME}/.gimp-2.6/plug-ins/' || true
	#cp ${BIN}_${RES} '${HOME}/.gimp-2.8/plug-ins/' || true

run_sim:
	maxcompilersim -n sim_${USER} -c MAX3424A -d 1 restart
	echo "=== START =================="
	MAXELEROSDIR=${MAXOSDIR_SIM} LD_PRELOAD=${MAXOSDIR_SIM}/lib/libmaxeleros.so LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MAXOSDIR_SIM}/lib/:/home/fabianwt/workspace/GrayFPGA/RunRules/simulation/binaries SLIC_CONF="${SLIC_CONF};use_simulation=sim_${USER}"  gimp
	echo "=== STOP  =================="
	maxcompilersim -n sim_${USER} -c MAX3424A -d 1 stop

run_dfe:
	MAXELEROSDOR=${MAXOSDIR_DFE} LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${MAXOSDIR_DFE}/lib/:/home/fabianwt/workspace/GrayFPGA/RunRules/simulation/binaries gimp

clean:
	rm -f ${BIN}_${RES} ${BIN}_${RES}.o
