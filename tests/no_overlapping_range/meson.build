StdCXXFSDep = meson.get_compiler('cpp').find_library('stdc++fs')

FullOverlapSpillOne = executable('full-overlap-spill-one',
                                 ['full-overlap-spill-one.cpp',
                                  'OverlapPolicy.cpp'],
                                 include_directories : inc,
                                 dependencies : StdCXXFSDep,
                                 link_with : BasicRegAlloc_lib)

NoSpillBoundary = executable('no-spill-boundary',
                             ['no-spill-boundary.cpp',
                              'OverlapPolicy.cpp'],
                             include_directories : inc,
                             dependencies : StdCXXFSDep,
                             link_with : BasicRegAlloc_lib)

test('full-overlap-spill-one', FullOverlapSpillOne)
test('no-spill-boundary', NoSpillBoundary)
