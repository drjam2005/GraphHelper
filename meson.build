project(
  'Graph',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=0', 'cpp_std=c++17'],
)
add_project_arguments('-g', language: 'cpp')

raylib_dep = dependency('raylib', required: true)
dependencies = [
  raylib_dep
]

src = [
  'src/main.cpp',
  'src/renderer.cpp',
  'src/functions.cpp'
]

includeDirs = include_directories('include')

exe = executable(
  'Graph',
  src,
  install : true,
  dependencies : dependencies,
  include_directories : includeDirs
)

test('basic', exe)
