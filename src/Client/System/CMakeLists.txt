


VISION_PRE_BUILD_EVENT(VSystem)

IF(APPLE)
	INCLUDE(FindCarbon)
	INCLUDE(FindCocoa)
ENDIF()

macro(ADD_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${CMAKE_OSX_SYSROOT}/System/Library
        PATH_SUFFIXES Frameworks
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} ${FRAMEWORK_${fwname}})
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_FRAMEWORK)


ADD_DEFINITIONS( -D${LIB_NAME_TOUPPER}_EXPORTS -D_USRDLL)

IF(WIN32)
	INCLUDE_DIRECTORIES(
		"../../Common/System/include" 
		"../../Common/System/include/implement"
		"../../Common/System/include/win32"
		)
ELSEIF(APPLE)
	INCLUDE_DIRECTORIES(
		"../../Common/System/include" 
		"../../Common/System/include/implement"
		"../../Common/System/include/osx"
		"../../Common/System/include/posix"
		)
ELSEIF(UNIX)
	INCLUDE_DIRECTORIES(
		"../../Common/System/include" 
		"../../Common/System/include/implement"
		"../../Common/System/include/linux"
		"../../Common/System/include/posix"
		)
ENDIF()



VISION_SET_PROJECT("FILE" include ../../Common/System/include/ .h)
VISION_SET_PROJECT("FILE" include ../../Common/System/include/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\implement ../../Common/System/include/implement/ .h)
IF(WIN32)
	VISION_SET_PROJECT("FILE" include\\\\\\\\win32 ../../Common/System/include/win32/ .h)
	VISION_SET_PROJECT("FILE" include\\\\\\\\win32 ../../Common/System/include/win32/ .inl)
ELSEIF(APPLE)
	VISION_SET_PROJECT("FILE" include\\\\\\\\osx ../../Common/System/include/osx/ .h)
	VISION_SET_PROJECT("FILE" include\\\\\\\\osx ../../Common/System/include/osx/ .inl)
	VISION_SET_PROJECT("FILE" include\\\\\\\\posix ../../Common/System/include/posix/ .h)
	VISION_SET_PROJECT("FILE" include\\\\\\\\posix ../../Common/System/include/posix/ .inl)
ELSEIF(UNIX)
	VISION_SET_PROJECT("FILE" include\\\\\\\\linux ../../Common/System/include/linux/ .h)
	VISION_SET_PROJECT("FILE" include\\\\\\\\linux ../../Common/System/include/linux/ .inl)
	VISION_SET_PROJECT("FILE" include\\\\\\\\posix ../../Common/System/include/posix/ .h)
	VISION_SET_PROJECT("FILE" include\\\\\\\\posix ../../Common/System/include/posix/ .inl)
ENDIF()

VISION_SET_PROJECT("FILE" source ../../Common/System/source/ .cpp)
IF(WIN32)
	VISION_SET_PROJECT("FILE" source\\\\\\\\win32 ../../Common/System/source/win32/ .cpp)
ELSEIF(APPLE)
	VISION_SET_PROJECT("FILE" source\\\\\\\\osx ../../Common/System/source/osx/ .cpp)
	VISION_SET_PROJECT("FILE" source\\\\\\\\osx ../../Common/System/source/osx/ .mm)
	VISION_SET_PROJECT("FILE" source\\\\\\\\osx ../../Common/System/source/osx/ .m)
	VISION_SET_PROJECT("FILE" source\\\\\\\\posix ../../Common/System/source/posix/ .cpp)
ELSEIF(UNIX)
	VISION_SET_PROJECT("FILE" source\\\\\\\\linux ../../Common/System/source/linux/ .cpp)
	VISION_SET_PROJECT("FILE" source\\\\\\\\posix ../../Common/System/source/posix/ .cpp)
ENDIF()

SET(PLATFORM_LIBS "")

IF(WIN32)
ELSEIF(APPLE)
#	ADD_FRAMEWORK(Foundation ${LIB_NAME})
	SET(PLATFORM_LIBS ${Carbon_LIBRARY_FWK} ${Cocoa_LIBRARY_FWK})
ELSEIF(UNIX)
ENDIF()


ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})


TARGET_LINK_LIBRARIES(
	${LIB_NAME}
	${PLATFORM_LIBS}
	)
#TARGET_LINK_LIBRARIES(
#	${LIB_NAME} 
#	debug OgreMain_d 
#	debug CEGUIBase_d 
#	debug OgreGUIRenderer_d
#	optimized OgreMain 
#	optimized CEGUIBase
#	optimized OgreGUIRenderer
#	)

VISION_POST_BUILD_EVENT()

