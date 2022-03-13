bsub -q q_sw_share -I -b -cgsp 64 -n 2 -host_stack 128 -share_size 2048 -o ./bandwidth.out ./bandwidth
