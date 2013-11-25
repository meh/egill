require 'rake'
require 'rake/clean'
require 'mkmf'

def executable(name)
	path = ENV['PATH'].split(?:).find {|p|
		File.executable? "#{p}/#{name}"
	}

	"#{path}/#{name}"
end

CC      = ENV['CC'] || 'clang'
AR      = ENV['AR'] || 'ar'
SCANNER = ENV['SCANNER'] || executable('wayland-scanner') || abort("could not find wayland-scanner")
CFLAGS  = "-Wall -Wextra -Iinclude -Iprotocol -std=c11 #{`pkg-config pixman-1 --cflags`.chomp} #{ENV['CFLAGS']}"
LDFLAGS = "-lwayland-server -lxkbcommon #{`pkg-config pixman-1 --libs`.chomp} #{ENV['LDFLAGS']}"

unless File.exists? '.config'
	result = ''

	if have_func 'mkostemp'
		result << " -DHAVE_MKOSTEMP"
	end

	File.open('.config', 'w') {|f|
		f.write result
	}
end

CFLAGS << " #{File.read('.config').chomp}"

SOURCES = FileList['source/*.c', 'source/data/*.c']

(ENV['backends'] || 'x11,drm').split(/\s*,\s*/).each {|backend|
	SOURCES.include("source/backend/#{backend}.c")
	CFLAGS << " -DUSE_BACKEND_#{backend.upcase}"

	case backend
	when 'x11'
		LDFLAGS << " -lxcb"

	when 'drm'
		LDFLAGS << " -ludev -ldrm -lgbm -mtdev"

	when 'fbdev'
		LDFLAGS << " -ludev -lmtdev"
	end
}

(ENV['renderers'] || 'pixman,gl').split(/\s*,\s*/).each {|renderer|
	SOURCES.include("source/renderer/#{renderer}.c")
	CFLAGS << " -DUSE_RENDERER_#{renderer.upcase}"

	case renderer
	when 'gl'
		LDFLAGS << " -lEGL -lGLESv2"
	end
}

if bin = executable('X')
	CFLAGS << " -DXWAYLAND_PATH='#{bin.inspect}'"
end

if ENV['type'] == 'debug'
	CFLAGS << " -g -O0"
else
	CFLAGS  << " -Os"
	LDFLAGS << " -Wl,-O1"
end

OBJECTS   = SOURCES.existing.ext('o')
PROTOCOLS = FileList['protocol/*.xml'].ext('.h')

CLEAN.include(OBJECTS.existing, PROTOCOLS.existing)
CLOBBER.include(FileList['.config', 'libegill.so', 'libegill-static.a'].existing)

task :default => ["libegill.so", "libegill-static.a"]

rule '.o' => '.c' do |t|
	sh "#{CC} #{CFLAGS} -fPIC -o #{t.name} -c #{t.source}"
end

rule '.h' => '.xml' do |t|
	sh "cat #{t.source} | #{SCANNER} server-header > #{t.name}"
end

file "libegill.so" => PROTOCOLS + OBJECTS do
	sh "#{CC} #{CFLAGS} #{OBJECTS} -shared -Wl,-soname,libegill.so -o libegill.so #{LDFLAGS}"
end

file "libegill-static.a" => PROTOCOLS + OBJECTS do
	sh "#{AR} rcs libegill-static.a #{OBJECTS}"
end

CLOBBER.include(FileList['test'].existing)

task :example => 'libegill.so' do
	sh "#{CC} #{CFLAGS} -L. -legill -Iinclude example/test.c -o test"
	sh "LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ./test"
end
