cmake_minimum_required(VERSION 3.20)
project(appsec_review_smoke_cpp LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(smoke_zlib STATIC
  third_party/zlib/adler32.c
  third_party/zlib/compress.c
  third_party/zlib/crc32.c
  third_party/zlib/deflate.c
  third_party/zlib/gzclose.c
  third_party/zlib/gzlib.c
  third_party/zlib/gzread.c
  third_party/zlib/gzwrite.c
  third_party/zlib/infback.c
  third_party/zlib/inffast.c
  third_party/zlib/inflate.c
  third_party/zlib/inftrees.c
  third_party/zlib/trees.c
  third_party/zlib/uncompr.c
  third_party/zlib/zutil.c
)
target_include_directories(smoke_zlib PUBLIC third_party/zlib)
target_compile_definitions(smoke_zlib PRIVATE HAVE_UNISTD_H=1 _LARGEFILE64_SOURCE=1)
target_compile_options(smoke_zlib PRIVATE -w)

add_library(smoke_bz2 STATIC
  third_party/bzip2/blocksort.c
  third_party/bzip2/huffman.c
  third_party/bzip2/crctable.c
  third_party/bzip2/randtable.c
  third_party/bzip2/compress.c
  third_party/bzip2/decompress.c
  third_party/bzip2/bzlib.c
)
target_include_directories(smoke_bz2 PUBLIC third_party/bzip2)
target_compile_options(smoke_bz2 PRIVATE -w)

add_executable(smoke_synthetic src/synthetic.cpp)

add_executable(smoke_server
  src/main.cpp
  src/request.cpp
)
target_link_libraries(smoke_server PRIVATE smoke_zlib smoke_bz2)
