
#ifndef __OGRE_BILLBOARD_CHAIN_EX_H__
#define __OGRE_BILLBOARD_CHAIN_EX_H__


#include <OgreSimpleRenderable.h>


namespace Ogre
{
	class BillboardChainEx : public SimpleRenderable
	{
	public:
		class Element
		{
		public:
			Element();
			Element(const Vector3 &pos, const Real &width, const Real &texCoord, ColourValue &colour);

			Vector3 position;
			Real	width;
			Real	texCoord;
			ColourValue	colour;
		};

		typedef std::vector<Element> ElementList;

	public:
		BillboardChainEx(size_t maxElements = 10);
		virtual ~BillboardChainEx();

		void setMaxChainElements(size_t maxElements);
		size_t getMaxChainElements() const;

		void updateChainElement(size_t elementIndex, const Element &billboardChainElement);
		void updateBoundingBox();

		void setEnabled(bool enabled)
		{
			mRenderEnabled = enabled;
		}
		bool getEnabled() const
		{
			return mRenderEnabled;
		}

	protected:
		virtual void _notifyCurrentCamera(Camera *cam);
		virtual void _updateRenderQueue(RenderQueue *queue);
		virtual Real getSquaredViewDepth(const Camera *cam) const;
		virtual Real getBoundingRadius() const;

		void _setupVertexBuffer();
		void _updateVertexBuffer();

	protected:
		size_t	mMaxElements;

		Real		mRadius;
		ElementList	mElementList;

		bool	mRenderEnabled;
		bool	mBoundsDirty;
	};
}


#endif	/*__OGRE_BILLBOARD_CHAIN_EX_H__*/
