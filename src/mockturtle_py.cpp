#include <stdio.h>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "mockturtle/mockturtle.hpp"
#include "lorina/aiger.hpp"

class Mockturtle_mig_api{
    public:
        explicit Mockturtle_mig_api(){
            mockturtle::mig_network mig;
            _mig = mig;
            return ;
        };
        float load_verilog(const std::string &filenames);
        float save_verilog(const std::string &filename);
        float load_genlib(const std::string &filename);
        double get_area();
        // float balance();
        // float rewrite();
        // float refactor();
        // float resub();

    private:
        mockturtle::mig_network _mig;
        mockturtle::map_stats _map_stats;
};

float Mockturtle_mig_api::load_verilog(const std::string &filename)
{
    auto beginClk = clock();
    char Command[1000];
    // read the file
    sprintf( Command, "%s", filename.c_str() );
    lorina::read_verilog(filename, mockturtle::verilog_reader( _mig ) );
    auto endClk = clock();
    auto elapsed = endClk - beginClk;
    return (float) elapsed/CLOCKS_PER_SEC;
}

float Mockturtle_mig_api::save_verilog(const std::string &filename)
{
    auto beginClk = clock();
    char Command[1000];
    // write the file
    sprintf( Command, "%s", filename.c_str() );
    mockturtle::write_verilog( _mig, Command );
    auto endClk = clock();
    auto elapsed = endClk - beginClk;
    return (float) elapsed/CLOCKS_PER_SEC;
}

float Mockturtle_mig_api::load_genlib(const std::string &filename)
{
    std::vector<mockturtle::gate> gates;
    lorina::read_genlib(filename, mockturtle::genlib_reader( gates ));
    mockturtle::tech_library tech_lib( gates );
    mockturtle::map_params ps;
    mockturtle::map_stats stats;
    mockturtle::map(_mig, tech_lib, ps, &stats);
    _map_stats = stats;
}

double Mockturtle_mig_api::get_area()
{
    return _map_stats.area;
}

namespace py = pybind11;

PYBIND11_MODULE(Mockturtle_api, handle){
    handle.doc() = "This is a Mockturtle Python API";

    py::class_<Mockturtle_mig_api>(handle, "Mockturtle_mig_api")
        .def(py::init<>())
        .def("load_genlib", &Mockturtle_mig_api::load_genlib)
        .def("get_area", &Mockturtle_mig_api::get_area)
        .def("load_verilog", &Mockturtle_mig_api::load_verilog)
        .def("save_verilog", &Mockturtle_mig_api::save_verilog);
}