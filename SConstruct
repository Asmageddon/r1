env = Environment()
if env.WhereIs("ccache"):
    env['CXX'] = 'ccache g++'
else:
    env['CXX'] = 'g++'

VariantDir('build', 'src', duplicate=0)

env.Append(LIBS = ["sfml-graphics", "sfml-window", "sfml-system"] )
env.Append(LIBS = ["boost_system", "boost_filesystem"] )
env.Append(LIBS = ["noise"] )

env.Append(CXXFLAGS =[
    "-Wall",
    "-pedantic",
    "-g"
])

files = Glob("./build/*.cpp")
files+= Glob("./build/Actions/*.cpp")
files+= Glob("./build/Resources/*.cpp")

env.Program("r1", files)

# EOF #
