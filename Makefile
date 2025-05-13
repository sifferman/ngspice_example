
NGSPICE_ROOT := ${HOME}/tools/ngspice

ports.txt:
	ngspice sky130_fd_sc_hd__dfxtp_1.spice sky130_fd_sc_hd__dfxtp_1_tb.spice

sim: sim.cpp
	g++ -I${NGSPICE_ROOT}/include/ngspice sim.cpp -L${NGSPICE_ROOT}/lib -lngspice -g -o $@

run: sim
	SPICE_SCRIPTS=${NGSPICE_ROOT}/share/ngspice/scripts LD_LIBRARY_PATH=${NGSPICE_ROOT}/lib ./$<

clean:
	rm -rf ports.txt
