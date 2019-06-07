# vfpbenchmark

Benchmark test for FPU and SIMD instructions.


### on Linux

```bash
$ git clone https://github.com/hiroog/FlatBuildTool
$ export PATH=$PATH:$PWD/FlatBuildTool
$ git clone https://github.com/hiroog/vfpbench
$ cd vfpbench
$ flmake
$ flmake run
$ more output_log.txt
```

### on Android + termux

```bash
$ pkg install python clang
$ git clone https://github.com/hiroog/FlatBuildTool
$ export PATH=$PATH:$PWD/FlatBuildTool
$ git clone https://github.com/hiroog/vfpbench
$ cd vfpbench
$ flmake termux
$ flmake run
$ more output_log.txt
```


### Web site

https://dench.flatlib.jp/app/vfpbench

