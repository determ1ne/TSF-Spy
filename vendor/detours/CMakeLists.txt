add_library(detours STATIC
  src/detours.cpp
  src/modules.cpp
  src/disasm.cpp
  src/image.cpp
  src/creatwth.cpp
  src/disolx86.cpp
  src/disolx64.cpp
  src/disolarm.cpp
  src/disolarm64.cpp
)

target_compile_options(detours PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od /DWIN32_LEAN_AND_MEAN /D_WIN32_WINNT=0x501)
target_include_directories(detours PUBLIC src)
