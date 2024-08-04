#pragma once

#include <MDK/Object.h>

#define OBJ(object) (MDK_Object*)object
#define OBJ_CREATE(type) (type*)MDK_Object_create((MDK_Object_Destructor)destructor, sizeof(type))
#define REF(object) MDK_Object_ref((MDK_Object*)object)
#define UNREF(object) MDK_Object_unref((MDK_Object*)object)
