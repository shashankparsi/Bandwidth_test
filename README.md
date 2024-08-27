# Bandwidth_test
HIP Bandwidth Test
This test will determine the bandwidth between CPU to GPU , GPU to CPU,CPU to CPU and GPU to GPU.
test are written using HIP language and ported to run on AMD GPU's.

compilation:
hipcc check_bandwidth.cpp -o check_bandwidth

execution:
./check_bandwidth

output on MI300A:
Number of GPU's in system are : 4
CPU to GPU Bandwidth is :51.775 GB/s
GPU to CPU Bandwidth is :50.468 GB/s
GPU to GPU Bandwidth is :891.386 GB/s
CPU to CPU Bandwidth is :47.119 GB/s

