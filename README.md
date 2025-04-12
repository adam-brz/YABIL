# Yet Another Big Integer Library (YABIL)

[![Test on Linux (gcc 12.2)](https://github.com/adam-brz/YABIL/actions/workflows/linux_gcc_test.yml/badge.svg?branch=master)](https://github.com/adam-brz/YABIL/actions/workflows/linux_gcc_test.yml)
[![Test on MacOS (apple-clang 14.0)](https://github.com/adam-brz/YABIL/actions/workflows/macos_clang_test.yml/badge.svg?branch=master)](https://github.com/adam-brz/YABIL/actions/workflows/macos_clang_test.yml)
[![Test on Windows (MSVC 17)](https://github.com/adam-brz/YABIL/actions/workflows/windows_mcvc_test.yml/badge.svg?branch=master)](https://github.com/adam-brz/YABIL/actions/workflows/windows_mcvc_test.yml)
[![Build for Android (NDK r25c, clang 14.0.7)](https://github.com/adam-brz/YABIL/actions/workflows/linux_android_test.yml/badge.svg)](https://github.com/adam-brz/YABIL/actions/workflows/linux_android_test.yml)
[![Code Analysis](https://github.com/adam-brz/YABIL/actions/workflows/code_analysis.yml/badge.svg?branch=master)](https://github.com/adam-brz/YABIL/actions/workflows/code_analysis.yml)
[![Coverage](https://adam-brz.github.io/YABIL/coverage_report/badge.svg)](https://adam-brz.github.io/YABIL/coverage_report)
[![License](https://raw.githubusercontent.com/adam-brz/YABIL/master/docs/badges/license_badge.svg)](https://github.com/adam-brz/YABIL/blob/master/LICENSE.txt)
[![Documentation](https://raw.githubusercontent.com/adam-brz/YABIL/master/docs/badges/documentation_badge.svg)](https://adam-brz.github.io/YABIL)

## About

This is a simple and easy to use C++ library for arbitrary precision integer arithmetic.

It only requires c++20 standard and can be compiled on most of the widely used platforms.

Comparing to other popular libraries (like GMP or FLINT), this library performs slightly worse. You can see benchmarks here: (TODO add link to benchmarks).
However, it is much easier to use and understand. And very simple to extend and modify.

Use this library when you:

- Need an easy-to-use library for arbitrary precision integer arithmetic, with no dependencies beyond c++20.
- Want to use arbitrary precision arithmetic also in **compile-time** (from my knowledge, there is no other library supporting such feature).
- Need a library which can be easily extended or modified, to suit your needs.

Do not use this library when you:

- Need a library which is extremely fast and efficient.
- Need a library which is extremely feature rich and has a lot of advanced features.

> [!NOTE]  
> See [documentation](https://adam-brz.github.io/YABIL) for more details.

## Features

- **Core Capabilities**
  - Arbitrary precision integer arithmetic
  - Compile-time (`constexpr`) big integer support

- **Performance**
  - Parallelized algorithms (currently, addition and multiplication)
  - CUDA acceleration (experimental, optional)

- **Utilities**
  - Math functions (e.g., `pow`, `log`, `gcd`)
  - Random number generation

## Examples

`BigInt` creation and usage:

```.cpp
#include <yabil/bigint/BigInt.h>

const yabil::bigint::BigInt a{"12345678901234567890"}; // Create from string literal in base 10
const yabil::bigint::BigInt b{"10101111100010101", 2}; // Create from string literal in base 2
const yabil::bigint::BigInt c = 123; // Create from integer 
const yabil::bigint::BigInt d = 12345678901234567890_bi; // Create from compile-time big integer

assert(c.is<int>()); // Check conversion to simple type
assert(c.to<int>() == 123); // Convert to simple type
assert(a > b); // Comparison operators

auto d = (a + b) * c; // Arithmetic operators
d <<= 2; // Bitwise operators
```

Compile time operations:

```.cpp
#include <yabil/compile_time/Literals.h>

using namespace yabil::compile_time::literals;

constexpr auto a = (1210144124823_bi * 1283919111203_bi) + 123_bi;
constexpr auto b = a / 118273192381_bi;

static_assert(b < a);
static_assert(b.is<uint64_t>() && b.to<uint64_t>() == 13136765296443);
```

Math functions and random number generation:

```.cpp
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/io.h>

#include <yabil/compile_time/Literals.h>

#include <yabil/random/RandomEngine.h>
#include <yabil/math/Math.h>

#include <iostream>

using namespace yabil;
using namespace yabil::bigint;
using namespace yabil::compile_time::literals;

const BigInt a{"129012891241242430011215125293"};
const BigInt b = 128391203124124124823_bi;
std::cout << "a = " << a << std::endl;
std::cout << "b = " << b << std::endl;
std::cout << "a*b = " << a * b << std::endl;
std::cout << "a/b = " << a / b << std::endl;
std::cout << "pow(a,log2(b)) = " << math::pow(a, BigInt(math::log2_int(b))) << std::endl;
std::cout << "random_prime(32-bits) = " << random::RandomEngine{}.random_prime(32) << std::endl;
```

<details>
<summary>possible output</summary>

```
a = 129012891241242430011215125293
b = 128391203124124124823
a*b = 16564120324984891019987312216479904980773916448139
a/b = 1004842139
pow(a,log2(b)) = 20034805658418649842124435922210540541675425970722901642539673114408543190308111320208554900874656424543353227888758848254685044936216741779482546926477601418405692299109581740912175053879539877934744896824222816752717979108737738418374137092719333901549075936074254187314281569773926755277478644744371998922040273554215672569849448681508108094765754046510750872926033176012637964288996641729066020303768335027226523193496808670359367927336820331825689833845470048644933844568169317588477774409774454321273649270142144987668379658547122446431278209577689803917580331073311377524587517566313663650574494778198798197762195017510170511555474191803749374800758588394634048584894093028261022355363104337687722739189262672341621219206906380106750434251898667990646038382262718359477642802370150935042152575108345874822684024213870785018471180446970799071457680259822239311817721549870930560766255005143752953279541505240135403978780892661788641834294492179211249834250170140747745340818019034493301503050334097853208525308831983900018039221830922254051129985469743603595339063852715243436475935856344409406622727442414713334971940454304637196340662576488654034882180479345097112278627910842326930950917611249852291226108002958244594869760951497653444657356731492866677757309019865664183564761117958638124984023833312215625265686091061372533220688702605724438607997656288466328753323196657795756592521659609959480202830470857317639704706503968871577116374550804977713446535222468483794737200235424886677322950209420227457365944639960437533367276199376517571027975329410365654055761394958815265766224984487233207408303249593619012794979708300874282709208566990581650012368837227552911618142239694517696819459286684678331003552644367416971593032891331720706912851042811381815767348424440867132956379560167275865166551821191796991021042723572350357650279939021128048558693754148307624937912303368429214631898689450812023401253603049
random_prime(32-bits) = 2635075717
```

</details>

## Usage

### CMake

This project uses [CMake](https://cmake.org). To build the project, you need to have it installed on your system.

To build and install the project, you can use the following commands:

```bash
cmake -S . -B build
cmake --build build
cmake --install build --prefix <install_path>
```

Then you can include the library in your project by adding the following code to your CMakeLists.txt:

```cmake
# Add yabil installation path, this can be done in many ways
set(CMAKE_PREFIX_PATH <install_path>)

find_package(yabil REQUIRED)
target_link_libraries(<your_target> yabil::bigint)
```

### Conan

This project supports [Conan](https://conan.io) package manager. To use it, you need to have it installed on your system.

To use the library in your project, you can export YABIL `conanfile.py` file:

```bash
conan export . --version=0.1 # From the root of YABIL project directory
```

Then you can use exported recipe to build package which can be used in your project.

First, add the YABIL package to conanfile in your project (in this example `conanfile.txt`):

```
[requires]
yabil/[>=0.1.0]

[generators]
CMakeToolchain
CMakeDeps

[layout]
cmake_layout
```

Then call `conan install` command with `--build` option:

```bash
conan install . --build=missing # From the root of your project directory
```

Finally, you can include the library in your project by adding the following code to your CMakeLists.txt:

```cmake
find_package(yabil REQUIRED)
target_link_libraries(<your_target> yabil::bigint) # Link all libraries that you need, e.g. yabil::bigint, yabil::math, yabil::random
```

## License

This project is licensed under the terms of the [MIT license](LICENSE.txt).
