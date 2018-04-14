project('BasicRegAlloc', 'cpp',
        version : '0.1.0',
        license : 'BSD 2-clause',
        default_options : ['cpp_std=c++17'])

add_global_arguments(language : 'cpp')
inc = include_directories('.')

mk_tests = get_option('build_tests')

subdir('lib')
subdir('examples')

if mk_tests == true
  subdir('tests')
endif

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : BasicRegAlloc_lib,
                 version : '0.1.0',
                 name : 'libBasicRegAlloc',
                 filebase : 'BasicRegAlloc',
                 description : 'A library for basic register allocation')
