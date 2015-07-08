

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
	/** �õ����� */
	virtual ObjectType getType() const = 0;

	/** ������������ */
	virtual void setPosition(const Vector3 &pos) = 0;
	/** ������ת�Ƕ� */
	virtual void setOrientation(const Vector3 &rot) = 0;
	/** �������� */
	virtual void setProperty(const VString &propertyName, const VString &propertyValue) = 0;
	/** ���ÿɼ��� */
	virtual void setVisible(VBOOL visible) = 0;
	virtual void setLeftWeaponEffect(const VString &effectName) = 0;
};


#endif	/*__VISION_GAME_OBJECT_H__*/
