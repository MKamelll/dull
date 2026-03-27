project(
  'dull',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)

dependencies = [
  dependency('gtk4'),
  dependency('gtksourceview-5')
]

sources = [
  'dull.c',
]

exe = executable(
  'dull',
  sources,
  dependencies : dependencies,
  install : true,
)

test('basic', exe)
