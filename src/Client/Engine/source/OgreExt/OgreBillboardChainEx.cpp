
#include "OgreBillboardChainEx.h"
#include <OgreHardwareBufferManager.h>
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreRoot.h>


namespace Ogre
{
	BillboardChainEx::Element::Element()
		: position(Vector3::ZERO)
		, width(0.0f)
		, texCoord(0.0f)
		, colour(ColourValue::Black)
	{
	}
	//------------------------------------------------------------------
	BillboardChainEx::Element::Element(const Vector3 &pos, 
		const Real &w, 
		const Real &t, 
		ColourValue &c)
		: position(pos)
		, width(w)
		, texCoord(t)
		, colour(c)
	{
	}
	//------------------------------------------------------------------
	BillboardChainEx::BillboardChainEx(size_t maxElements /* = 10 */)
		: SimpleRenderable()
		, mMaxElements(maxElements)
		, mRadius(0.0f)
		, mElementList(maxElements)
		, mRenderEnabled(false)
		, mBoundsDirty(true)
	{
		_setupVertexBuffer();

		setMaterial("BaseWhiteNoLighting");
	}
	//------------------------------------------------------------------
	BillboardChainEx::~BillboardChainEx()
	{
		if (mRenderOp.vertexData)
		{
			delete mRenderOp.vertexData;
			mRenderOp.vertexData = NULL;
		}
	}
	//------------------------------------------------------------------
	void BillboardChainEx::setMaxChainElements(size_t maxElements)
	{
		mMaxElements = maxElements;
		_setupVertexBuffer();
		mElementList.resize(mMaxElements);
	}
	//------------------------------------------------------------------
	size_t BillboardChainEx::getMaxChainElements() const
	{
		return mMaxElements;
	}
	//------------------------------------------------------------------
	void BillboardChainEx::updateChainElement(size_t elementIndex, const Element &billboardChainElement)
	{
		assert(elementIndex < mMaxElements);
		mElementList[elementIndex] = billboardChainElement;
		mBoundsDirty = true;
	}
	//------------------------------------------------------------------
	void BillboardChainEx::updateBoundingBox()
	{
		if (mBoundsDirty)
		{
			if (mElementList.size() < 2)
				return;

			Ogre::Real width = mElementList[0].width;
			Vector3 widthVector = Vector3(width, width, width);
			const Vector3 &position = mElementList[0].position;
			Vector3 minimum = position - widthVector;
			Vector3 maximum = position + widthVector;

			for (size_t i = 1; i < mElementList.size(); i++)
			{
				// Update the bounds of the bounding box
				Ogre::Real width = mElementList[i].width;
				Vector3 widthVector = Vector3(width, width, width);
				const Vector3& position = mElementList[i].position;

				minimum.makeFloor(position - widthVector);
				maximum.makeCeil(position + widthVector);
			}

			// Set the current bounding box
			setBoundingBox(AxisAlignedBox(minimum, maximum));

			// Set the current radius
			mRadius = Math::Sqrt(std::max(minimum.squaredLength(), maximum.squaredLength()));

			mBoundsDirty = false;
		}
	}
	//------------------------------------------------------------------
	void BillboardChainEx::_notifyCurrentCamera(Camera *cam)
	{
		SimpleRenderable::_notifyCurrentCamera(cam);
		_updateVertexBuffer();
	}
	//------------------------------------------------------------------
	void BillboardChainEx::_updateRenderQueue(RenderQueue *queue)
	{
		if (mRenderEnabled)
		{
			queue->addRenderable(this, mRenderQueueID, OGRE_RENDERABLE_DEFAULT_PRIORITY);
		}
	}
	//------------------------------------------------------------------
	Real BillboardChainEx::getSquaredViewDepth(const Camera *cam) const
	{
		Vector3 min, max, mid, dist;
		min = mBox.getMinimum();
		max = mBox.getMaximum();
		mid = ((max - min) * 0.5f) + min;
		dist = cam->getDerivedPosition() - mid;

		return dist.squaredLength();
	}
	//------------------------------------------------------------------
	Real BillboardChainEx::getBoundingRadius() const
	{
		return mRadius;
	}
	//------------------------------------------------------------------
	void BillboardChainEx::_setupVertexBuffer()
	{
		if (mRenderOp.vertexData)
		{
			delete mRenderOp.vertexData;
			mRenderOp.vertexData = NULL;
		}

		size_t vertexCount = mMaxElements * 2;
		mRenderOp.vertexData = new VertexData();
		mRenderOp.indexData = NULL;
		mRenderOp.vertexData->vertexCount = vertexCount;
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.operationType = RenderOperation::OT_TRIANGLE_STRIP;
		mRenderOp.useIndexes = false;

		VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

		// Add a description for the buffer of the positions of the vertices
		size_t offset = 0;
		decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
		offset += VertexElement::getTypeSize(VET_FLOAT3);

		decl->addElement(0, offset, VET_COLOUR, VES_DIFFUSE);
		offset += VertexElement::getTypeSize(VET_COLOUR);

		decl->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
		offset += VertexElement::getTypeSize(VET_FLOAT2);

		// Create the buffer
		HardwareVertexBufferSharedPtr vertexBuffer =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(0),
			vertexCount,
			HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// Bind the buffer
		bind->setBinding(0, vertexBuffer);
	}
	//------------------------------------------------------------------
	void BillboardChainEx::_updateVertexBuffer()
	{
		if (mElementList.size() < 2)
			return;

		HardwareVertexBufferSharedPtr vertexBuffer =
			mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

		void *bufferStart = vertexBuffer->lock(HardwareBuffer::HBL_DISCARD);

		// Here. we need to compute the position of the camera in the coordinate system of the billboard chain.
		Vector3 eyePos = mParentNode->_getDerivedOrientation().Inverse() *
			(m_pCamera->getDerivedPosition() - mParentNode->_getDerivedPosition());

		// Compute the position of the vertices in the chain
		unsigned int chainSize = mElementList.size();
		for (size_t i = 0; i < chainSize; i++)
		{
			// 计算新的偏移量，因为每个循环是计算两个顶点，所以要乘以2
			void *base = static_cast<void*>(
				static_cast<char*>(bufferStart) +
				vertexBuffer->getVertexSize() * i * 2);

			Vector3 chainTangent;
			if (i == 0) 
			{
				chainTangent = mElementList[1].position - mElementList[0].position;
			}
			else if (i == chainSize - 1) 
			{
				chainTangent = mElementList[chainSize - 1].position - mElementList[chainSize - 2].position;
			}
			else 
			{
				chainTangent = mElementList[i + 1].position - mElementList[i - 1].position;
			}

			const Vector3 &p1 = mElementList[i].position;

			Vector3 vP1ToEye = eyePos - p1;
			Vector3 vPerpendicular = chainTangent.crossProduct(vP1ToEye);
			vPerpendicular.normalise();
			vPerpendicular *= mElementList[i].width;

			Vector3 pos0 = p1 - vPerpendicular;
			Vector3 pos1 = p1 + vPerpendicular;

			// vertex 1
			// pos
			float *value = static_cast<float*>(base);

			*value++ = pos0.x;
			*value = pos0.y;
			*value++ = pos0.z;

			// colour
			ColourValue colour = mElementList[i].colour;
			RGBA *rgb = static_cast<RGBA*>(static_cast<void*>(value));
			Root::getSingleton().convertColourValue(colour, rgb);
			rgb++;

			// tex coord
			value = static_cast<float*>( static_cast<void*>(rgb) );
			*value++ = mElementList[i].texCoord;
			*value++ = 0.0f;

			// vertex 2
			// pos
			*value++ = pos1.x;
			*value++ = pos1.y;
			*value++ = pos1.z;

			// colour
			rgb = static_cast<RGBA*>( static_cast<void*>(value) );
			Root::getSingleton().convertColourValue(colour, rgb);
			rgb++;

			// tex coord
			value = static_cast<float*>( static_cast<void*>(rgb) );
			*value++ = mElementList[i].texCoord;
			*value++ = 1.0f;
		}

		vertexBuffer->unlock();
	}
}
