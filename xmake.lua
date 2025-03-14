
set_languages( "cxx11" )

add_rules("mode.debug")
add_rules("mode.release")

target( "libWyvernTool" )
    set_kind( "binary" )
    
    set_basename("libWyvernTool_test_$(mode)_$(arch)")

    set_targetdir "bin"
    set_objectdir "build/obj"

    add_headerfiles( 
        "**.h",
        "**.hpp" 
        )
    add_files( "**.cpp" )
    add_includedirs( "include/" )
target_end()
