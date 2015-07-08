

#ifndef __VEVENT_MACRO_H__
#define __VEVENT_MACRO_H__


#define V_DECLARE_EVENT_MAP() \
	protected: \
	virtual VBOOL processEvent(VUINT32 eventID, VUINT32 param); \
	VBOOL eventProc(VUINT32 eventID, VUINT32 param);


#define V_BEGIN_EVENT_MAP(theClass, classBase) \
	VBOOL theClass::processEvent(VUINT32 eventID, VUINT32 param) \
	{ \
		VBOOL bResult = eventProc(eventID, param); \
		if (!bResult) \
		{ \
			bResult = classBase::processEvent(eventID, param); \
		} \
		return bResult; \
	} \
	\
	VBOOL theClass::eventProc(VUINT32 eventID, VUINT32 param) \
	{ \
		VBOOL bResult = VFALSE; \


#define V_ON_EVENT(id, func)	\
		if (id == eventID)	\
		{	\
			bResult = func(param);	\
			return bResult;	\
		}	\

#define V_END_EVENT_MAP()	\
		return bResult;	\
	}

#endif	/*__VEVENT_MACRO_H__*/
