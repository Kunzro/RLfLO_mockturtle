from build import Mockturtle_api
import os

print(os.getpid())

mtl = Mockturtle_api.Mockturtle_mig_api()
verilog_file = os.path.abspath("../../circuits/sqrt.v")
print(verilog_file)
dur = mtl.load_verilog(verilog_file)
print(dur)