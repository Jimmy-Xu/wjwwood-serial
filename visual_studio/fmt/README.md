fmtlib
===============================

# generate visual studio project

```
FMT_VER="3.0.1"

rm -rf source fmt-${FMT_VER}.zip

curl -L -O https://github.com/fmtlib/fmt/releases/download/${FMT_VER}/fmt-${FMT_VER}.zip
unzip fmt-${FMT_VER}.zip
mv fmt-${FMT_VER} source
cmake source

rm -rf fmt-${FMT_VER}.zip
```
