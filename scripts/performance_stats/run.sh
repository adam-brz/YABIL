set -e

CC=clang CXX=clang++ conan create -pr ../../conan/profiles/clang -s build_type=Release ../../conan/recipe
CC=clang CXX=clang++ conan test -pr ../../conan/profiles/clang -s build_type=Release . yabil/0.1 --build=missing
