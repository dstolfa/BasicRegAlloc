FullOverlapSpillOne = executable('full-overlap-spill-one',
                                 ['full-overlap-spill-one.cpp',
                                  'OverlapPolicy.cpp'],
                                 include_directories : inc,
                                 cpp_args : ['-lstdc++fs', '-lstdc++'],
                                 link_with : BasicRegAlloc_lib)

test('full-overlap-spill-one', FullOverlapSpillOne)
