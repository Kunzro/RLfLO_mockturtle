from build import Mockturtle_api
import os

print(os.getpid())

mtl = Mockturtle_api.Mockturtle_mig_api()
verilog_file = os.path.abspath("../../circuits/adder.v")
print(verilog_file)
dur = mtl.load_verilog(verilog_file)
print(dur)

print("balancing")
# balance: critical, cut_size: 4
print(mtl.balance(False, 4))
print(mtl.balance(True, 4))

print("rewriting")
# rewrite(bool allow_zero_gain, bool use_dont_cares, bool preserve_depth, uint32_t min_cut_size: 3)
print(mtl.rewrite(False, False, False, 3))
print(mtl.rewrite(False, False, True, 3))
print(mtl.rewrite(False, True, False, 3))
print(mtl.rewrite(False, True, True, 3))
print(mtl.rewrite(True, False, False, 3))
print(mtl.rewrite(True, False, True, 3))
print(mtl.rewrite(True, True, False, 3))
print(mtl.rewrite(True, True, True, 3))

print("refactoring")
# refactor(bool allow_zero_gain, bool use_dont_cares)
print(mtl.refactor(False, False))
print(mtl.refactor(False, True))
print(mtl.refactor(True, False))
print(mtl.refactor(True, True))

print("resubing")
# float resub(uint32_t max_pis: 8, uint32_t max_inserts: 2, bool use_dont_cares, uint32_t window_size: 12, bool preserve_depth);
print(mtl.resub(8, 2, True, 12, False))
print(mtl.resub(8, 2, True, 12, True))
print(mtl.resub(8, 2, False, 12, False))
print(mtl.resub(8, 2, False, 12, True))