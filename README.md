
# Ngspice Example

Example of how to run Ngspice on the [Sky130 DFXTP cell](https://github.com/fossi-foundation/skywater-pdk-libs-sky130_fd_sc_hd/blob/main/cells/dfxtp/sky130_fd_sc_hd__dfxtp_1.spice). It takes in a `.stim` file, which describes the voltages for each input.

See [`".github/workflows/run.yml"`](https://github.com/sifferman/ngspice_example/blob/main/.github/workflows/run.yml) for an example of how to install all dependencies and run the simulation.

## Dependencies

* Sky130 PDK via [Ciel](https://github.com/fossi-foundation/ciel)
* [Ngspice](https://ngspice.sourceforge.io/)
