env = Environment()
env['CPPPATH'] = [ './inc',
                   './depends/tetra-meta/inc',
                   './depends/tetra-message/inc' ]

env['LIBPATH'] = [ './bin',
                   './depends/tetra-meta/bin',
                   './depends/tetra-message/bin' ]

env['LIBS'] = [ 'tetraMeta', 'tetraMessage' ]
env['CXX'] = 'clang++';
env['CXXFLAGS'] = [ '-std=c++11', '-ggdb' ]

buildLib = env.Library('./bin/tetraFramework', Glob('src/*/*/*.cpp'))

env['CPPPATH'] += ['./tst']
env['LIBS'] += [ 'tetraFramework' ]
buildTests = env.Program('./bin/catchTests.out',
                         Glob('tst/*.cpp') +
                         Glob('tst/test/*.cpp') +
                         Glob('tst/*/*/*.cpp'));
Depends(buildTests, buildLib)

runTests = Command( target = "runTests"
                  , source = "./bin/catchTests.out"
                  , action = [ "./bin/catchTests.out" ]
                  )

Depends(runTests, buildTests)
Default(runTests)
