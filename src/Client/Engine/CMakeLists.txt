


VISION_PRE_BUILD_EVENT(VEngine)


ADD_DEFINITIONS(-D${LIB_NAME_TOUPPER}_EXPORTS -D_USRDLL)

INCLUDE_DIRECTORIES(
	"include" 
	"include/Camera"
	"include/Scene"
	"include/Object"
	"include/Effect"
	"include/Terrain"
	"include/Framework"
	"include/XML"
	"include/Math"
	"include/Utils"
	"include/OgreExt"
	"include/Core"
	"../expat/include"
	"../tinyxml/include"
	"../../Common/System/include"
	${Vision_DependenciesInclude}/Ogre
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	${Vision_DependenciesInclude}/boost
	)



VISION_SET_PROJECT("FILE" include include/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Camera include/Camera/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Camera include/Camera/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Scene include/Scene/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Scene include/Scene/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Object include/Object/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Object include/Object/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Effect include/Effect/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Effect include/Effect/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Terrain include/Terrain/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Terrain include/Terrain/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Framework include/Framework/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Framework include/Framework/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\XML include/XML/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\XML include/XML/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Math include/Math/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Math include/Math/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Utils include/Utils/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Utils include/Utils/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\OgreExt include/OgreExt/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\OgreExt include/OgreExt/ .inl)
VISION_SET_PROJECT("FILE" include\\\\\\\\Core include/Core/ .h)
VISION_SET_PROJECT("FILE" include\\\\\\\\Core include/Core/ .inl)
VISION_SET_PROJECT("FILE" source source/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Camera source/Camera/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Scene source/Scene/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Object source/Object/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Effect source/Effect/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Terrain source/Terrain/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Framework source/Framework/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\XML source/XML/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Math source/Math/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Utils source/Utils/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\OgreExt source/OgreExt/ .cpp)
VISION_SET_PROJECT("FILE" source\\\\\\\\Core source/Core/ .cpp)

ADD_LIBRARY(${LIB_NAME} SHARED ${${LIB_NAME}_FILE})

IF(WIN32)
TARGET_LINK_LIBRARIES(
	${LIB_NAME} 
	debug expat
	debug tinyxml
	debug OgreMain_d 
	debug VSystem
	debug OIS_d
#	debug CEGUIBase_d 
#	debug OgreGUIRenderer_d
	optimized expat
	optimized tinyxml
	optimized OgreMain 
	optimized VSystem
	optimized OIS
#	optimized CEGUIBase
#	optimized OgreGUIRenderer
	)
ELSEIF(APPLE)
TARGET_LINK_LIBRARIES(
	${LIB_NAME} 
	"-framework IOKit"
	debug expat
	debug tinyxml
	debug "${Vision_DependenciesLibrary}/lib/Debug/Ogre.framework"
	debug VSystem
	debug OIS
#	debug CEGUIBase_d 
#	debug OgreGUIRenderer_d
	optimized expat
	optimized tinyxml
#	optimized "${Vision_DependenciesLibrary}/lib/Release/Ogre.framework"
	optimized VSystem
	optimized OIS
#	optimized CEGUIBase
#	optimized OgreGUIRenderer
	)
ELSEIF(UNIX)
ENDIF()

VISION_POST_BUILD_EVENT()

