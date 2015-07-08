
IF(WIN32)
	FIND_PATH(Vision_DependenciesLibrary
	  bin/debug/OgreMain_d.dll
	  PATHS 
	  ${PROJECT_SOURCE_DIR}/../Dependencies/${IDE}
	  NO_DEFAULT_PATH
	)
ELSEIF(APPLE)
	FIND_PATH(Vision_DependenciesLibrary
	  lib/debug/Ogre.framework
	  PATHS 
	  ${PROJECT_SOURCE_DIR}/../Dependencies/${IDE}
	  NO_DEFAULT_PATH
	)
ELSEIF(UNIX)
	FIND_PATH(Vision_DependenciesLibrary
	  bin/debug/OgreMain_d.dll
	  PATHS 
	  ${PROJECT_SOURCE_DIR}/../Dependencies/${IDE}
	  NO_DEFAULT_PATH
	)
ENDIF()



