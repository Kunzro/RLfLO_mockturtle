#include "pybind11/pybind11.h"

#include "mockturtle/mockturtle.hpp"
#include <lorina/aiger.hpp>

class Mockturtle_mig_api{
    public:
        Mockturtle_mig_api(float a){turtle = a;}
        float get_turtle(){
            return turtle;
        }
    private:
        float turtle;
};


namespace py = pybind11;

PYBIND11_MODULE(Mockturtle_api, handle){
    handle.doc() = "The is a Mockturtle Python API";

    py::class_<Mockturtle_mig_api>(handle, "Mockturtle_mig_api")
        .def(py::init<float>())
        .def("get_turtle", &Mockturtle_mig_api::get_turtle);
}