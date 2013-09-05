# MSVC PrecompiledHeader 
MACRO( MSVC_SET_PCH Target PrecompiledHeader PrecompiledSource)
  IF( MSVC )
    SET(PrecompiledBinary "\$(IntDir)\$(TargetName).pch")
    SET(Sources ${${SourcesVar}})
    GET_FILENAME_COMPONENT(PrecompiledBasename
       ${PrecompiledHeader} NAME)
    GET_SOURCE_FILE_PROPERTY(OLD_COMPILE_FLAGS ${PrecompiledSource} COMPILE_FLAGS)

    SET_PROPERTY(SOURCE ${PrecompiledSource}
      APPEND
      PROPERTY
        COMPILE_FLAGS
          "/Yc\"${PrecompiledBasename}\" /Fp\"${PrecompiledBinary}\"")
    SET_PROPERTY(SOURCE ${PrecompiledSource}
      APPEND
      PROPERTY
        OBJECT_OUTPUTS "${PrecompiledBinary}") 
    SET_PROPERTY(TARGET ${Target}
      APPEND
      PROPERTY
        COMPILE_FLAGS
          "/Yu\"${PrecompiledBasename}\"")
  ENDIF( MSVC )
ENDMACRO( MSVC_SET_PCH )