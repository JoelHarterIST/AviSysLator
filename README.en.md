# AviSysLator

[日本語](README.md) | [English](README.en.md)

## Initial Repo Setup

Make sure clone the submodules along with the repo.
~~~
git clone --recurse-submodules https://github.com/JoelHarterIST/AviSysLator.git
~~~
Submodules may need to be updated periodically.
~~~
git submodule update --remote --init --recursive
~~~
Enable useful scripts to compile and run the code for you.
~~~
cd ./
for file in scripts/*.sh; do
    if [ -f "$file" ]; then
        chmod +x "$file"
    fi
done
cd lib/CMatrixMath
for file in scripts/*.sh; do
    if [ -f "$file" ]; then
        chmod +x "$file"
    fi
done
cd ./
~~~

## Usage

### Simple

~~~shell
scripts/build_run.sh
~~~

### Advanced

#### Compiling

~~~shell
make
~~~

**After compiling, ``create_symbolmap_yaml.py`` will be exececuted and ``symbol_map.yaml`` will be automatically generated.*

#### Setup (optional)

Edit `` test_case.yaml ``

### Running

~~~shell
./avsl test_case.yaml
~~~

## Analysis

### Graphing in Python
Graph `` time_seires_log_output.csv `` using `` analysis/plot.ipynb ``.

### Additional Visualization in Julia

#### Setup

Install Julia
~~~
curl -fsSL https://install.julialang.org | sh
~~~
Install Packages
~~~
import Pkg; Pkg.add("Plots"); Pkg.add("CSV"); Pkg.add("DataFrames"); Pkg.add(GLMakie)
~~~

#### Run

Run `` analysis/plot3h.jl `` for additional visualization of results.