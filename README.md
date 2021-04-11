| Task | Link |
| --- | ----------- |
| Triangle app | [Here](https://github.com/rbetik12/jetbrains-internship/tree/master/TriangleApp) |
| Dictionary search | [Here](https://github.com/rbetik12/jetbrains-internship/tree/master/DictionaryUI) |

# Tested on

- Visual Studio 2019 (16.8.4) with MSVC v142 (14.28)

# Dependencies

*lib* directory contains all the dependencies for Dictionary UI application, but for Triangle application you must install following packages for Visual Studio

- C++ ATL for v142 build tools
- Windows 10 SDK (10.0.18362.0)

It should work with future versions, but was tested on exactly that ones.


# How to build

First of all, you should clone repository with `--recursive` key.

```bash
git clone --recursive https://github.com/rbetik12/jetbrains-internship.git
```

After you've cloned the repo and checked for dependecies to be present on your machine, you should compile the applications.

Run
```bash
cmake .
```

That will generate necessary files for compiler. If we are taking about Windows, it will generate Visual Studio solution.
After that just run visual studio and compile application through it.
