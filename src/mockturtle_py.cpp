#include <stdio.h>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
#include "mockturtle/mockturtle.hpp"
#include "mockturtle/algorithms/balancing.hpp"
#include "mockturtle/algorithms/balancing/sop_balancing.hpp"
#include "lorina/aiger.hpp"

namespace py = pybind11;

class Mockturtle_mig_api
{
public:
    explicit Mockturtle_mig_api()
    {
        mockturtle::mig_network mig;
        _mig = mig;
        return;
    };
    float load_verilog(const std::string &filenames);
    float save_verilog(const std::string &filename);
    float load_genlib(const std::string &filename);
    double get_area();
    double get_delay();
    double get_power();
    py::array_t<int32_t> get_edge_index();
    float map();
    float balance(bool crit, uint32_t cut_size);
    float rewrite(bool allow_zero_gain, bool use_dont_cares, bool preserve_depth, uint32_t min_cut_size);
    float refactor(bool allow_zero_gain, bool use_dont_cares);
    float resub(uint32_t max_pis, uint32_t max_inserts, bool use_dont_cares, uint32_t window_size, bool preserve_depth);

private:
    mockturtle::mig_network _mig;
    mockturtle::map_stats _map_stats;
    mockturtle::tech_library<4u, mockturtle::classification_type::np_configurations> *_lib;
};

py::array_t<int32_t> Mockturtle_mig_api::get_edge_index()
{
    _mig.foreach_gate( [&]( auto node ) {
        std::cout <<_mig.node_to_index(node) << "\n";
    } );
    py::array_t<int32_t> edge_index(1000000);
    return edge_index;
}

float Mockturtle_mig_api::load_verilog(const std::string &filename)
{
    auto beginClk = clock();
    char Command[1000];
    // read the file
    sprintf(Command, "%s", filename.c_str());
    lorina::read_verilog(filename, mockturtle::verilog_reader(_mig));
    auto endClk = clock();
    auto elapsed = endClk - beginClk;
    return (float)elapsed / CLOCKS_PER_SEC;
}

float Mockturtle_mig_api::save_verilog(const std::string &filename)
{
    auto beginClk = clock();
    char Command[1000];
    // write the file
    sprintf(Command, "%s", filename.c_str());
    mockturtle::write_verilog(_mig, Command);
    auto endClk = clock();
    auto elapsed = endClk - beginClk;
    return (float)elapsed / CLOCKS_PER_SEC;
}

float Mockturtle_mig_api::load_genlib(const std::string &filename)
{
    std::vector<mockturtle::gate> gates;
    lorina::read_genlib(filename, mockturtle::genlib_reader(gates));
    _lib = new mockturtle::tech_library(gates);
}

float Mockturtle_mig_api::map()
{
    mockturtle::map_params ps;
    mockturtle::map_stats stats;
    mockturtle::map(_mig, *_lib, ps, &stats);
    _map_stats = stats;
}

float Mockturtle_mig_api::balance(bool crit, uint32_t cut_size)
{
    mockturtle::sop_rebalancing<mockturtle::mig_network> sop_balancing;
    mockturtle::balancing_params ps;
    mockturtle::balancing_stats st;
    ps.cut_enumeration_ps.cut_size = cut_size;
    ps.only_on_critical_path = crit;
    _mig = mockturtle::balancing(_mig, {sop_balancing}, ps, &st);
    return mockturtle::to_seconds(st.time_total);
}

float Mockturtle_mig_api::rewrite(bool allow_zero_gain, bool use_dont_cares, bool preserve_depth, uint32_t min_cut_size)
{
    mockturtle::mig_npn_resynthesis resyn;
    mockturtle::cut_rewriting_params ps;
    mockturtle::cut_rewriting_stats st;
    ps.cut_enumeration_ps.cut_size = 4u;
    ps.min_cand_cut_size = min_cut_size;
    ps.allow_zero_gain = allow_zero_gain;
    ps.use_dont_cares = use_dont_cares;
    ps.preserve_depth = preserve_depth;
    _mig = mockturtle::cut_rewriting(_mig, resyn, ps, &st);
    _mig = mockturtle::cleanup_dangling(_mig);
    return mockturtle::to_seconds(st.time_total);
}

float Mockturtle_mig_api::refactor(bool allow_zero_gain, bool use_dont_cares)
{
    mockturtle::akers_resynthesis<mockturtle::mig_network> resyn;
    mockturtle::refactoring_params ps;
    mockturtle::refactoring_stats st;
    ps.allow_zero_gain = allow_zero_gain;
    ps.use_dont_cares = use_dont_cares;
    mockturtle::refactoring(_mig, resyn, ps, &st);
    _mig = mockturtle::cleanup_dangling(_mig);
    return mockturtle::to_seconds(st.time_total);
}

float Mockturtle_mig_api::resub(uint32_t max_pis, uint32_t max_inserts, bool use_dont_cares, uint32_t window_size, bool preserve_depth)
{
    mockturtle::resubstitution_params ps;
    mockturtle::resubstitution_stats st;
    ps.max_pis = max_pis;
    ps.max_inserts = max_inserts;
    ps.use_dont_cares = use_dont_cares;
    ps.window_size = window_size;
    ps.preserve_depth = preserve_depth;
    mockturtle::depth_view _depth_mig{_mig};
    mockturtle::fanout_view _fanout_mig{_depth_mig};
    mockturtle::mig_resubstitution(_fanout_mig, ps, &st);
    _mig = mockturtle::cleanup_dangling(_mig);
    return mockturtle::to_seconds(st.time_total);
}

double Mockturtle_mig_api::get_area()
{
    return _map_stats.area;
}

double Mockturtle_mig_api::get_delay()
{
    return _map_stats.delay;
}

double Mockturtle_mig_api::get_power()
{
    return _map_stats.power;
}

PYBIND11_MODULE(Mockturtle_api, handle)
{
    handle.doc() = "This is a Mockturtle Python API";

    py::class_<Mockturtle_mig_api>(handle, "Mockturtle_mig_api")
        .def(py::init<>())
        .def("load_genlib", &Mockturtle_mig_api::load_genlib)
        .def("get_area", &Mockturtle_mig_api::get_area)
        .def("get_delay", &Mockturtle_mig_api::get_delay)
        .def("get_power", &Mockturtle_mig_api::get_power)
        .def("load_verilog", &Mockturtle_mig_api::load_verilog)
        .def("save_verilog", &Mockturtle_mig_api::save_verilog)
        .def("balance", &Mockturtle_mig_api::balance)
        .def("rewrite", &Mockturtle_mig_api::rewrite)
        .def("refactor", &Mockturtle_mig_api::refactor)
        .def("resub", &Mockturtle_mig_api::resub);
}