env = Environment();
env.Append(LIBS = [
	"SDL2",
	"SDL2_image"
])
env.Append(CCFLAGS = ['-O3'])

VariantDir("build", "src", duplicate=0)
apteryx = env.Program("apteryx", Glob("build/*.cpp"))

if "debian" in COMMAND_LINE_TARGETS:
	SConscript("deb/SConscript")
