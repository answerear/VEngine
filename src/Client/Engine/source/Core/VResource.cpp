

#include "VResource.h"


namespace Vision
{
	const VString DEFAULT_RESOURCE_GROUP_NAME = "Vision";
	const VString AUTO_ANIMATION_NAME = "[auto]";

	VBOOL splitResourceName(const Ogre::String& name,Ogre::String& resourceName,Ogre::String& groupName)
	{
		Ogre::String::size_type pos = name.find_first_of(':');
		if (pos ==Ogre::String::npos)
		{
			if (groupName.empty())
				groupName = DEFAULT_RESOURCE_GROUP_NAME;
			resourceName = name;
			return VFALSE;
		}
		else
		{
			groupName = name.substr(0, pos);
			resourceName = name.substr(pos+1,Ogre::String::npos);
			return VTRUE;
		}
	}

	VBOOL findCorrelativeResource(VString& resourceName,Ogre::String& groupName,	const Ogre::String& baseResourceName, const Ogre::String& baseGroupName)
	{
		Ogre::ResourceGroupManager& mgr = Ogre::ResourceGroupManager::getSingleton();

		Ogre::String name, path;
		Ogre::StringUtil::splitFilename(resourceName, name, path);
		VBOOL existsPath = !path.empty();
		VString grp = baseGroupName;

		// First, find in correlatived group and path if resource name doesn't exists path
		if (!existsPath)
		{
			Ogre::StringUtil::splitFilename(baseResourceName, name, path);
			if (!path.empty())
			{
				name = path + resourceName;
				if (mgr.resourceExists(grp, name))
				{
					resourceName = name;
					groupName = baseGroupName;
					return VTRUE;
				}
			}
		}

		// Second, find in correlatived group
		if (mgr.resourceExists(grp, resourceName))
		{
			groupName = baseGroupName;
			return VTRUE;
		}

		// Three, find in user given group
		if (!groupName.empty())
		{
			if (mgr.resourceExists(groupName, resourceName))
			{
				return VTRUE;
			}
		}

		// Four, find in global default group
		if (groupName != DEFAULT_RESOURCE_GROUP_NAME)
		{
			Ogre::String grp = DEFAULT_RESOURCE_GROUP_NAME;
			if (mgr.resourceExists(grp, resourceName))
			{
				groupName = grp;
				return VTRUE;
			}
		}

		return VFALSE;
	}

	Ogre::ResourcePtr tryLoadResource(Ogre::ResourceManager& resourceManager, const Ogre::String& resourceName, const Ogre::String& groupName)
	{
		try
		{
			return resourceManager.load(resourceName, groupName,VFALSE,NULL,NULL);
		}
		catch (...)
		{
			return Ogre::ResourcePtr();
		}
	}

	Ogre::ResourcePtr loadCorrelativeResource(
		const Ogre::String& resourceName, const Ogre::String& groupName,
		const Ogre::String& baseResourceName, const Ogre::String& baseGroupName,
		Ogre::ResourceManager& resourceManager)
	{
		Ogre::ResourcePtr res;

		Ogre::String name, path;
		Ogre::StringUtil::splitFilename(resourceName, name, path);
		VBOOL existsPath = !path.empty();

		// First, load in correlatived group and path if resource name doesn't exists path
		if (!existsPath)
		{
			Ogre::StringUtil::splitFilename(baseResourceName, name, path);
			if (!path.empty())
			{
				name = path + resourceName;
				res = tryLoadResource(resourceManager, name, baseGroupName);
				if (!res.isNull())
					return res;
			}
		}

		// Second, load in correlatived group
		res = tryLoadResource(resourceManager, resourceName, baseGroupName);
		if (!res.isNull())
			return res;

		// Three, load in user given group
		if (!groupName.empty())
		{
			res = tryLoadResource(resourceManager, resourceName, groupName);
			if (!res.isNull())
				return res;
		}

		// Four, load in global default group
		if (groupName != DEFAULT_RESOURCE_GROUP_NAME)
		{
			res = tryLoadResource(resourceManager, resourceName, groupName);
			if (!res.isNull())
				return res;
		}

		return res;
	}

	Ogre::MeshPtr loadMesh(const Ogre::String& meshName, const Ogre::String& groupName, const Ogre::String& baseResourceName, const Ogre::String& baseGroupName)
	{
		// Load the mesh
		Ogre::MeshPtr mesh = loadCorrelativeResource(
			meshName, groupName,
			baseResourceName, baseGroupName,
			Ogre::MeshManager::getSingleton());

		if (mesh.isNull())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
				"Unable to load mesh " + meshName,
				"loadMesh");
		}

		// Try to resolve skeleton resource
		if (mesh->hasSkeleton() && mesh->getSkeleton().isNull())
		{
			// resolve correlative with mesh
			Ogre::SkeletonPtr skeleton = loadCorrelativeResource(
				mesh->getSkeletonName(), groupName,
				mesh->getName(), mesh->getGroup(),
				Ogre::SkeletonManager::getSingleton());

			if (skeleton.isNull())
			{
				// resolve correlative with base resource
				skeleton = loadCorrelativeResource(
					mesh->getSkeletonName(), groupName,
					baseResourceName, baseGroupName,
					Ogre::SkeletonManager::getSingleton());
			}

			if (skeleton.isNull())
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
					"Unable to load skeleton " + mesh->getSkeletonName() +
					" for mesh " + mesh->getName(),
					"loadMesh");
			}

			// Set to the actual name
			mesh->setSkeletonName(skeleton->getName());
		}

		return mesh;
	}
}
