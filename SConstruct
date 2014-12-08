import os;
import os.path;

def pullDependencies():
  init = "git submodule init"
  update = "git submodule update"
  foreach = "git submodule foreach "
  return Command( target = "pullDependencies"
                , source = ""
                , action = [ init, update,
                             foreach + init,
                             foreach + update ]
                );

def buildDependencies( libTarget ):
  pull = pullDependencies();

  dependencyTargets = [];
  dependsBaseDir = "./depends";
  dependsNames = [ d for d in os.listdir(dependsBaseDir) ];

  for dependsName in dependsNames:
    dependsDir = os.path.join(dependsBaseDir, dependsName);
    cmd = "cd " + dependsDir + "; scons";
    cmdTarget = Command( target = dependsName
                       , source = ""
                       , action = [ cmd ]
                       );
    Depends( cmdTarget, pull );
    Depends( libTarget, cmdTarget );

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

buildDependencies( buildLib );

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
