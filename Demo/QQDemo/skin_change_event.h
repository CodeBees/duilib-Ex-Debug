#ifndef SKIN_CHANGE_EVENT_HPP
#define SKIN_CHANGE_EVENT_HPP

#include "observer_impl_base.h"

struct SkinChangedParam
{
	DWORD bkcolor;
	CDuiString bgimage;
};

typedef class ObserverImpl<BOOL, SkinChangedParam> SkinChangedObserver;
typedef class ReceiverImpl<BOOL, SkinChangedParam> SkinChangedReceiver;


#endif // SKIN_CHANGE_EVENT_HPP