
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <filesystem>
#include "sharedspice.h"

//— Callback to capture printf/fprintf output from ngspice
int cb_SendChar(char* msg, int /*id*/, void* /*ud*/) {
    std::cout << msg << std::endl;
    return 0;
}

//— Required controlled-exit callback
int cb_ControlledExit(int status, NG_BOOL /*immed*/, NG_BOOL /*quit*/, int /*id*/, void* /*ud*/) {
    std::cerr << "[ngspice exit] status=" << status << std::endl;
    std::exit(status);
}

//— Simple bg-thread indicator (unused here)
int cb_BgRunning(NG_BOOL /*running*/, int /*id*/, void* /*ud*/) {
    return 0;
}

//— This will be called by ngspice whenever it needs the value of an EXTERNAL voltage source.
//  We use it to drive our two “digital” nodes CLK_DIGITAL and D_DIGITAL via the DAC bridge.
int cb_GetVSRCData(double* value, double time, char* nodeName, int /*id*/, void* /*ud*/) {
    std::string nm(nodeName);
    if (nm == "vclk_digital") {
        double phase = std::fmod(time, 1e-9);
        *value = (phase < 0.5e-9 ? 0.0 : 1.8);
    }
    else if (nm == "D_DIGITAL") {
        // A simple pulse: low for first half, then high
        *value = (time < 5e-9 ? 0.0 : 1.8);
    }
    else {
        *value = 0.0;  // default
    }
    return 0;
}

//— Optional: capture each data point and print node Q
int cb_SendData(pvecvaluesall allvals, int /*count*/, int /*id*/, void* /*ud*/) {
    double time = NAN;
    double clk = NAN;
    double d = NAN;
    double q = NAN;
    for (int i = 0; i < allvals->veccount; ++i) {
        const vecvalues &v = *allvals->vecsa[i];
        // std::cout << "*allvals->vecsa[" << i << "]->name: " << v.name << std::endl;
        if (std::string(v.name) == "time") time = v.creal;
        else if (std::string(v.name) == "clk") clk = v.creal;
        else if (std::string(v.name) == "d") d = v.creal;
        else if (std::string(v.name) == "q") q = v.creal;
    }
    std::cout << "time=" << time << " clk=" << clk << " d=" << d << " q=" << q << std::endl;
    return 0;
}

//— We don’t need the vector-info callback for this example
int cb_SendInitData(pvecinfoall, int, void*) { return 0; }

int main() {
    // 1) Initialize ngspice with our callbacks
    ngSpice_Init(
        cb_SendChar,
        nullptr,               // SendStat
        cb_ControlledExit,
        cb_SendData,
        cb_SendInitData,
        cb_BgRunning,
        nullptr                // userData
    );

    // 2) Register the synchronous external-source callback
    int uniq_id = 0;
    ngSpice_Init_Sync(
        cb_GetVSRCData,
        nullptr,               // current‐source callback (unused)
        nullptr,               // sync callback (unused)
        &uniq_id,
        nullptr                // optional override userdata
    );

    std::vector<const char*> tb;
    tb.push_back(".lib \"$PDK_ROOT/sky130A/libs.tech/combined/sky130.lib.spice\" tt");
    tb.push_back("");
    tb.push_back(".include \"sky130_fd_sc_hd__dfxtp_1.spice\"");
    tb.push_back("X1 CLK D VGND VNB VPB VPWR Q sky130_fd_sc_hd__dfxtp_1");
    tb.push_back("");
    tb.push_back("VVGND VGND 0 0");
    tb.push_back("VVNB  VNB  0 0");
    tb.push_back("VVPB  VPB  0 1.8");
    tb.push_back("VVPWR VPWR 0 1.8");
    tb.push_back("");
    tb.push_back(".model dac_model dac_bridge(out_low=0 out_high=1.8 t_rise=2e-11 t_fall=2e-11)");
    tb.push_back("a_dac_bridge1 [CLK_DIGITAL] [CLK] dac_model");
    tb.push_back("a_dac_bridge2 [D_DIGITAL]   [D]   dac_model");
    tb.push_back("");
    tb.push_back(".tran 1n 20n");
    tb.push_back(".end");
    for (const auto & l : tb)
        std::cout << l << std::endl;

    if (ngSpice_Circ((char**)tb.data()) != 0) {
        std::cerr << "ERROR: failed to send testbench netlist\n";
        return 1;
    }

    // 7) Kick off the transient analysis
    ngSpice_Command((char*)"tran 1n 20n");

    while (ngSpice_running()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "Simulation finished.\n";

    return 0;
}
