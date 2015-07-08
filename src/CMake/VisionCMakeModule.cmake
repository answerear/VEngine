IF(WIN32)
	# settings for Microsoft Visual C++
	IF(CMAKE_CXX_COMPILER MATCHES cl)
	     IF(CMAKE_GENERATOR STREQUAL "Visual Studio 7" OR CMAKE_GENERATOR STREQUAL "Visual Studio 7 .NET 2003")
			SET(IDE "vc71")
	     ELSEIF(CMAKE_GENERATOR STREQUAL "Visual Studio 8 2005")
	        SET(IDE "vc80")
	     ELSEIF(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")
	        SET(IDE "vc90")
	     ELSEIF(CMAKE_GENERATOR STREQUAL "Visual Studio 10")
	     	SET(IDE "vc100")
	     ENDIF()
	ENDIF(CMAKE_CXX_COMPILER MATCHES cl)
ELSEIF(APPLE)
	# settings for Apple
	SET(IDE "osx")
ELSEIF(UNIX)
	# settings for Linux
	SET(IDE "linux")
ENDIF()


MACRO(VISION_ADD_PROJECT type group dir)

	FOREACH(var ${ARGN})
		LIST(APPEND ${LIB_NAME}_${type} ${dir}${var})
		SOURCE_GROUP(${group} FILES ${dir}${var})
	ENDFOREACH( var )

ENDMACRO(VISION_ADD_PROJECT)



MACRO(VISION_SET_PROJECT type group dir match)

	FILE(GLOB var ${dir}*${match})
	
	FOREACH(f ${var})
		GET_FILENAME_COMPONENT(f ${f} NAME_WE)
		VISION_ADD_PROJECT(${type} ${group} ${dir} "${f}${match}")
	ENDFOREACH()

ENDMACRO(VISION_SET_PROJECT)



MACRO(VISION_ADD_PROJECT_FILE group dir)
	FOREACH(var ${ARGN})
		LIST(APPEND ${LIB_NAME}_FILE ${dir}${var})
		SOURCE_GROUP(${group} FILES ${dir}${var})
	ENDFOREACH( var )
ENDMACRO(VISION_ADD_PROJECT_FILE)



MACRO(VISION_SET_PROJECT_FILES group dir match)

	FILE(GLOB orz_files  ${dir}*${match})
		
	FOREACH(f ${orz_files})
		GET_FILENAME_COMPONENT(f ${f} NAME_WE)
		VISION_ADD_PROJECT_FILE(${group} ${dir} "${f}${match}")

	ENDFOREACH()

ENDMACRO(VISION_SET_PROJECT_FILES)






MACRO( VISION_PRE_BUILD_EVENT target )
	SET( LIB_NAME ${target} )
	SET( EXE_NAME ${target} )
	STRING(TOUPPER ${LIB_NAME} LIB_NAME_TOUPPER)
ENDMACRO( VISION_PRE_BUILD_EVENT )


IF(WIN32)
	MACRO( VISION_POST_BUILD_EVENT )
		INSTALL(TARGETS ${LIB_NAME} 
			RUNTIME DESTINATION bin/debug
			CONFIGURATIONS Debug
		#	LIBRARY DESTINATION bin/debug
		#	CONFIGURATIONS Debug
		#	ARCHIVE DESTINATION lib/debug
		#	CONFIGURATIONS Debug
		)
	
		INSTALL(TARGETS ${LIB_NAME} 
			RUNTIME DESTINATION bin/release
			CONFIGURATIONS Release
		#	LIBRARY DESTINATION bin/release 
		#	CONFIGURATIONS Release
		#	ARCHIVE DESTINATION lib/release
		#	CONFIGURATIONS Release
		)
	
	ENDMACRO(  VISION_POST_BUILD_EVENT)
ELSE()
	MACRO( VISION_POST_BUILD_EVENT )
		INSTALL(TARGETS ${LIB_NAME} 
			RUNTIME DESTINATION bin/debug
			CONFIGURATIONS Debug
			LIBRARY DESTINATION bin/debug
			CONFIGURATIONS Debug
		#	ARCHIVE DESTINATION lib/debug
		#	CONFIGURATIONS Debug
		)
	
		INSTALL(TARGETS ${LIB_NAME} 
			RUNTIME DESTINATION bin/release
			CONFIGURATIONS Release
			LIBRARY DESTINATION bin/release 
			CONFIGURATIONS Release
		#	ARCHIVE DESTINATION lib/release
		#	CONFIGURATIONS Release
		)
	
	ENDMACRO(  VISION_POST_BUILD_EVENT)
ENDIF()





MACRO(VISION_FIND name dir file)
	SET(${name}_DIR "" CACHE PATH "the directory of the ${name}")


	IF(${name}_DIR)
 		FIND_PATH( 
		_temp_ 	
		${file}
		PATHS 
		${${name}_DIR}
		NO_DEFAULT_PATH
		)
	
		IF(NOT _temp_)
			MESSAGE("\"${${name}}\" is not the ${name}'s directory")
			UNSET(${name} CACHE)
		ENDIF(NOT _temp_)
	
		UNSET(_temp_ CACHE)
	ENDIF(${name}_DIR)

	IF(NOT ${name}_DIR)
		UNSET(${name} CACHE)
	FIND_PATH(
		_temp_	
		${file}
		PATHS 
		${PROJECT_SOURCE_DIR}/build/${dir}
		${PROJECT_SOURCE_DIR}/../build/${dir}
		${PROJECT_SOURCE_DIR}/../../build/${dir}
		NO_DEFAULT_PATH
		)
		SET(${name}_DIR  ${_temp_} CACHE PATH "the directory of the ${name}" FORCE)
		UNSET(_temp_ CACHE)

		
		MESSAGE(${name}_DIR)	
	ENDIF(NOT ${name}_DIR)

	IF(NOT ${name}_DIR)
		MESSAGE("[You have to select the directory of the ${name}!] ")
	ENDIF(NOT ${name}_DIR)	
ENDMACRO(VISION_FIND)
