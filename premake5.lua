flags("MultiProcessorCompile")

newoption({
	trigger = "openssl",
	value = "yes/no",
	description = "Use OpenSSL or not",
	allowed = {
		{"yes", "Use OpenSSL"},
		{"no", "Don't use OpenSSL"}
	},
	category = "IRCd",
	default = "no"
})

workspace("ircd2-mp")
        configurations({
                "Debug",
                "Release"
        })
        platforms({
                "Native",
                "Win32",
                "Win64"
        })
        defaultplatform("Native")

filter("platforms:Win32")
        system("windows")
        architecture("x86")
        gccprefix("i686-w64-mingw32-")

filter("platforms:Win64")
        system("windows")
        architecture("x86_64")
        gccprefix("x86_64-w64-mingw32-")

filter({})

project("Common")
	kind("StaticLib")
	filter({"options:openssl=yes"})
		defines({"HAVE_OPENSSL"})
		links({"ssl", "crypto"})
	filter({})
	files("src/common/*.c")
	includedirs({
		"include/common",
		"include/server",
		"include"
	})
	filter("configurations:Debug")
		defines({
			"DEBUG"
		})
		symbols("On")
	filter("configurations:Release")
		defines({
			"NDEBUG"
		})
		optimize("On")
	filter({})

project("Server")
	kind("ConsoleApp")
	targetname("ircd")
	filter({"options:openssl=yes"})
		defines({"HAVE_OPENSSL"})
		links({"ssl", "crypto"})
	filter({})
	files("src/server/*.c")
	removefiles({
		"src/server/config_read.c",
		"src/server/fileio.c",
	})
	includedirs({
		"include/common",
		"include/server",
		"include"
	})
	links("Common")
	filter("system:windows")
		links({
			"wsock32",
			"dnsapi"
		})
	filter({})
	filter("configurations:Debug")
		defines({
			"DEBUG"
		})
		symbols("On")
	filter("configurations:Release")
		defines({
			"NDEBUG"
		})
		optimize("On")
	filter({})

project("Service")
	kind("ConsoleApp")
	targetname("service")
	files("src/service/*.c")
	includedirs({
		"include/common",
		"include/service",
		"include"
	})
	links("Common")
	filter("system:windows")
		links({
			"wsock32"
		})
	filter({})
	filter("configurations:Debug")
		defines({
			"DEBUG"
		})
		symbols("On")
	filter("configurations:Release")
		defines({
			"NDEBUG"
		})
		optimize("On")
	filter({})
