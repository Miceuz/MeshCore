# LTO + tiny float lib replacements (tinymath.c) for 128k flash parts
Import("env")
env.Append(
  CCFLAGS=["-flto"],
  LINKFLAGS=["-flto", "-Os", "-Wl,--wrap=strtof,--wrap=atof,--wrap=pow,--wrap=powf"],
)
