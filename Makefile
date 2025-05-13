
ports.txt:
	ngspice sky130_fd_sc_hd__dfxtp_1.spice sky130_fd_sc_hd__dfxtp_1_tb.spice

sim: sim.cpp
	g++ -Ingspice-install/include/ngspice sim.cpp -Lngspice-install/lib -lngspice -g -o $@

run: sim
	SPICE_SCRIPTS=ngspice-install/share/ngspice/scripts LD_LIBRARY_PATH=ngspice-install/lib ./$<

clean:
	rm -rf ports.txt
