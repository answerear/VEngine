

#ifndef __VISION_GAME_OBJECT_H__
#define __VISION_GAME_OBJECT_H__


#include "VObject.h"


class VGameObject
{
public:
	enum ObjectType
	{
		OBJ_TYPE_DUMMY,
		OBJ_TYPE_MESH,
		OBJ_TYPE_MODEL,
		OBJ_TYPE_ACTOR
	};
public:
	/** 得到类型 */
	virtual ObjectType getType() const = 0;

	/** 设置世界坐标 */
	virtual void setPosition(const Vector3 &pos) = 0;
	/** 设置旋转角度 */
	virtual void setOrientation(const Vector3 &rot) = 0;
	/** 设置属性 */
	virtual void setProperty(const VString &propertyName, const VString &propertyValue) = 0;
	/** 设置可见性 */
	virtual void setVisible(VBOOL visible) = 0;
	virtual void setLeftWeaponEffect(const VString &effectName) = 0;
};


#endif	/*__VISION_GAME_OBJECT_H__*/
