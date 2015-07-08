


VISION_PRE_BUILD_EVENT(Plugin_Scorpio)


ADD_DEFINITIONS(-DPLUGIN_Scorpio_EXPORTS -D_USRDLL)

INCLUDE_DIRECTORIES(
	"include" 
	${Vision_DependenciesInclude}/Ogre
#	${Vision_DependenciesInclude}/Ogre/CEGUI
#	${Vision_DependenciesInclude}/boost
	)



VISION_SET_PROJECT("FILE" include include/ .h)
VISION_SET_PROJECT("FILE" source src/ .cpp)
VISION_SET_PROJECT("FILE" PhysicsSimulation src/PhysicsSimulation/ .cpp)

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

IF(WIN32)
TARGET_LINK_LIBRARIES(
	${LIB_NAME} 
	debug OgreMain_d 
	optimized OgreMain 
	)
ELSEIF(APPLE)
TARGET_LINK_LIBRARIES(
	${LIB_NAME} 
	"-framework IOKit"
	debug "${Vision_DependenciesLibrary}/lib/Debug/Ogre.framework"
#	optimized "${Vision_DependenciesLibrary}/lib/Release/Ogre.framework"
	)
ELSEIF(UNIX)
ENDIF()

VISION_POST_BUILD_EVENT()

