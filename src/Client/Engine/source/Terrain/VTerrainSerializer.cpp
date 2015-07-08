

#include "VTerrainSerializer.h"
#include "VTerrainInfo.h"
#include "VisionXMLAttributes.h"


VTerrainXMLHandler::VTerrainXMLHandler(VTerrainInfo *info)
	: mInfo(info)
{

}

VTerrainXMLHandler::~VTerrainXMLHandler()
{

}


void VTerrainXMLHandler::startElement(const VString &element, const XMLAttributes &attributes)
{
	if (element == "Terrain")
	{
		mInfo->mName = attributes.getValueAs<VString>("name", Ogre::StringUtil::BLANK);
		mInfo->mXSize = attributes.getValueAs<VUINT32>("xsize");
		mInfo->mZSize = attributes.getValueAs<VUINT32>("zsize");
		mInfo->mTileSize = attributes.getValueAs<VUINT32>("tileSize");
	}
	else if (element == "scale")
	{
		mInfo->mScale = Ogre::Vector3(
			attributes.getValueAs<Ogre::Real>("x"),
			attributes.getValueAs<Ogre::Real>("y"),
			attributes.getValueAs<Ogre::Real>("z"));

		if (mInfo->mScale.x <= 0 || mInfo->mScale.y <= 0 || mInfo->mScale.z <= 0)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate scale definition in terrain file",
				"VTerrainXMLHandler::startElement");
		}

		mInfo->mPosition = Ogre::Vector3(-mInfo->mScale.x * mInfo->mXSize / 2, 
										 0, 
										 -mInfo->mScale.z * mInfo->mZSize / 2);
	}
	else if (element == "size")
	{
		mInfo->mScale = Ogre::Vector3(
			attributes.getValueAs<Ogre::Real>("x") / mInfo->mXSize,
			attributes.getValueAs<Ogre::Real>("y"),
			attributes.getValueAs<Ogre::Real>("z") / mInfo->mZSize);

		if (mInfo->mScale.x <= 0 || mInfo->mScale.y <= 0 || mInfo->mScale.z <= 0)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate size definition in terrain file",
				"VTerrainXMLHandler::startElement");
		}

		mInfo->mPosition = Ogre::Vector3(-mInfo->mScale.x * mInfo->mXSize / 2,
										 0,
										 -mInfo->mScale.z * mInfo->mZSize / 2);
	}
	else if (element == "position")
	{
		mInfo->mPosition = Ogre::Vector3(
			attributes.getValueAs<Ogre::Real>("x"),
			attributes.getValueAs<Ogre::Real>("y"),
			attributes.getValueAs<Ogre::Real>("z"));
	}
	else if (element == "centre" || element == "center")
	{
		mInfo->mPosition = Ogre::Vector3(
			attributes.getValueAs<Ogre::Real>("x") - mInfo->mScale.x * mInfo->mXSize / 2,
			attributes.getValueAs<Ogre::Real>("y"),
			attributes.getValueAs<Ogre::Real>("z") - mInfo->mScale.z * mInfo->mZSize / 2);
	}
	else if (element == "heightmap")
	{
		const VString &type = attributes.getValueAs<VString>("type", "standard");
		const VString &filename = attributes.getValue("filename");

		if (type != "image" && type != "standard" && type != "raw")
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate height data type",
				"VTerrainXMLHandler::startElement");
		}

		mInfo->mHeightmapType = type;
		mInfo->mHeightmapFilename = filename;
		if (mInfo->mHeightmapFilename.empty())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate height data filename",
				"VTerrainXMLHandler::startElement");
		}
	}
	else if (element == "gridInfo")
	{
		const VString &type = attributes.getValueAs<VString>("type", "standard");
		const VString &filename = attributes.getValue("filename");

		if (type != "standard")
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate grid info type",
				"VTerrainXMLHandler::startElement");
		}

		mInfo->mGridInfoType = type;
		mInfo->mGridInfoFilename = filename;
		if (mInfo->mGridInfoFilename.empty())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate grid info filename",
				"VTerrainXMLHandler::startElement");
		}
	}
	else if (element == "lightmap")
	{
		const VString &type = attributes.getValueAs<VString>("type", "standard");
		const VString &filename = attributes.getValue("filename");

		if (type != "standard" && type != "image")
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate lightmap type",
				"VTerrainXMLHandler::startElement");
		}

		mInfo->mLightmapType = type;
		mInfo->mLightmapFilename = filename;
		if (mInfo->mLightmapFilename.empty())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate lightmap filename",
				"VTerrainXMLHandler::startElement");
		}
	}
	else if (element == "textures")
	{
		// no attributes
	}
	else if (element == "texture")
	{
		const VString &type = attributes.getValueAs<VString>("type", "image");
		const VString &filename = attributes.getValue("filename");

		if (type != "image")
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate texture type",
				"VTerrainXMLHandler::startElement");
		}

		mInfo->mTextures.push_back(filename);
	}
	else if (element == "pixmaps")
	{
		// no attributes
	}
	else if (element == "pixmap")
	{
		Pixmap pixmap;
		pixmap.textureID = attributes.getValueAs<VUINT32>("textureId");
		pixmap.left = attributes.getValueAs<Ogre::Real>("left", 0);
		pixmap.top = attributes.getValueAs<Ogre::Real>("top", 0);
		pixmap.right = attributes.getValueAs<Ogre::Real>("right", 1);
		pixmap.bottom = attributes.getValueAs<Ogre::Real>("bottom", 1);

		if (!(0 <= pixmap.left && pixmap.left <= 1 
			&& 0 <= pixmap.top && pixmap.top <= 1
			&& 0 <= pixmap.right && pixmap.right <= 1
			&& 0 <= pixmap.bottom && pixmap.bottom <= 1))
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalidate pixmap definition in terrain file",
				"VTerrainXMLHandler::startElement");
		}

		mInfo->mPixmaps.push_back(pixmap);
	}
	else if (element == "materials")
	{
		// no attributes
	}
	else if (element == "template")
	{
		const VString &name = attributes.getValue("name");
		const VString &material = attributes.getValue("material");

		std::pair<MaterialTemplates::iterator, bool> inserted =
			mInfo->mMaterialTemplates.insert(MaterialTemplates::value_type(
			name, material));
		if (!inserted.second)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
				"Duplicate material template '" + inserted.first->first + "' in terrain file",
				"VTerrainXMLHandler::startElement");
		}
	}
	else if (element == "fog_replacement")
	{
		FogReplacement fogReplacement;
		fogReplacement.programNames[Ogre::FOG_NONE] = attributes.getValue("none");
		fogReplacement.programNames[Ogre::FOG_EXP] = attributes.getValue("exp");
		fogReplacement.programNames[Ogre::FOG_EXP2] = attributes.getValue("exp2");
		fogReplacement.programNames[Ogre::FOG_LINEAR] = attributes.getValue("linear");
		mInfo->mFogReplacements.push_back(fogReplacement);
	}
	else if (element == "surface")
	{

	}
	else if (element == "ambient")
	{

	}
	else if (element == "diffuse")
	{

	}
	else if (element == "specular")
	{

	}
	else if (element == "emisive")
	{

	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
			"Unexpected data was found while parsing the terrain file: '" + element + "' is unknown.",
			"VTerrainXMLHandler::startElement");
	}
}
