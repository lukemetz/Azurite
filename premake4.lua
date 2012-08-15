solution "Azurite"
  configurations { "Debug", "Release" }
  
  project "Azurite"
    kind "ConsoleApp"
    language "C++"
      files { "**.h", "**.cpp"}
      links{"Sapling", 'glfw','Horde3D', 'Horde3DUtils', "jansson"}
      includedirs{".","Samples/tactics/"} 
      if os.is('linux') then --Try to find horde3d and glfw
        libdirs{'/usr/local/lib'}
        includedirs{'/usr/local/include/'}
      end
      buildoptions{"-std=c++11 -Wall"} 
      targetdir ""
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }

