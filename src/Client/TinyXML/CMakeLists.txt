
VISION_PRE_BUILD_EVENT(tinyxml)

ADD_DEFINITIONS(-D_LIB)

INCLUDE_DIRECTORIES(
	"include" 
#	${Vision_DependenciesInclude}/Ogre
#	${Vision_DependenciesInclude}/Ogre/CEGUI
	)



VISION_SET_PROJECT("FILE" include include/ .h)
VISION_SET_PROJECT("FILE" source source/ .cpp)


ADD_LIBRARY(${LIB_NAME} STATIC ${${LIB_NAME}_FILE})

#TARGET_LINK_LIBRARIES(
#	${LIB_NAME} 
#	debug OgreMain_d 
#	debug CEGUIBase_d 
#	debug OgreGUIRenderer_d
#	optimized OgreMain 
#	optimized CEGUIBase
#	optimized OgreGUIRenderer
#	)

#VISION_POST_BUILD_EVENT()