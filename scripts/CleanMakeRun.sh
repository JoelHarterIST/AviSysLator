cd "$(dirname "$0")"/..
make clean
make
./avsl test_case.yaml