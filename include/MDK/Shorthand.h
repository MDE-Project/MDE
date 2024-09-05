#pragma once

#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/TypeID.h>

#define CAST_THIS(TYPE) TYPE* this = (TYPE*)this_raw

#define OBJ(OBJECT) (MDK_Object*)OBJECT
#define OBJ_CREATE(TYPE) (TYPE*)MDK_Object_create((MDK_Object_Destructor)TYPE##_destroy, sizeof(TYPE))
#define REF(OBJECT) MDK_Object_ref((MDK_Object*)OBJECT)
#define UNREF(OBJECT) MDK_Object_unref((MDK_Object*)OBJECT)
#define REF_NULL(OBJECT) if (OBJECT) MDK_Object_ref((MDK_Object*)OBJECT)
#define UNREF_NULL(OBJECT) if (OBJECT) MDK_Object_unref((MDK_Object*)OBJECT)

#define ENSURE(TYPE) MDK_TypeID_ensure(this->id, TYPE##_typeID)

#define EVT(EVENT) (MDK_Event*)EVENT
