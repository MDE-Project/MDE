#pragma once

#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/TypeID.h>

#define CAST_THIS(type) type* this = (type*)this_raw

#define OBJ(object) (MDK_Object*)object
#define OBJ_CREATE(type) (type*)MDK_Object_create((MDK_Object_Destructor)type##_destroy, sizeof(type))
#define REF(object) MDK_Object_ref((MDK_Object*)object)
#define UNREF(object) MDK_Object_unref((MDK_Object*)object)
#define REF_NULL(object) if (object) MDK_Object_ref((MDK_Object*)object)
#define UNREF_NULL(object) if (object) MDK_Object_unref((MDK_Object*)object)

#define ENSURE(type) MDK_TypeID_ensure(this->id, type##_typeID)

#define EVT(event) (MDK_Event*)event
