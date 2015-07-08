


VISION_PRE_BUILD_EVENT(Game)

IF(WIN32)
INCLUDE_DIRECTORIES(
	"include" 
	"include/GameStage"
	"include/Role"
	"include/Adapter"
#	"include/Win32"
	"../Engine/include"
	"../Engine/include/Camera"
	"../Engine/include/Scene"
	"../Engine/include/Object"
	"../Engine/include/Framework"
	"../Engine/include/XML"
	"../Engine/include/Math"
	"../Engine/include/Core"
	"../../Common/System/include"
#	${Vision_DependenciesInclude}/Ogre
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	)
ELSEIF(APPLE)
INCLUDE_DIRECTORIES(
	"include" 
	"include/GameStage"
	"include/Role"
	"include/Adapter"
#	"include/Mac"
	"../Engine/include"
	"../Engine/include/Camera"
	"../Engine/include/Scene"
	"../Engine/include/Object"
	"../Engine/include/Framework"
	"../Engine/include/XML"
	"../Engine/include/Math"
	"../Engine/include/Core"
	"../../Common/System/include"
#	${Vision_DependenciesInclude}/Ogre
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	)
ELSEIF(UNIX)
INCLUDE_DIRECTORIES(
	"include" 
	"include/GameStage"
	"include/Role"
	"include/Adapter"
#	"include/Linux"
	"../Engine/include"
	"../Engine/include/Camera"
	"../Engine/include/Scene"
	"../Engine/include/Object"
	"../Engine/include/Framework"
	"../Engine/include/XML"
	"../Engine/include/Math"
	"../Engine/include/Core"
	"../../Common/System/include"
#	${Vision_DependenciesInclude}/Ogre
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	)
ENDIF()


VISION_SET_PROJECT("FILE" include include/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\GameStage include/GameStage/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Role include/Role/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Adapter include/Adapter/ .h)
VISION_SET_PROJECT("FILE" source source/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\GameStage source/GameStage/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Role source/Role/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Adapter source/Adapter/ .cpp)


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
	
TARGET_LINK_LIBRARIES(
	${EXE_NAME}
	debug VSystem
	debug VEngine
	optimized VSystem
	optimized VEngine
	)


VISION_POST_BUILD_EVENT()

