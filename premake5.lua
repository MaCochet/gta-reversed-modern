--[[
    GTA_reversed Build Script
    Use 'premake5 --help' for help
--]]


--[[
    Options and Actions
--]]

newoption {
    trigger     = "outdir",
    value       = "path",
    description = "Output directory for the build files"
}
if not _OPTIONS["outdir"] then
    _OPTIONS["outdir"] = "Build"
end


--[[
    The Solution
--]]

solution "gta_reversed"

    configurations { "Release", "Debug" }

    location( _OPTIONS["outdir"] )
    targetprefix "" -- no 'lib' prefix on gcc
    targetdir "Bin"
    implibdir "Bin"
    
    configuration "Debug*"
        flags { symbols ("On") }
        buildoptions {"/MDd"}          
    configuration "Release*"
        defines { "NDEBUG" }
        flags { symbols ("On") }
        buildoptions {"/MD"}
        optimize "Full"
    configuration "vs*"
         linkoptions   { "/ignore:4099" }      
         buildoptions {"/EHsc"}

    flags {
        characterset ("MBCS"), --fix strings
        staticruntime("On"),
        "NoImportLib",
        rtti ("Off"),
        "NoBufferSecurityCheck"
    }
    defines { "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS"}

group "Dependencies"
    defines { "WIN32", "_WINDOWS" }

    project "ogg"
        vpaths {
            ["Headers/*"] = {"libs/ogg/**.h",},
            ["Sources/*"] = {"libs/ogg/**.c",},
            ["*"] = {"premake5.lua", "CMakeLists.txt"}
        }
        includedirs { "libs/vorbis/include", "libs/ogg/include", "libs/ogg/include" }
        language "C++"
        kind "StaticLib"
        targetname "ogg"       
        files {
            "libs/ogg/**.h",
            "libs/ogg/**.c"
        }

    project "vorbis"
        vpaths {
            ["Headers/*"] = {"libs/vorbis/**.h",},
            ["Sources/*"] = {"libs/vorbis/**.c",},
            ["*"] = {"premake5.lua", "CMakeLists.txt"}
        }
        includedirs { "libs/vorbis/include", "libs/ogg/include", "%{cfg.targetdir}" }
        language "C++"
        kind "StaticLib"
        targetname "vorbis"   

        local filePaths = {
            "backends.h", "bitrate.h", "codebook.h", "codec_internal.h", "envelope.h", "highlevel.h", "lookup.h", "lookup_data.h", "lpc.h", "lsp.h", "masking.h", "mdct.h", "misc.h", "os.h", "psy.h", "registry.h", "scales.h", "smallft.h", "window.h",
            "analysis.c", "bitrate.c", "block.c", "codebook.c", "envelope.c", "floor0.c", "floor1.c", "info.c", "lookup.c", "lpc.c", "lsp.c", "mapping0.c", "mdct.c", "psy.c", "registry.c", "res0.c", "sharedbook.c", "smallft.c", "synthesis.c", "vorbisenc.c", "window.c"
        }
        for i, fileName in pairs(filePaths) do 
            filePaths[i] = "libs/vorbis/lib/"..fileName
        end 
        files { "libs/vorbis/win32/vorbis.def", table.unpack(filePaths) }

    project "vorbisenc"
        vpaths {
            ["Sources/*"] = {"libs/vorbis/**.c",},
            ["*"] = {"premake5.lua", "CMakeLists.txt"}
        }
        includedirs { "libs/vorbis/include", "libs/ogg/include", "%{cfg.targetdir}" }
        language "C++"
        kind "StaticLib"
        targetname "vorbisenc"   
        files { "libs/vorbis/lib/vorbisenc.c", "/libs/vorbis/win32/vorbisenc.def" }

    project "vorbisfile"
        vpaths {
            ["Sources/*"] = {"libs/vorbis/**.c",},
            ["*"] = {"premake5.lua", "CMakeLists.txt"}
        }
        includedirs { "libs/vorbis/include", "libs/ogg/include", "%{cfg.targetdir}" }
        language "C++"
        kind "StaticLib"
        targetname "vorbisfile"   
        files { "libs/vorbis/lib/vorbisfile.c", "/libs/vorbis/win32/vorbisfile.def" }

    project "imgui"
        vpaths {
            ["Headers/*"] = {"libs/imgui/**.h",},
            ["Sources/*"] = {"libs/imgui/**.c*",},
            ["*"] = {"premake5.lua", "CMakeLists.txt"}
        }
        includedirs { "libs/imgui", "libs/imgui/misc/cpp" }
        language "C++"
        kind "StaticLib"
        targetname "imgui" 

        local filePaths = {
            "imconfig.h", "imgui.h", "imgui_internal.h", "imstb_rectpack.h", "imstb_textedit.h", "imstb_truetype.h", 
            "imgui.cpp", "imgui_draw.cpp",  "imgui_widgets.cpp"
        }
        for i, fileName in pairs(filePaths) do 
            filePaths[i] = "libs/imgui/"..fileName
        end 
        files { "libs/imgui/misc/cpp/imgui_stdlib.h", "libs/imgui/misc/cpp/imgui_stdlib.cpp", table.unpack(filePaths) }      

group ""
    project "gta_reversed"
        vpaths {
            ["Headers/*"] = {"source/**.h*",},
            ["Sources/*"] = {"source/**.c*",},
            ["*"] = {"premake5.lua", "CMakeLists.txt"}
        }
        defines { "NOMINMAX", "USE_GTASA_ALLOCATOR", "EXTRA_DEBUG_FEATURES" }
        includedirs { "source", "source/**", "libs/vorbis/include", "libs/ogg/include", "libs/imgui", "libs/imgui/misc/cpp", "libs/dxsdk"}
        links { "ogg", "vorbis", "vorbisenc", "vorbisfile", "imgui" }
        libdirs { 
            "%{cfg.targetdir}/ogg.lib", "%{cfg.targetdir}/vorbis.lib", "%{cfg.targetdir}/vorbisfile.lib", 
            "%{cfg.targetdir}/vorbisenc.lib",  "%{cfg.targetdir}/imgui.lib", "libs/dxsdk/d3d9.lib", "libs/dxsdk/dinput.lib"
        }
        language "C++"
        kind "SharedLib"
        targetname "gta_reversed"
        targetextension ".asi"
        pchheader "StdInc.h"
        pchsource "source/StdInc.cpp"           
        files {
            "source/StdInc.h",
            "source/StdInc.cpp",
            "source/**.h*",
            "source/**.c*"
        }
        excludes{
            "source/**/errcom.def", --bugfix for premake5
            "source/**/errcore.def"
        }