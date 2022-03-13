## StencilDSL vs Patus on x86

## time of each step for 100 steps
|                       | Patus(ms) | StencilDSL(ms) |
| :-------------------- | --------- | -------------- |
| 2d9pt_star            | 37.62936  | 5.67           |
| 2d9pt_box             | 33.44078  | 5.70           |
| 3d7pt_star            | 19.38934  | 4.95           |
| 3d13pt_star_helmholtz | 17.47330  | 5.04           |
| 3d27pt_box            | 18.90152  | 10.40          |

### config
|                       | shape       | tile      | reorder             | parallel |
| :-------------------- | ----------- | :-------- | ------------------- | -------- |
| 2d9pt_star            | 4096x4096   | 2, 2048   | (xo,yo,xi,yi)       | 28       |
| 2d9pt_box             | 4096x4096   | 2, 2048   | (xo,yo,xi,yi)       | 28       |
| 3d7pt_star            | 256x256x256 | 2, 8, 256 | (xo,yo,zo,xi,yi,zi) | 28       |
| 3d13pt_star_helmholtz | 256x256x256 | 2, 8, 256 | (xo,yo,zo,xi,yi,zi) | 28       |
| 3d27pt_box            | 256x256x256 | 2, 8, 256 | (xo,yo,zo,xi,yi,zi) | 28       |



## StencilDSL weak scalability on Feiteng

## time of each step for 100 step

| stencil\num_mpi_processes | 32(ms) | 64(ms) | 128(ms) | 256(ms) |
| ------------------------- | ------ | ------ | ------- | ------- |
| 2d9pt_star                | 92.29  | 84.00  | 97.15   | 95.71   |
| 2d9pt_box                 | 63.50  | 64.75  | 72.43   | 73.62   |
| 3d7pt_star                | 109.10 | 109.50 | 118.54  | 124.29  |
| 3d13pt_star_helmholtz     | 155.96 | 154.21 | 166.36  | 168.22  |

## the config used in weak scalability test is the same as experiment of comparing to Patus



## StencilDSL strong scalability on Feiteng

### time of each step for 100 step

| stencil\num_mpi_processes | 32(ms) | 64(ms) | 128(ms) | 256(ms) |
| ------------------------- | ------ | ------ | ------- | ------- |
| 2d9pt_star                | 71.84  | 48.66  | 21.80   | 12.61   |
| 2d9pt_box                 | 58.63  | 34.17  | 18.42   | 10.06   |
| 3d7pt_star                | 120.34 | 59.95  | 30.06   | 16.92   |
| 3d13pt_star_helmholtz     | 158.04 | 85.14  | 48.11   | 25.56   |



## StencilDSL weak scalability on Sunway

## time of each step for 100 step

| stencil\num_mpi_processes | 128(ms) | 256(ms) | 512(ms) | 1024(ms) |
| ------------------------- | ------- | ------- | ------- | -------- |
| 2d9pt_star                | 14.51   | 14.66   | 14.92   | 15.15    |
| 2d9pt_box                 | 13.56   | 13.70   | 13.88   | 14.27    |
| 3d7pt_star                | 18.58   | 18.91   | 19.09   | 19.40    |
| 3d13pt_star_helmholtz     | 22.95   | 23.25   | 23.57   | 24.21    |



## StencilDSL strong scalability on Sunway

## time of each step for 100 step

| stencil\num_mpi_processes | 128(ms) | 256(ms) | 512(ms) | 1024(ms) |
| ------------------------- | ------- | ------- | ------- | -------- |
| 2d9pt_star                | 28.71   | 14.89   | 8.03    | 4.91     |
| 2d9pt_box                 | 26.74   | 13.71   | 7.62    | 4.56     |
| 3d7pt_star                | 36.40   | 20.74   | 14.73   | 7.94     |
| 3d13pt_star_helmholtz     | 45.26   | 27.06   | 19.05   | 10.11    |



## StencilDSL vs Physis on x86

## time of each step for 100 steps

| stencil\num_mpi_processes | physis 28mpi(ms) | dsl 28mpi(ms) | dsl 14mpi 2omp(ms) | dsl 7mpi 4omp(ms) |
| ------------------------- | ---------------- | ------------- | ------------------ | ----------------- |
| 2d9pt_star                | 1062.56638       | 398.91        | 399.17             | 431.47            |
| 2d9pt_box                 | 848.99022        | 396.03        | 397.82             | 407.37            |
| 3d7pt_star                | 310.35166        | 423.67        | 432.49             | 450.39            |
| 3d13pt_star_helmholtz     | 543.63219        | 502.45        | 500.09             | 524.34            |
| 3d27pt_box                | 1112.86033       |               |                    |                   |

## config

| physis | shape_sub_grid | shape_grid | num_mpi |
| :----- | -------------- | :--------- | ------- |
| 2D     | 4096x4096      | 4x7        | 28      |
| 2D     | 8192x4096      | 2x7        | 14      |
| 2D     | 16384x4096     | 1x7        | 7       |
| 3D     | 256x256x256    | 2x2x7      | 28      |
| 3D     | 512x256x256    | 1x2x7      | 14      |
| 3D     | 512x512x256    | 1x1x7      | 7       |

## 实验注意事项

1. 单个优化性能提升图中提供Gflops
2. 单双精度对比



## StencilDSL vs OpenACC and manual_code on sunway

## time of each step for 100 steps f64/f32

| stencil(f64/f32) | OpenACC(f64) | StencilDSL(f64) | Manual_Code(f64) | OpenACC(f32) | StencilDSL(f32) | Manual_Code(f32) |
| ---------------- | ------------ | --------------- | ---------------- | ------------ | --------------- | ---------------- |
| 2d9pt_star       | 2.72         | 2.37            | 1.66             | 1.91         | 1.25            | 1.09             |
| 2d9pt_box        | 2.39         | 1.64            | 1.29             | 1.48         | 1.01            | 1.02             |
| 3d7pt_star       | 2.53         | 1.77            | 1.30             | 1.47         | 0.99            | 0.92             |
| 3d13pt_star      | 3.81         | 2.71            | 1.89             | 2.10         | 1.44            | 1.12             |

注：由于OpenACC专注于单节点异构处理器上的程序性能优化，所以我们在单个CG上和openacc比较性能。



## StencilDSL vs manual_code on feiteng

## time of each step for 100 steps f64/f32 original

| stencil(f64/f32) | StencilDSL(f64) | Manual_Code(f64) | StencilDSL(f32) | Manual_Code(f32) |
| ---------------- | --------------- | ---------------- | --------------- | ---------------- |
| 2d9pt_star       | 6.66            | 5.62             | 7.00            | 6.09             |
| 2d9pt_box        | 7.16            | 6.32             | 6.73            | 6.59             |
| 3d7pt_star       | 7.87            | 5.96             | 6.41            | 5.60             |
| 3d13pt_star      | 10.04           | 7.87             | 7.45            | 5.41             |

## time of each step for 100 steps f64/f32

| stencil(f64/f32) | StencilDSL(f64) | Manual_Code(f64) | StencilDSL(f32) | Manual_Code(f32) |
| ---------------- | --------------- | ---------------- | --------------- | ---------------- |
| 2d9pt_star       | 63.18           | 55.19            | 33.53           | 29.16            |
| 2d9pt_box        | 43.02           | 41.17            | 24.76           | 24.01            |
| 3d7pt_star       | 51.59           | 43.42            | 36.64           | 32.34            |
| 3d13pt_star      | 102.79          | 80.39            | 53.76           | 42.96            |



## code length of openacc dsl and manual code on sunway

| stencil     | OpenACC(line) | StencilDSL(line) | Manual_Code(line) |
| ----------- | ------------- | ---------------- | ----------------- |
| 2d9pt_star  | 26            | 35               | 179               |
| 2d9pt_box   | 27            | 33               | 170               |
| 3d7pt_star  | 29            | 32               | 180               |
| 3d13pt_star | 35            | 29               | 177               |



## code length of dsl and manual code on feiteng

| stencil     | StencilDSL(line) | Manual_Code(line) |
| ----------- | ---------------- | ----------------- |
| 2d9pt_star  | 27               | 88                |
| 2d9pt_box   | 27               | 94                |
| 3d7pt_star  | 26               | 98                |
| 3d13pt_star | 23               | 95                |



### Branch miss of dsl on x86

| stencil     | dsl_vs_patus | dsl_vs_physis |
| ----------- | ------------ | ------------- |
| 2d9pt_star  | 0.08         | 0.33          |
| 2d9pt_box   | 0.08         | 0.33          |
| 3d7pt_star  | 0.52         | 0.50          |
| 3d13pt_star | 0.31         | 0.46          |

### overhead of compiler on feiteng

| stencil     | Frontend+Schedule+AST_Building | CodeGen |
| ----------- | ------------------------------ | ------- |
| 2d9pt_star  | 3.00435                        | 9.71985 |
| 2d9pt_box   | 3.05792                        | 9.74298 |
| 3d7pt_star  | 3.254                          | 11.4691 |
| 3d13pt_star | 3.56201                        | 9.68909 |

### instruction per cycle and cpu utilization on x86 dsl vs patus

| stencil/insn/cpuutil | Patus(insn) | StencilDSL(insn) | Patus(cpus) | StencilDSL(cpus) |
| -------------------- | ----------- | ---------------- | ----------- | ---------------- |
| 2d9pt_star           | 0.28        | 0.54             | 14.883      | 27.468           |
| 2d9pt_box            | 0.31        | 0.57             | 16.753      | 23.578           |
| 3d7pt_star           | 0.26        | 0.53             | 17.014      | 23.255           |
| 3d13pt_star          | 0.36        | 0.99             | 14.530      | 23.506           |

### Cache_misses of patus and dsl on x86

| stencil     |                       | Patus  | DSL    |
| ----------- | --------------------- | ------ | ------ |
| 2d9pt_star  | L1-dcache-load-misses | 18.29% | 16.14% |
|             | LLC-load-misses       | 0.00%  | 0.00%  |
|             | dTLB-load-misses      | 0.00%  | 0.00%  |
| 2d9pt_box   | L1-dcache-load-misses | 10.78% | 11.06% |
|             | LLC-load-misses       | 0.00%  | 0.00%  |
|             | dTLB-load-misses      | 0.00%  | 0.00%  |
| 3d7pt_star  | L1-dcache-load-misses | 13.26% | 13.44% |
|             | LLC-load-misses       | 0.00%  | 0.00%  |
|             | dTLB-load-misses      | 0.00%  | 0.00%  |
| 3d13pt_star | L1-dcache-load-misses | 12.72% | 6.28%  |
|             | LLC-load-misses       | 0.00%  | 0.00%  |
|             | dTLB-load-misses      | 0.00%  | 0.00%  |

