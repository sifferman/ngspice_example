
# Ngspice Example

Example of how to run Ngspice on the [Sky130 DFXTP cell](https://github.com/fossi-foundation/skywater-pdk-libs-sky130_fd_sc_hd/blob/main/cells/dfxtp/sky130_fd_sc_hd__dfxtp_1.spice).

One option is using the Ngspice application and a `.stim` file. See [`".github/workflows/run.yml"`](https://github.com/sifferman/ngspice_example/blob/main/.github/workflows/run.yml) for an example of how to install all dependencies and run the simulation.

Another option is using the Ngspice shared API and a `.cpp` program. See [`".github/workflows/shared.yml"`](https://github.com/sifferman/ngspice_example/blob/main/.github/workflows/run.yml) for an example of how to install all dependencies and run the simulation.

## Dependencies

* Sky130 PDK via [Ciel](https://github.com/fossi-foundation/ciel)
* [Ngspice](https://ngspice.sourceforge.io/)

## References

* <https://ngspice.sourceforge.io/docs/ngspice-html-manual/manual.xhtml>
