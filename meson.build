project('canary', ['cpp'],
    default_options: ['cpp_std=c++20'],
    version: 'latest',
)

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Qml', 'Widgets', 'Svg'], required: true)

cppcan_subproject = subproject('cppcan')
libcan_dep = cppcan_subproject.get_variable('libcan_dep')

spdlog_subproject = cppcan_subproject.get_variable('spdlog_subproject')
spdlog_dep = spdlog_subproject.get_variable('spdlog_dep')

##########
# canary #
##########

canary_includes = [
    include_directories('include'),
]

canary_sources = [
    'source/CanBus.cpp',
    'source/CanDevice.cpp',
    'source/CanDatabase.cpp',
    'source/Component/CanSniffer.cpp',
    'source/Log.cpp',
    'source/Main.cpp',
    qt5.compile_moc(headers: 'include/CanBus.hpp'),
    qt5.compile_moc(headers: 'include/CanDevice.hpp'),
    qt5.compile_moc(headers: 'include/CanDatabase.hpp'),
    qt5.compile_moc(headers: 'include/Component/CanSniffer.hpp'),
    qt5.compile_resources(sources: ['resources.qrc']),
]

canary_deps = [
    qt5_dep,
    libcan_dep,
    spdlog_dep,
]

canary = executable('canary', canary_sources,
    include_directories: canary_includes,
    dependencies: canary_deps,
    # cpp_args: ['-frtti']
)

#################
# format target #
#################

format_include = '`find include -type f -iname \'*.hpp\'`'
format_source = '`find source -type f -iname \'*.cpp\'`'
format_tests = '`find tests -type f -iname \'*.cpp\'`'
format_files = '@0@ @1@ @2@'.format(format_include, format_source, format_tests)

run_target('format',
    command: [
        'sh', '-c',
        'cd @0@ && clang-format --verbose -i @1@'.format(
            meson.source_root(),
            format_files,
        )
    ],
)

###############
# tidy target #
###############

tidy_include = '`find include -type f -iname \'*.hpp\'`'
tidy_source = '`find source -type f -iname \'*.cpp\'`'
tidy_tests = '`find source -type f -iname \'*.cpp\'`'
tidy_files = '@0@ @1@ @2@'.format(tidy_include, tidy_source, tidy_tests)

run_target('tidy',
    command: [
        'sh', '-c',
        'cd @0@ && clang-tidy -p=@1@ @2@'.format(
            meson.source_root(),
            meson.current_build_dir(),
            tidy_files,
        )
    ],
)