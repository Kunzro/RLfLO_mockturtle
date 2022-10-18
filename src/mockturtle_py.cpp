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
        // float balance();
        // float rewrite();
        // float refactor();
        // float resub();

    private:
        mockturtle::mig_network _mig;
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

namespace py = pybind11;

PYBIND11_MODULE(Mockturtle_api, handle){
    handle.doc() = "This is a Mockturtle Python API";

    py::class_<Mockturtle_mig_api>(handle, "Mockturtle_mig_api")
        .def(py::init<>())
        .def("load_verilog", &Mockturtle_mig_api::load_verilog)
        .def("save_verilog", &Mockturtle_mig_api::save_verilog);
}