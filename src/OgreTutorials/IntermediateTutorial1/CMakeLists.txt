


VISION_PRE_BUILD_EVENT(IntermediateTutorial1)

IF(WIN32)
INCLUDE_DIRECTORIES(
	"../TutorialFramework" 
	${Vision_DependenciesInclude}/Ogre
	${Vision_DependenciesInclude}/OIS
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	)
ELSEIF(APPLE)
INCLUDE_DIRECTORIES(
	"../TutorialFramework" 
	${Vision_DependenciesInclude}/Ogre
	${Vision_DependenciesInclude}/OIS
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	)
ELSEIF(UNIX)
INCLUDE_DIRECTORIES(
	"../TutorialFramework" 
	${Vision_DependenciesInclude}/Ogre
	${Vision_DependenciesInclude}/OIS
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	)
ENDIF()

VISION_SET_PROJECT("FILE" TutorialFramework ../TutorialFramework/ .h)
VISION_SET_PROJECT("FILE" include ./ .h)
VISION_SET_PROJECT("FILE" TutorialFramework ../TutorialFramework/ .cpp)
VISION_SET_PROJECT("FILE" source ./ .cpp)

IF(WIN32)
ADD_EXECUTABLE(
	${EXE_NAME} WIN32 
	${${EXE_NAME}_FILE}
	)
ELSE()
ADD_EXECUTABLE(
	${EXE_NAME}
	${${EXE_NAME}_FILE}
	)
ENDIF()
	
IF(WIN32)
TARGET_LINK_LIBRARIES(
	${EXE_NAME}
	debug OgreMain_d
	debug OIS_d
	optimized OgreMain
	optimized OIS
	)
ELSE()
ENDIF()


VISION_POST_BUILD_EVENT()

