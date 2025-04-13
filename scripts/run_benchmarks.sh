conan export . --version=0.1
cd benchmark

conan install . -s build_type=Release -s compiler.cppstd=gnu20 --build=missing

cmake --preset conan-release
cmake --build benchmark/build/Release -j 12 --config Release

./benchmark/build/Release/yabil_benchmarks --benchmark_repetitions=12 --benchmark_report_aggregates_only=true --benchmark_out=linux_x64_benchmarks.json
