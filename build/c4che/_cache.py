APPNAME = 'ns'
AR = '/usr/bin/ar'
ARFLAGS = 'rcs'
BINDIR = '/usr/local/bin'
BUILD_PROFILE = 'debug'
BUILD_SUFFIX = '-debug'
CC = ['/usr/bin/gcc']
CCDEFINES = ['_DEBUG']
CCFLAGS = ['-O0', '-ggdb', '-g3', '-O0', '-ggdb', '-g3', '-Wno-error=deprecated-declarations', '-fstrict-aliasing', '-Wstrict-aliasing']
CCFLAGS_PTHREAD = '-pthread'
CCFLAGS_PYEXT = ['-fvisibility=hidden']
CCLNK_SRC_F = []
CCLNK_TGT_F = ['-o']
CC_NAME = 'gcc'
CC_SRC_F = []
CC_TGT_F = ['-c', '-o']
CC_VERSION = ('9', '3', '0')
CFLAGS_MACBUNDLE = ['-fPIC']
CFLAGS_PYEMBED = ['-fwrapv', '-fwrapv', '-fstack-protector-strong', '-fwrapv', '-fno-strict-aliasing']
CFLAGS_PYEXT = ['-fwrapv', '-pthread', '-fwrapv', '-fwrapv', '-fstack-protector-strong', '-fwrapv', '-fno-strict-aliasing']
CFLAGS_cshlib = ['-fPIC']
COMPILER_CC = 'gcc'
COMPILER_CXX = 'g++'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/g++']
CXXDEFINES = ['_DEBUG']
CXXFLAGS = ['-O0', '-ggdb', '-g3', '-Wno-error=deprecated-declarations', '-fstrict-aliasing', '-Wstrict-aliasing']
CXXFLAGS_MACBUNDLE = ['-fPIC']
CXXFLAGS_PTHREAD = '-pthread'
CXXFLAGS_PYEMBED = ['-fwrapv', '-fwrapv', '-fstack-protector-strong', '-fwrapv', '-fno-strict-aliasing']
CXXFLAGS_PYEXT = ['-fwrapv', '-pthread', '-fwrapv', '-fwrapv', '-fstack-protector-strong', '-fwrapv', '-fno-strict-aliasing', '-fvisibility=hidden', '-Wno-array-bounds']
CXXFLAGS_cxxshlib = ['-fPIC']
CXXLNK_SRC_F = []
CXXLNK_TGT_F = ['-o']
CXX_NAME = 'gcc'
CXX_SRC_F = []
CXX_TGT_F = ['-c', '-o']
DATADIR = '/usr/local/share'
DATAROOTDIR = '/usr/local/share'
DEFINES = ['NS3_ASSERT_ENABLE', 'NS3_LOG_ENABLE', 'HAVE_SYS_IOCTL_H=1', 'HAVE_IF_NETS_H=1', 'HAVE_NET_ETHERNET_H=1', 'HAVE_PACKET_H=1', 'HAVE_SQLITE3=1', 'HAVE_IF_TUN_H=1', 'HAVE_GSL=1']
DEFINES_PYEMBED = ['NDEBUG']
DEFINES_PYEXT = ['NDEBUG']
DEFINES_ST = '-D%s'
DEST_BINFMT = 'elf'
DEST_CPU = 'x86_64'
DEST_OS = 'linux'
DOCDIR = '/usr/local/share/doc/ns'
DOXYGEN = '/usr/bin/doxygen'
DVIDIR = '/usr/local/share/doc/ns'
ENABLE_BRITE = False
ENABLE_EMU = True
ENABLE_EXAMPLES = False
ENABLE_FDNETDEV = True
ENABLE_GSL = '-lgsl -lgslcblas -lm\n'
ENABLE_GTK2 = None
ENABLE_LIBXML2 = '-I/usr/include/libxml2 -lxml2\n'
ENABLE_NSC = False
ENABLE_PYTHON_BINDINGS = False
ENABLE_PYVIZ = False
ENABLE_REAL_TIME = True
ENABLE_STATIC_NS3 = False
ENABLE_SUDO = False
ENABLE_TAP = True
ENABLE_TESTS = False
ENABLE_THREADING = True
EXAMPLE_DIRECTORIES = ['wireless', 'udp-client-server', 'stats', 'error-model', 'ipv6', 'matrix-topology', 'routing', 'energy', 'naming', 'tutorial', 'tcp', 'socket', 'udp', 'realtime']
EXEC_PREFIX = '/usr/local'
HTMLDIR = '/usr/local/share/doc/ns'
INCLUDEDIR = '/usr/local/include'
INCLUDES_LIBXML2 = ['/usr/include/libxml2']
INCLUDES_PYEMBED = ['/usr/include/python3.8']
INCLUDES_PYEXT = ['/usr/include/python3.8']
INFODIR = '/usr/local/share/info'
INT64X64_USE_128 = 1
LIBDIR = '/usr/local/lib'
LIBEXECDIR = '/usr/local/libexec'
LIBPATH_PYEMBED = ['/usr/lib']
LIBPATH_PYEXT = ['/usr/lib']
LIBPATH_PYTHON3.8 = ['/usr/lib']
LIBPATH_ST = '-L%s'
LIB_BOOST = []
LIB_GSL = ['gsl', 'gslcblas', 'm']
LIB_LIBXML2 = ['xml2']
LIB_PYEMBED = ['python3.8']
LIB_PYEXT = ['python3.8']
LIB_PYTHON3.8 = ['python3.8']
LIB_RT = ['rt']
LIB_SQLITE3 = ['sqlite3']
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_PTHREAD = '-pthread'
LINKFLAGS_PYEMBED = ['-Wl,-Bsymbolic-functions', '-Wl,-z,relro']
LINKFLAGS_PYEXT = ['-Wl,-Bsymbolic-functions', '-Wl,-z,relro', '-pthread', '-Wl,-O1', '-Wl,-Bsymbolic-functions', '-Wl,-Bsymbolic-functions', '-Wl,-z,relro']
LINKFLAGS_cshlib = ['-shared']
LINKFLAGS_cstlib = ['-Wl,-Bstatic']
LINKFLAGS_cxxshlib = ['-shared']
LINKFLAGS_cxxstlib = ['-Wl,-Bstatic']
LINK_CC = ['/usr/bin/gcc']
LINK_CXX = ['/usr/bin/g++']
LOCALEDIR = '/usr/local/share/locale'
LOCALSTATEDIR = '/usr/local/var'
MANDIR = '/usr/local/share/man'
MODULES_NOT_BUILT = ['brite', 'click', 'openflow', 'visualizer']
NS3_ENABLED_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-fd-net-device', 'ns3-flow-monitor', 'ns3-internet', 'ns3-lr-wpan', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-mpi', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-sixlowpan', 'ns3-spectrum', 'ns3-stats', 'ns3-tap-bridge', 'ns3-test', 'ns3-topology-read', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wave', 'ns3-wifi', 'ns3-wimax']
NS3_EXECUTABLE_PATH = ['/home/tsesterh/Dokumente/ns-3-gsoc/build/src/fd-net-device', '/home/tsesterh/Dokumente/ns-3-gsoc/build/src/tap-bridge']
NS3_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-fd-net-device', 'ns3-flow-monitor', 'ns3-internet', 'ns3-lr-wpan', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-mpi', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-sixlowpan', 'ns3-spectrum', 'ns3-stats', 'ns3-tap-bridge', 'ns3-test', 'ns3-topology-read', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wave', 'ns3-wifi', 'ns3-wimax']
NS3_MODULE_PATH = ['/usr/lib/gcc/x86_64-linux-gnu/9', '/home/tsesterh/Dokumente/ns-3-gsoc/build']
NS3_OPTIONAL_FEATURES = [('python', 'Python Bindings', False, 'PyBindGen missing'), ('brite', 'BRITE Integration', False, 'BRITE not enabled (see option --with-brite)'), ('nsclick', 'NS-3 Click Integration', False, 'nsclick not enabled (see option --with-nsclick)'), ('GtkConfigStore', 'GtkConfigStore', [], "library 'gtk+-2.0 >= 2.12' not found"), ('XmlIo', 'XmlIo', '-I/usr/include/libxml2 -lxml2\n', "library 'libxml-2.0 >= 2.7' not found"), ('Threading', 'Threading Primitives', True, '<pthread.h> include not detected'), ('RealTime', 'Real Time Simulator', True, 'threading not enabled'), ('FdNetDevice', 'File descriptor NetDevice', True, 'FdNetDevice module enabled'), ('TapFdNetDevice', 'Tap FdNetDevice', True, 'Tap support enabled'), ('EmuFdNetDevice', 'Emulation FdNetDevice', True, 'Emulation support enabled'), ('PlanetLabFdNetDevice', 'PlanetLab FdNetDevice', False, 'PlanetLab operating system not detected (see option --force-planetlab)'), ('nsc', 'Network Simulation Cradle', False, 'NSC not found (see option --with-nsc)'), ('mpi', 'MPI Support', False, 'option --enable-mpi not selected'), ('openflow', 'NS-3 OpenFlow Integration', False, 'Required boost libraries not found'), ('SqliteDataOutput', 'SQlite stats data output', '-lsqlite3\n', "library 'sqlite3' not found"), ('TapBridge', 'Tap Bridge', True, '<linux/if_tun.h> include not detected'), ('PyViz', 'PyViz visualizer', False, 'Python Bindings are needed but not enabled'), ('ENABLE_SUDO', 'Use sudo to set suid bit', False, 'option --enable-sudo not selected'), ('ENABLE_TESTS', 'Build tests', False, 'defaults to disabled'), ('ENABLE_EXAMPLES', 'Build examples', False, 'defaults to disabled'), ('GSL', 'GNU Scientific Library (GSL)', '-lgsl -lgslcblas -lm\n', 'GSL not found')]
OLDINCLUDEDIR = '/usr/include'
PACKAGE = 'ns'
PDFDIR = '/usr/local/share/doc/ns'
PKGCONFIG = '/usr/bin/pkg-config'
PLATFORM = 'linux2'
PREFIX = '/usr/local'
PRINT_BUILT_MODULES_AT_END = False
PSDIR = '/usr/local/share/doc/ns'
PYC = 1
PYCMD = '"import sys, py_compile;py_compile.compile(sys.argv[1], sys.argv[2])"'
PYFLAGS = ''
PYFLAGS_OPT = '-O'
PYO = 1
PYTHON = ['/usr/bin/python']
PYTHONARCHDIR = '/usr/local/lib/python3/dist-packages'
PYTHONDIR = '/usr/local/lib/python3/dist-packages'
PYTHON_CONFIG = '/usr/bin/python3.8-config'
PYTHON_VERSION = '3.8'
REQUIRED_BOOST_LIBS = ['system', 'signals', 'filesystem']
RPATH_ST = '-Wl,-rpath,%s'
SBINDIR = '/usr/local/sbin'
SHAREDSTATEDIR = '/usr/local/com'
SHLIB_MARKER = '-Wl,-Bdynamic'
SONAME_ST = '-Wl,-h,%s'
SQLITE_STATS = '-lsqlite3\n'
STLIBPATH_ST = '-L%s'
STLIB_MARKER = '-Wl,-Bstatic'
STLIB_ST = '-l%s'
SUDO = '/usr/bin/sudo'
SYSCONFDIR = '/usr/local/etc'
VALGRIND = '/usr/bin/valgrind'
VALGRIND_FOUND = True
VERSION = '3-dev'
WL_SONAME_SUPPORTED = True
cfg_files = ['/home/tsesterh/Dokumente/ns-3-gsoc/build/ns3/config-store-config.h', '/home/tsesterh/Dokumente/ns-3-gsoc/build/ns3/core-config.h']
cprogram_PATTERN = '%s'
cshlib_PATTERN = 'lib%s.so'
cstlib_PATTERN = 'lib%s.a'
cxxprogram_PATTERN = '%s'
cxxshlib_PATTERN = 'lib%s.so'
cxxstlib_PATTERN = 'lib%s.a'
define_key = ['HAVE_SYS_IOCTL_H', 'HAVE_IF_NETS_H', 'HAVE_NET_ETHERNET_H', 'HAVE_IF_TUN_H', 'HAVE_PACKET_H', 'HAVE_SQLITE3', 'HAVE_GSL']
macbundle_PATTERN = '%s.bundle'
pyext_PATTERN = '%s.cpython-38-x86_64-linux-gnu.so'
