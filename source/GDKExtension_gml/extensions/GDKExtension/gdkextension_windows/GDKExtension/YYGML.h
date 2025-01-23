// Copyright © Opera Norway AS. All rights reserved.
// This file is an original work developed by Opera.
#ifndef __YYGML_H__
#define __YYGML_H__

// uncomment the line below to enable extra checks while running to isolate Garbage Collection (GC) issues
//#define GC_ENABLE_EXTRA_CHECKS

#include "YYStd.h"
#include "Ref.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <math.h>

# if defined(_MSC_VER)
#pragma warning ( 3: 4456)
#endif 

#if defined(YYLLVM)
#define NaN    					NAN
# if defined(_MSC_VER) || (__cplusplus > 199711L)
#  if defined(_MSC_VER) && (_MSC_VER < 1700)
#  error Visual Studio 2012 should be used for compiling YYC 
#  endif
// disable microsoft warning on unreachable code.
#if !defined(__clang__)
#pragma warning( disable : 4702)
#endif
// Lambda style (for MSVC et al)
#define YYCOMPOUNDEXPR_BEGIN	[&](){
#define YYCOMPOUNDEXPR_RETURN	return
#define YYCOMPOUNDEXPR_END		;}()
# else
// GCC style
#define YYCOMPOUNDEXPR_BEGIN	({
#define YYCOMPOUNDEXPR_RETURN	
#define YYCOMPOUNDEXPR_END		;})
# endif
#endif

#define ARRAY_INDEX_NO_INDEX	INT_MIN

#define POINTER_INVALID			((void*)-1)
#define POINTER_NULL			(NULL)

// Save and restore the arrayowner
#if defined(YYLLVM_SEP_DLL)
YYCEXTERN	int64	YYGML_GetArrayOwner();
YYCEXTERN	void	YYGML_SetArrayOwner( int64 _owner );
#else
extern int64 g_CurrentArrayOwner;
FORCEINLINE int64 NOTHROW_ATTR YYGML_GetArrayOwner()  PURE_ATTR FORCEINLINE_ATTR;
FORCEINLINE int64 NOTHROW_ATTR YYGML_GetArrayOwner()
{
	return g_CurrentArrayOwner;
} // end YYGML_GetArrayOwner
FORCEINLINE void NOTHROW_ATTR YYGML_SetArrayOwner( int64 _owner) FORCEINLINE_ATTR;
FORCEINLINE void NOTHROW_ATTR YYGML_SetArrayOwner( int64 _owner)
{
#ifdef YYLLVM_COPY_ON_WRITE
	g_CurrentArrayOwner = _owner;
#endif
} // end YYGML_SetArrayOwner
#endif
struct SYYArrayOwner
{
	int64 arrayOwner;
	SYYArrayOwner() {
		arrayOwner = YYGML_GetArrayOwner();
	}
	~SYYArrayOwner() {
		YYGML_SetArrayOwner( arrayOwner );
	} 
};

#if defined(YYLLVM) || defined(__YYLLVM__)
#if defined(GC_ENABLE_EXTRA_CHECKS)
extern int g_ContextStackTop;
#endif
struct SYYStackTrace
{
	SYYStackTrace* pNext;
	const char* pName;
	int line;
#if defined(GC_ENABLE_EXTRA_CHECKS)
	int tos;
#endif
	static SYYStackTrace*	s_pStart;
	SYYStackTrace(const char* _pName, int _line)
	{
		pName = _pName;
		line = _line;
		pNext = s_pStart;
		s_pStart = this;
#if defined(GC_ENABLE_EXTRA_CHECKS)
		tos = g_ContextStackTop;
#endif
	} // end constructor

	~SYYStackTrace() {
		s_pStart = pNext;
	} // end destructor
};
#ifdef YYLLVM_COPY_ON_WRITE
#define YY_STACKTRACE_FUNC_ENTRY( _name, _line )	SYYArrayOwner __array; SYYStackTrace __stackFunc( _name, _line )
#else
#define YY_STACKTRACE_FUNC_ENTRY( _name, _line )	SYYStackTrace __stackFunc( _name, _line )
#endif
#if defined(GC_ENABLE_EXTRA_CHECKS)
# define YY_STACKTRACE_LINE( _line )					if (__stackFunc.tos != g_ContextStackTop) YYError("GC Context Stack Imbalance" ); __stackFunc.line = _line
#else
# define YY_STACKTRACE_LINE( _line )					__stackFunc.line = _line
#endif
#else
#ifdef YYLLVM_COPY_ON_WRITE
# define YY_STACKTRACE_FUNC_ENTRY( _name, _line )	SYYArrayOwner __array
#else
# define YY_STACKTRACE_FUNC_ENTRY( _name, _line )	
#endif
#define YY_STACKTRACE_LINE( _line )
#endif

#define FUNCTION_STATIC_USED( a, b )			if (!(a)) { a = (YYObjectBase*)YYGML_GetStaticObject( b.val ); }
#define YYASSET_REF( a )						YYRValue( a, true)

//#if defined(YYLLVM)
// from math.h
YYCEXTERN double yyfabs(double _val);
YYCEXTERN double yyfdiv(int64 _lhs, int64 _rhs);
YYCEXTERN double yyfdiv(const RValue& _lhs, const RValue& _rhs);
YYCEXTERN double yyfdiv(int64 _lhs, const RValue& _rhs);
YYCEXTERN double yyfdiv(const RValue& _lhs, int64 _rhs);
//#endif

class YYObjectBase;
class CInstance;
class IBuffer;
struct RValue;
struct YYRValue;

struct SWithIterator
{
	YYObjectBase* pOriginalSelf;
	YYObjectBase* pOriginalOther;

	YYObjectBase** ppBufferBase;
	YYObjectBase** ppCurrent;

	~SWithIterator()
	{
		if (ppBufferBase != NULL) {
			YYFree(ppBufferBase);
			ppBufferBase = NULL;
		} // end if
	}
};
typedef void(*GML_Call)(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg);

extern YYObjectBase** g_ContextStack;
extern int g_ContextStackTop;

YYCEXTERN	void	NOTHROW_ATTR PushContextStack(YYObjectBase* _pObj);
YYCEXTERN	void	NOTHROW_ATTR PopContextStack( int num=1 );
YYCEXTERN	int		YYCompareVal(const RValue& val1, const RValue& val2, double _prec, bool throw_error);
YYCEXTERN	void	YYDuplicateMultiply(YYRValue* _ans, const YYRValue& rhs);
YYCEXTERN	bool  Variable_GetBuiltIn_Direct(YYObjectBase* inst, int var_ind, int array_ind, RValue *val);
YYCEXTERN	bool  Variable_GetValue_Direct(YYObjectBase* inst, int var_ind, int array_ind, RValue *val, bool fPrepareArray = false, bool fPartOfPop = false);
YYCEXTERN	bool  Variable_SetBuiltIn_Direct(YYObjectBase* inst, int var_ind, int array_ind, RValue *val);
YYCEXTERN	bool  Variable_SetValue_Direct(YYObjectBase* inst, int var_ind, int array_ind, RValue *val);
YYCEXTERN	void	Variable_Global_SetVar(int var_ind, int arr_ind, RValue  * val);
YYCEXTERN	bool	Variable_Global_GetVar(int var_ind, int arr_ind, RValue * val, bool fPrepareArray = false, bool fPartOfPop = false);
YYCEXTERN	void  	YYGML_ErrCheck_Variable_GetValue(int obj_ind, int var_ind, int array_ind, RValue *res);
YYCEXTERN	bool  	YYGML_Variable_GetValue(int obj_ind, int var_ind, int array_ind, RValue *res, bool fPrepareArray = false, bool fPartOfPop = false);
YYCEXTERN	bool  	YYGML_Variable_SetValue(int obj_ind, int var_ind, int array_ind, RValue *res);
//YYCEXTERN	bool  	Variable_GetValue(int obj_ind, int var_ind, int array_ind, RValue *res, bool fPrepareArray=false);
YYCEXTERN	bool  	Variable_SetValue(int obj_ind, int var_ind, int array_ind, RValue *res);
YYCEXTERN	bool	YYGML_Variable_GetValue(const YYRValue& _val, int var_ind, int array_ind, RValue *res, bool fPrepareArray = false, bool fPartOfPop = false);
YYCEXTERN	bool	YYGML_Variable_SetValue(const YYRValue& _val, int var_ind, int array_ind, RValue *res);
YYCEXTERN	bool  	Variable_SetValue(const YYRValue& _val, int var_ind, int array_ind, RValue *res);
YYCEXTERN	void  	YYGML_ErrCheck_Variable_GetValue(const YYRValue& _val, int var_ind, int array_ind, RValue *res, CInstance* _self, CInstance* _other);
YYCEXTERN	CInstance*  	YYGML_FindInstance(int _ind);
YYCEXTERN	void YYGML_Check( void );

YYCEXTERN YYRValue* ARRAY_LVAL_RValue(YYRValue* pV, int _index);
//YYCEXTERN void FREE_RValue( RValue* p );
FORCEINLINE void FREE_RValue__Pre(RValue* p);
#define FREE_RValue(rvp)     do { RValue *__p = (rvp); if (YYFree_valid_vkind(__p->kind)) { FREE_RValue__Pre(__p); } __p->ptr = NULL; __p->flags = 0; __p->kind = VALUE_UNDEFINED; } while (0);

//YYCEXTERN void COPY_RValue( RValue* pD, const RValue* pS);
YYCEXTERN void YYError(const char* _error, ...);
YYCEXTERN void YYprintf(const char* _error, ...);
YYCEXTERN char* yyitoa(int _n, char* _pDest, int _radix);
YYCEXTERN void STRING_RValue(char** _ppCurrent, char** _pBase, int* _pMaxLen, const RValue* _pV);
YYCEXTERN int64 INT64_RValue(const RValue* _pV);
YYCEXTERN int32 INT32_RValue(const RValue* _pV);
YYCEXTERN bool BOOL_RValue(const RValue* _pV);
FORCEINLINE bool BOOL_RValue(const YYRValue& _v) FORCEINLINE_ATTR;
FORCEINLINE bool BOOL_RValue(const YYRValue& _v) { return BOOL_RValue((const RValue*)&_v); }
YYCEXTERN double REAL_RValue_Ex(const RValue* _pV);

FORCEINLINE double REAL_RValue(const RValue *_pV);

YYCEXTERN void* PTR_RValue(const RValue* _pV);

// this will need to be a get / set
extern double g_GMLMathEpsilon;
extern int64 g_CurrentArrayOwner;
extern bool g_fCopyOnWriteEnabled;
extern YYObjectBase* g_pGetRValueContainer;





YYCEXTERN int YYGML_NewWithIterator(SWithIterator* pIterator, YYObjectBase** ppSelf, YYObjectBase** ppOther, const YYRValue& val);
YYCEXTERN bool YYGML_WithIteratorNext(SWithIterator* pIterator, YYObjectBase** ppSelf, YYObjectBase** ppOther);
YYCEXTERN void YYGML_DeleteWithIterator(SWithIterator* pIterator, YYObjectBase** ppSelf, YYObjectBase** ppOther);

YYCEXTERN double YYGML_random(double _v);
YYCEXTERN double YYGML_irandom(int64 _range);
YYCEXTERN double YYGML_random_range(double _base, double _end);
YYCEXTERN double YYGML_irandom_range(int64 _base, int64 _end);

YYCEXTERN void YYGML_sprite_set_cache_size(int _sprite_index, int _cache_size);
YYCEXTERN bool YYGML_keyboard_check_direct(int _key);
YYCEXTERN char* YYGML_string(const RValue& _val);
YYCEXTERN char* YYGML_AddString(const char* _first, const char* _second);
YYCEXTERN void YYGML_AddString(YYRValue& _first, const YYRValue& _second);
YYCEXTERN YYRValue& YYGML_AddVar(const YYRValue& _first, const YYRValue& _second);

YYCEXTERN int YYGML_BUFFER_Write(int buffer_index, int value_type, const YYRValue &val);

YYCEXTERN double YYGML_StringByteAt(const char *string, int _index);



YYCEXTERN void YYGML_ini_open(const char* _pFilename);
YYCEXTERN char* YYGML_ini_close(void);
YYCEXTERN double YYGML_ini_read_real(const char* _pSection, const char* _pKey, double _default);
YYCEXTERN void YYGML_ini_write_real(const char* _pSection, const char* _pKey, double _value);
YYCEXTERN bool YYGML_place_free(CInstance* _self, float _x, float _y);
YYCEXTERN void YYGML_move_snap(CInstance* _self, float _x, float _y);
YYCEXTERN void YYGML_motion_set(CInstance* pSelf, float _direction, float _speed);

YYCEXTERN void YYGML_instance_activate_object(CInstance* _self, CInstance* _other, int _objind);
YYCEXTERN bool YYGML_position_meeting(CInstance* _self, CInstance* _other, float _x, float _y, int ind);
YYCEXTERN int YYGML_instance_create(float _x, float _y, int _objind);
YYCEXTERN void YYGML_instance_change(CInstance* _self, int _objind, bool _performEvents);
YYCEXTERN void YYGML_instance_destroy(CInstance* _self, CInstance* _other, int _count, YYRValue** _args);
YYCEXTERN int YYGML_instance_number(CInstance* pSelf, CInstance* pOther, int _objectID);
YYCEXTERN void YYGML_event_inherited(CInstance* pSelf, CInstance* pOther);
YYCEXTERN void YYGML_event_perform(CInstance* pSelf, CInstance* pOther, int etype, int enumb);
YYCEXTERN void YYGML_event_perform_async(CInstance* pSelf, CInstance* pOther, int etype,int ds_map);
YYCEXTERN void YYGML_event_object(CInstance* pSelf, CInstance* pOther, int objId, int etype, int enumb);
YYCEXTERN void YYGML_event_user(CInstance* pSelf, CInstance* pOther, int enumb);
YYCEXTERN void YYGML_room_goto_next(void);
YYCEXTERN void YYGML_game_restart(void);
YYCEXTERN void YYGML_room_restart(void);
YYCEXTERN void YYGML_game_end(int _count = 0, YYRValue** _args = NULL);
YYCEXTERN void YYGML_window_set_caption(const char* _pStr);
YYCEXTERN int YYGML_make_color_rgb(int _red, int _green, int _blue);
YYCEXTERN int YYGML_color_get_red(int64 _color);
YYCEXTERN int YYGML_color_get_green(int64 _color);
YYCEXTERN int YYGML_color_get_blue(int64 _color);

YYCEXTERN void YYGML_draw_set_halign(int _type);
YYCEXTERN void YYGML_draw_set_valign(int _type);
YYCEXTERN void YYGML_draw_self(CInstance* _pSelf);
YYCEXTERN void YYGML_draw_text_transformed(float _x, float _y, const char* _pStr, float _xscale, float _yscale, float _angle);
YYCEXTERN void YYGML_draw_text_color(float _x, float _y, const char* _pStr, int _colTL, int _colTR, int _colBR, int _colBL, float _alpha);
YYCEXTERN void YYGML_draw_text_transformed_color(float _x, float _y, const char* _pStr, float _xscale, float _yscale, float _angle, int _colTL, int _colTR, int _colBR, int _colBL, float _alpha);
YYCEXTERN void YYGML_draw_text_ext_color(float _x, float _y, const char* _pStr, int _linesep, int _linewidth, int _colTL, int _colTR, int _colBR, int _colBL, float _alpha);
YYCEXTERN void YYGML_draw_text_ext_transformed_color(float _x, float _y, const char* _pStr, int _linesep, int _linewidth, float _xscale, float _yscale, float _angle, int _colTL, int _colTR, int _colBR, int _colBL, float _alpha);
YYCEXTERN void YYGML_draw_point(float _x1, float _y1);
YYCEXTERN void YYGML_draw_point_ext(float _x1, float _y1, unsigned int _c1);
YYCEXTERN void YYGML_draw_rectangle(float _x1, float _y1, float _x2, float _y2, bool _outline);
YYCEXTERN void YYGML_draw_sprite(CInstance* _pSelf, int _sprite_index, float _image_index, float _x, float _y);
YYCEXTERN void YYGML_draw_sprite_part(CInstance* _pSelf, int _sprite_index, float _image_index, float _left, float _top, float _width, float _height, float _x, float _y);
YYCEXTERN void YYGML_draw_sprite_ext(CInstance* _pSelf, int _sprite_index, float _image_index, float _x, float _y, float _xscale, float _yscale, float _angle, int _colour, float _alpha);
YYCEXTERN void YYGML_draw_set_font(int _font_index);
YYCEXTERN void YYGML_draw_text(float _x, float _y, const char* _string);
YYCEXTERN void YYGML_draw_set_colour(int _color);
YYCEXTERN void YYGML_draw_set_alpha(float _alpha);
YYCEXTERN bool YYGML_draw_surface_part_ext(int _id, float _xo, float _yo, float _w, float _h, float _x, float _y, float _xscale, float _yscale, int _color, float _alpha);
YYCEXTERN bool YYGML_surface_set_target(int _surfaceid, int _depth_id = -1);
YYCEXTERN int YYGML_surface_get_target();
YYCEXTERN int YYGML_surface_get_target_depth();
YYCEXTERN bool YYGML_surface_set_target_ext(int _stage, int _id, int _depth_id = -1);
YYCEXTERN int YYGML_surface_get_target_ext(int _stage);
YYCEXTERN bool YYGML_surface_reset_target(void);

YYCEXTERN void YYGML_shader_set(int _shader);
YYCEXTERN void YYGML_shader_reset();
YYCEXTERN void YYGML_shader_set_uniform_i(int _count, YYRValue** _args);
YYCEXTERN void YYGML_shader_set_uniform_f(int _count, YYRValue** _args);

//void YYGML_draw_sprite_ext();
YYCEXTERN bool YYGML_keyboard_check(int _key);
//void YYGML_keyboard_check_direct();
YYCEXTERN int YYGML_joystick_direction(int _joystick);
YYCEXTERN bool YYGML_joystick_check_button(int _joystick, int _button);
//void YYGML_random();
YYCEXTERN double YYGML_abs(double _val);
YYCEXTERN double YYGML_sign(double _val);
YYCEXTERN double YYGML_cos(double _val);
YYCEXTERN double YYGML_degtorad(double _val);
YYCEXTERN YYRValue& YYGML_choose(YYRValue& _result, int _count, YYRValue** _args);
YYCEXTERN YYRValue& YYGML_max(YYRValue& _result, int _count, YYRValue** _args);
YYCEXTERN YYRValue& YYGML_min(YYRValue& _result, int _count, YYRValue** _args);
YYCEXTERN float YYGML_point_direction(float _x1, float _y1, float _x2, float _y2);
YYCEXTERN float YYGML_lengthdir_x(float _deltaX, float _deltaY);
YYCEXTERN float YYGML_lengthdir_y(float _deltaX, float _deltaY);
YYCEXTERN void YYGML_sound_play(int _soundid);
YYCEXTERN void YYGML_sound_stop(int _soundid);
YYCEXTERN void YYGML_show_debug_message(const YYRValue& _val);

// common functions used for models in st00pid scripts
YYCEXTERN void YYGML_vertex_normal(int _buffer, float _x, float _y, float _z);
YYCEXTERN void YYGML_vertex_position(int _buffer, float _x, float _y);
YYCEXTERN void YYGML_vertex_position_3d(int _buffer, float _x, float _y, float _z);
YYCEXTERN void YYGML_vertex_colour(int _buffer, int _col, float _alpha);
YYCEXTERN void YYGML_vertex_texcoord(int _buffer, float _u, float _v);
YYCEXTERN void YYGML_vertex_argb(int _buffer, unsigned int _col);
YYCEXTERN void YYGML_vertex_end(int _buffer);
YYCEXTERN void YYGML_vertex_begin(int _buffer, int _format);
YYCEXTERN void YYGML_vertex_float1(int _buffer, float _x);
YYCEXTERN void YYGML_vertex_float2(int _buffer, float _x, float _y);
YYCEXTERN void YYGML_vertex_float3(int _buffer, float _x, float _y, float _z);
YYCEXTERN void YYGML_vertex_float4(int _buffer, float _x, float _y, float _z, float _w);
YYCEXTERN void YYGML_vertex_ubyte4(int _buffer, int _x, int _y, int _z, int _w);

YYCEXTERN void YYGML_Vertex_Submit(int buffer_index, int prim_type, const YYRValue& _value, int _offset=0, int64 _num=-1);
YYCEXTERN double  YYGML_clamp(double v, double a, double b);
YYCEXTERN YYRValue& YYGML_sprite_get_texture(YYRValue& _result, int sprite, int subimg);
YYCEXTERN void YYGML_SetMatrix(int kind, const YYRValue& _array);
YYCEXTERN int YYGML_shader_current();


YYCEXTERN YYRValue& YYGML_ds_grid_create(YYRValue& _result, int _width, int _height);
YYCEXTERN void YYGML_ds_grid_set(int _index, int _x, int _y, const YYRValue& _val);
YYCEXTERN YYRValue& YYGML_ds_grid_get(YYRValue& _result, int _index, int _x, int _y);
YYCEXTERN YYRValue& YYGML_array_get(YYRValue& _result, const YYRValue& _arg0, int _index, bool _fPrepareArray=false);
YYCEXTERN void YYGML_array_set(const YYRValue& _arg0, int _index, const YYRValue& _value);
YYCEXTERN void YYGML_array_set_2D(const YYRValue& _arg0, int _index1, int _index2, const YYRValue& _value);
#if defined(YYLLVM_SEP_DLL)
YYCEXTERN void YYGML_array_set_owner(int64 _owner);
#else
extern int64 g_CurrentArrayOwner;
#ifdef YYLLVM_COPY_ON_WRITE
FORCEINLINE void NOTHROW_ATTR YYGML_array_set_owner(int64 _owner) FORCEINLINE_ATTR;
FORCEINLINE void NOTHROW_ATTR YYGML_array_set_owner(int64 _owner)
{
#ifdef YYLLVM_COPY_ON_WRITE
	g_CurrentArrayOwner = _owner;
#endif
}
#else
#define YYGML_array_set_owner(a)		((void)0)
#endif
#endif

YYCEXTERN YYRValue& YYGML_ds_stack_create(YYRValue& _result);
YYCEXTERN void YYGML_ds_stack_push(int _count, YYRValue** _args);
//YYCEXTERN YYRValue& YYGML_ds_stack_pop(int _index);
YYCEXTERN int YYGML_ds_map_add(int _index, const YYRValue& _key, const YYRValue& _val);
YYCEXTERN void YYGML_variable_struct_set(CInstance* _pSelf, CInstance* _pOther, YYRValue& _obj, const YYRValue& _key, const YYRValue& _value);
YYCEXTERN void YYGML_variable_struct_remove(CInstance* _pSelf, CInstance* _pOther, const YYRValue& _obj, const YYRValue& _key);

// method variables
typedef YYRValue& (*PFUNC_YYGMLScript)( CInstance* pSelf, CInstance* pOther, YYRValue& _result, int _count,  YYRValue** _args  );
YYCEXTERN YYRValue& YYGML_CallMethod( CInstance* _pSelf, CInstance* _pOther, YYRValue& _result, int _argc, const YYRValue& _method, YYRValue** _args );
YYCEXTERN YYRValue& YYGML_method( CInstance* _pSelf, YYRValue& _result, PFUNC_YYGMLScript _func );
YYCEXTERN void YYGML_SetObjectConstructorName(CInstance* pSelf, const char* _pClass);
YYCEXTERN CInstance* YYGML_GetStaticObject(int _scriptIndex);
YYCEXTERN CInstance* YYGML_GetStaticObject(CInstance* _pSelf);
YYCEXTERN void YYGML_PrepareInheritanceCall();
YYCEXTERN void YYGML_SetStaticObject(CInstance* _pSelf, CInstance* _pStatic);
YYCEXTERN YYObjectBase* YYGML_GetObject(CInstance* selfinst, CInstance* otherinst, int _id);
YYCEXTERN void YYGML_CopyStatic(CInstance* _pSelf, YYObjectBase* _pDest, YYObjectBase* _pSource);

// This function routes any unknown functions to the correct destination
YYCEXTERN YYRValue& YYGML_CallLegacyFunction(CInstance* _pSelf, CInstance* _pOther, YYRValue& _result, int _argc, int _id, YYRValue** _args);
YYCEXTERN YYRValue& YYGML_CallScriptFunction(CInstance* _pSelf, CInstance* _pOther, YYRValue& _result, int _argc, int _id, YYRValue** _args);
YYCEXTERN YYRValue& YYGML_CallExtensionFunction(CInstance* _pSelf, CInstance* _pOther, YYRValue& _result, int _argc, int _id, YYRValue** _args);
inline void YYOpError(const char* pOp, const YYRValue* _lhs, const YYRValue* _rhs);


struct YYVAR
{
	const char* pName;
	int val;
};

#ifndef NULL
#define NULL	0
#endif

#if defined(YYLLVM) && !defined(__defined_YYString__)
#define __defined_YYString__
struct YYString
{
	const char* pStr;

	// default constructor
	//YYString() {
	//	pStr = NULL;
	//} // end YYString

	YYString(const char* _pMessage) {
		pStr = YYStrDup(_pMessage);
	} // end _pMessage

	  // copy constructor
	  //YYString( const YYString& _s ) {
	  //	pStr = YYStrDup( _s.pStr );
	  //} // end YYString

	  // destructor
	~YYString() {
		if (pStr != NULL) {
			YYStrFree(pStr);
			pStr = NULL;
		} // end if
	} // end destructor

	  // cast operator to char*
	operator char* () {
		return (char*)pStr;
	} // end char*

	YYString& operator=(const YYString& _s) {
		pStr = YYStrDup(_s.pStr);
		return *this;
	} // end operator=

	YYString& operator=(const char* _pMessage) {
		pStr = YYStrDup(_pMessage);
		return *this;
	} // end operator=

	  // handle +=
	YYString& operator+=(const YYString& rhs) {
		char* pNew = (char*)YYGML_AddString(pStr, rhs.pStr);
		YYFree((void*)pNew);
		pStr = pNew;
		return *this;
	} // end operator+=
};
#else
typedef char* YYString;
#endif

#if !defined(__defined_RValue_consts__)
enum
{
	OBJECT_KIND_YYOBJECTBASE = 0,
	OBJECT_KIND_CINSTANCE,
	OBJECT_KIND_ACCESSOR,
	OBJECT_KIND_SCRIPTREF,
	OBJECT_KIND_PROPERTY,
	OBJECT_KIND_ARRAY,
	OBJECT_KIND_WEAKREF,

	OBJECT_KIND_CONTAINER,

	OBJECT_KIND_SEQUENCE,
	OBJECT_KIND_SEQUENCEINSTANCE,
	OBJECT_KIND_SEQUENCETRACK,
	OBJECT_KIND_SEQUENCECURVE,
	OBJECT_KIND_SEQUENCECURVECHANNEL,
	OBJECT_KIND_SEQUENCECURVEPOINT,
	OBJECT_KIND_SEQUENCEKEYFRAMESTORE,
	OBJECT_KIND_SEQUENCEKEYFRAME,
	OBJECT_KIND_SEQUENCEKEYFRAMEDATA,
	OBJECT_KIND_SEQUENCEEVALTREE,
	OBJECT_KIND_SEQUENCEEVALNODE,
	OBJECT_KIND_SEQUENCEEVENT,

	OBJECT_KIND_NINESLICE,

	OBJECT_KIND_FILTERHOST,
	OBJECT_KIND_EFFECTINSTANCE,

	OBJECT_KIND_SKELETON_SKIN,

	OBJECT_KIND_SWITCH_NPLN_USERCONTEXT,
	OBJECT_KIND_SWITCH_NPLN_SESSION,

	OBJECT_KIND_AUDIOBUS,
	OBJECT_KIND_AUDIOEFFECT,

	OBJECT_KIND_ZIPFILE,
	OBJECT_KIND_PROTOTYPE,

	OBJECT_KIND_MAX
};

const int YYVAR_ACCESSOR_GET = (0);
const int YYVAR_ACCESSOR_SET = (1);

const int YYVAR_PROPERTY_THIS = (0);
const int YYVAR_PROPERTY_GET = (1);
const int YYVAR_PROPERTY_SET = (2);

#include "YYRValue.h"

YYCEXTERN int Array_GetRef(RefDynamicArrayOfRValue* _pArray);
YYCEXTERN int Array_DecRef(RefDynamicArrayOfRValue* _pArray);
YYCEXTERN int Array_IncRef(RefDynamicArrayOfRValue* _pArray);
YYCEXTERN void Array_SetOwner(RefDynamicArrayOfRValue* _pArray);
YYCEXTERN int Array_GetLength(RefDynamicArrayOfRValue* _pArray);
YYCEXTERN RValue& Array_GetEntry(RefDynamicArrayOfRValue* _pArray, int _index);

#endif

class RefDynamicArrayOfRValue
{
public:
	RefDynamicArrayOfRValue()
	{
		pObjThing = NULL;
		refcount = 0;
		flags = 0;
		pArray = NULL;
		owner = 0;
		visited = 0;
		length = 0;
	}
	YYObjectBase* pObjThing;
	RValue* pArray;
	// copy on write only
	int64 owner;
	int	refcount;
	//
	int flags;
	int visited;
	int length;
};

FORCEINLINE void	NOTHROW_ATTR PushContextStack(RefDynamicArrayOfRValue* _p) FORCEINLINE_ATTR;
FORCEINLINE void	NOTHROW_ATTR PushContextStack(RefDynamicArrayOfRValue* _p)
{
	PushContextStack(_p->pObjThing);
}


#if defined(YYLLVM) && !defined(__defined_CInstanceBase__)
#define __defined_CInstanceBase__
class CInstanceBase
{
public:
	YYRValue*		yyvars;
	virtual ~CInstanceBase() {};
#if _MSC_VER != 1500
	YYRValue& GetYYVarRef(int index) {
		return InternalGetYYVarRef(index);
	} // end GetYYVarRef
	virtual  YYRValue& InternalGetYYVarRef(int index) = 0;
	YYRValue& GetYYVarRefL(int index) {
		return InternalGetYYVarRefL(index);
	} // end GetYYVarRef
	virtual  YYRValue& InternalGetYYVarRefL(int index) = 0;
#else
	virtual  YYRValue& GetYYVarRef(int index) = 0;
#endif
};
#endif

WEAK_ATTR RValue g_undefinedRV = { 0, 0, VALUE_UNDEFINED };
WEAK_ATTR DValue gs_constTrue = { 1.0, 0, VALUE_BOOL };
WEAK_ATTR RValue gs_constFalse = { 0, 0, VALUE_BOOL };
#define g_undefined (*((YYRValue*)&g_undefinedRV))

FORCEINLINE NOTHROW_ATTR YYRValue*  __YYGetArg__(YYRValue** _args, int _i, int _r) FORCEINLINE_ATTR;
FORCEINLINE NOTHROW_ATTR YYRValue* __YYGetArg__(YYRValue** _args, int _i, int _r)
{
	return (_i < _r) ? _args[_i] : (YYRValue*)&g_undefinedRV;
} // end __YYDoCheckRange__
#define YY_GET_ARG( a, i, r)	(__YYGetArg__( (a), (i), (r)))

FORCEINLINE bool NOTHROW_ATTR YYFree_valid_vkind(unsigned x) PURE_ATTR FORCEINLINE_ATTR;
FORCEINLINE bool NOTHROW_ATTR YYFree_valid_vkind(unsigned x) {

	x &= 0x1f;
	return (((1 << VALUE_STRING) | (1 << VALUE_OBJECT) | (1 << VALUE_ARRAY)) & (1 << x)) != 0;
}

FORCEINLINE bool NOTHROW_ATTR IsGCKind(int _kind) PURE_ATTR FORCEINLINE_ATTR;
FORCEINLINE bool NOTHROW_ATTR IsGCKind(int _kind)
{
	_kind &= 0x1f;
	return (((1 << VALUE_ACCESSOR) | (1 << VALUE_OBJECT) | (1 << VALUE_ARRAY)) & (1 << _kind)) != 0;
}

FORCEINLINE bool NOTHROW_ATTR YYGML_IsNullish(const RValue& _val) PURE_ATTR FORCEINLINE_ATTR;
FORCEINLINE bool NOTHROW_ATTR YYGML_IsNullish(const RValue& _val)
{
	bool fIsNullish = false;
	switch (_val.kind & MASK_KIND_RVALUE) {
	case VALUE_UNDEFINED:
		fIsNullish = true;
		break;
	case VALUE_PTR:
		fIsNullish = (_val.ptr == NULL);
		break;
	} // end switch
	return fIsNullish;
} // end YYGML_IsNullish

YYCEXTERN void DeterminePotentialRoot(YYObjectBase* _pContainer, YYObjectBase* _pObj);
FORCEINLINE void DeterminePotentialRoot(YYObjectBase* _pContainer, RefDynamicArrayOfRValue* _pObj) FORCEINLINE_ATTR;
FORCEINLINE void DeterminePotentialRoot(YYObjectBase* _pContainer, RefDynamicArrayOfRValue* _pObj)
{
	DeterminePotentialRoot(_pContainer, _pObj->pObjThing);
}

YYCEXTERN void GCCollectHint(YYObjectBase* _pObj);

YYCEXTERN NOTHROW_ATTR YYObjectBase* GetContextStackTop();

// #############################################################################################
/// Function:<summary>
///          	Copy an RValue, taking a reference to the array when required...
///          </summary>
///
/// In:		<param name="_pDest">Copying TO</param>
///			<param name="_pSource">Copying FROM</param>
///				
// #############################################################################################
NOINLINE_ATTR 
static void COPY_RValue_do__Post(RValue* _pDest, const RValue* _pSource)
{
	switch (_pSource->kind&MASK_KIND_RVALUE) {
	case VALUE_ARRAY:
		_pDest->pRefArray = _pSource->pRefArray;
		if (_pDest->pRefArray != NULL) {
			RefDynamicArrayOfRValue* pArray = _pDest->pRefArray;
			if (g_fCopyOnWriteEnabled) {
				++pArray->refcount;
				if (pArray->owner == 0L) pArray->owner = g_CurrentArrayOwner;
			} // end if
			YYObjectBase* pContainer = GetContextStackTop();
			DeterminePotentialRoot(pContainer, pArray);
		}
		break;
	case VALUE_STRING:
		_pDest->pRefString = RefString::assign(_pSource->pRefString);
		break;
	case VALUE_OBJECT:
	{
		_pDest->pObj = _pSource->pObj;

		if (_pSource->pObj != NULL)
		{
			//extern void MarkObjectAsCurrent(void* _pObj);
			//MarkObjectAsCurrent(_pDest->pObj);			

			YYObjectBase* pContainer = GetContextStackTop();
			DeterminePotentialRoot(pContainer, _pSource->pObj);
		}
	}
	break;
	}
}

FORCEINLINE void COPY_RValue__Post_nodest(RValue* _pDest, const RValue* _pSource) FORCEINLINE_ATTR;
FORCEINLINE void COPY_RValue__Post_nodest(RValue* _pDest, const RValue* _pSource)
{
	unsigned int skind = _pSource->kind;

	_pDest->kind = _pSource->kind;
	_pDest->flags = _pSource->flags;

	if (YYFree_valid_vkind(skind))
		COPY_RValue_do__Post(_pDest, _pSource);
	else {
		_pDest->v64 = _pSource->v64;
	} // end else

	YYASSUME(_pSource->kind == skind);
	YYASSUME(_pDest->kind == skind);
	YYASSUME(_pDest->flags == sflags);
}

FORCEINLINE void COPY_RValue__Post(RValue* _pDest, const RValue* _pSource) FORCEINLINE_ATTR;
FORCEINLINE void COPY_RValue__Post(RValue* _pDest, const RValue* _pSource)
{
	unsigned int skind = _pSource->kind;
	unsigned int dkind = _pDest->kind;

	_pDest->kind = _pSource->kind;
	_pDest->flags = _pSource->flags;

	if (YYFree_valid_vkind(skind))
		COPY_RValue_do__Post(_pDest, _pSource);
	else {
		if ((skind & MASK_KIND_RVALUE) == VALUE_UNDEFINED)
		{
			dkind &= MASK_KIND_RVALUE;

			if (dkind == VALUE_ARRAY)
			{
				if (_pDest->pRefArray != NULL)
				{
					GCCollectHint(_pDest->pRefArray->pObjThing);
				}
			}
			else if (IsGCKind((int)(dkind)))
			{
				GCCollectHint(_pDest->pObj);
			}
		}

		_pDest->v64 = _pSource->v64;
	} // end else

	YYASSUME(_pSource->kind == skind);
	YYASSUME(_pDest->kind == skind);
	YYASSUME(_pDest->flags == sflags);
}

FORCEINLINE void COPY_RValue(RValue* _pDest, const RValue* _pSource)FORCEINLINE_ATTR;
FORCEINLINE void COPY_RValue(RValue* _pDest, const RValue* _pSource)
{
	if (YYFree_valid_vkind(_pDest->kind)) {
		FREE_RValue__Pre(_pDest);
	} // end if

	COPY_RValue__Post(_pDest, _pSource);
}

class COwnedObject
{
public:
	virtual ~COwnedObject() {};
};

FORCEINLINE double REAL_RValue(const RValue* p)
{
	return ((p->kind&MASK_KIND_RVALUE) == VALUE_REAL ? p->val : REAL_RValue_Ex(p));
}

NOINLINE_ATTR void FREE_RValue__Pre(RValue* p)
{
	switch (p->kind&MASK_KIND_RVALUE) {
	case VALUE_PTR:
		if (p->flags & ERV_Owned)
		{
			delete (COwnedObject*)p->ptr;
		}
		break;
	case VALUE_STRING:
		p->pRefString = RefString::remove(p->pRefString);
		break;
	case VALUE_ARRAY:
		if (g_fCopyOnWriteEnabled) {
			RefDynamicArrayOfRValue* pArray = p->pRefArray;
			if (pArray != NULL) {

				--pArray->refcount;
				if (pArray->owner == 0) pArray->owner = g_CurrentArrayOwner;
				// Don't need to do anything else here - once the array is not referenced by anything it should get automatically garbage collected
			} // end if
		} // end if
		break;

	} // end switch
}

FORCEINLINE double yyfmod(double _lhs, double _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfmod(double _lhs, double _rhs)
{
	if (_rhs == 0) {
		YYError("Divide by zero");
	} // end if
	return fmod(_lhs, _rhs);
} // end yyfmod

FORCEINLINE double yyfmod(const RValue& _lhs, const RValue& _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfmod(const RValue& _lhs, const RValue& _rhs)
{
	return yyfmod(_lhs.asReal(), _rhs.asReal());
} // end yyfmod
FORCEINLINE double yyfmod(const RValue& _lhs, double _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfmod(const RValue& _lhs, double _rhs)
{
	return yyfmod(_lhs.asReal(), _rhs);
} // end yyfmod
FORCEINLINE double yyfmod(double _lhs, const RValue& _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfmod(double _lhs, const RValue& _rhs)
{
	return yyfmod(_lhs, _rhs.asReal());
} // end yyfmod
  //FORCEINLINE double yyfmod( int64 _lhs, int64 _rhs ) FORCEINLINE_ATTR;
  //FORCEINLINE double yyfmod( int64 _lhs, int64 _rhs ) 
  //{
  //	return (double)(_lhs % _rhs);
  //} // end yyfmod

FORCEINLINE double yyfdiv(int64 _lhs, int64 _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfdiv(int64 _lhs, int64 _rhs)
{
	if (_rhs == 0) {
		YYError("Divide by zero");
	} // end if
	return (double)((int64)_lhs / (int64)_rhs);
} // end yyfmod
FORCEINLINE double yyfdiv(const RValue& _lhs, const RValue& _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfdiv(const RValue&  _lhs, const RValue&  _rhs)
{
	return yyfdiv(_lhs.asInt64(), _rhs.asInt64());
} // end yyfmod
FORCEINLINE double yyfdiv(int64 _lhs, const RValue& _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfdiv(int64  _lhs, const RValue&  _rhs)
{
	return yyfdiv(_lhs, _rhs.asInt64());
} // end yyfmod
FORCEINLINE double yyfdiv(const RValue& _lhs, int64 _rhs) FORCEINLINE_ATTR;
FORCEINLINE double yyfdiv(const RValue&  _lhs, int64  _rhs)
{
	return yyfdiv(_lhs.asInt64(), _rhs);
} // end yyfmod

FORCEINLINE double yyfabs(double _val) FORCEINLINE_ATTR;
FORCEINLINE double yyfabs(double _val)
{
	return (_val < 0) ? -_val : _val;
}

#if defined(YYLLVM) && !defined(__defined_YYRValue__)
#define __defined_YYRValue__
#define VARIABLE_ARRAY_MAX_DIMENSION 32000
struct YYRValue : RValue
{
	// --------------------------------------------------------------------------------------------------
	// destructors
	// --------------------------------------------------------------------------------------------------
	~YYRValue() {
		__localFree();
		//FREE_RValue__Pre( this );
	} // end YYRValue

	void __localFree(void)
	{
		if (YYFree_valid_vkind(kind)) {
			FREE_RValue__Pre(this);
		} // end if
	} // end __localFree

	void __localCopy(const YYRValue& _v) {
		COPY_RValue__Post(this, &_v);
	} // end __localCopy

	void __localCopy_nodest(const YYRValue& _v) {
		COPY_RValue__Post_nodest(this, &_v);
	} // end __localCopy

	  // --------------------------------------------------------------------------------------------------
	  // constructors
	  // --------------------------------------------------------------------------------------------------
	  // default constructor
	YYRValue() {
		kind = VALUE_UNSET;
		ptr = NULL;
		YYASSUME(kind == VALUE_UNSET);
	} // end YYRValue

	YYRValue(const YYRValue& _v) {
		__localCopy_nodest(_v);
	} // end YYRValue

	YYRValue(double _val) {
		kind = VALUE_REAL;
		val = _val;
		YYASSUME(kind == VALUE_REAL);
	}

	YYRValue(bool _val) {
		kind = VALUE_BOOL;
		val = _val ? 1 : 0;
		YYASSUME(kind == VALUE_BOOL);
	}

	YYRValue(float _val) {
		kind = VALUE_REAL;
		val = _val;
		YYASSUME(kind == VALUE_REAL);
	}

	YYRValue(int _val) {
		kind = VALUE_REAL;
		val = _val;
		//YYASSUME(kind == VALUE_REAL);
	}

	YYRValue(long long _val) {
		kind = VALUE_INT64;
		v64 = _val;
		YYASSUME(kind == VALUE_INT64);
	}

	YYRValue(const char* _pStr) {
		YYSetString(this, _pStr);
		YYASSUME(kind == VALUE_STRING);
	}

	YYRValue(const char* _pStr, bool _dontCare) {

		YYConstString(this, _pStr);
		YYASSUME(kind == VALUE_STRING);
	}

	YYRValue(YYObjectBase* _p) {
		kind = VALUE_OBJECT;
		pObj = _p;
		YYASSUME(kind == VALUE_OBJECT);
	}

	YYRValue(PFUNC_YYGMLScript _pMethod, CInstance* _pSelf) {
		YYSetScriptRef(this, (PFUNC_YYGMLScript_Internal)_pMethod, (YYObjectBase*)_pSelf);
		YYASSUME(kind == VALUE_OBJECT);
	}

	YYRValue(int a, bool _ref)
	{
		int index = a & 0x00ffffff;
		int refType = ((a >> 24) & 0xff);
		if (refType == 14) {
			refType = REFID_INSTANCE;
		} // end if
		else
			refType |= REFCAT_RESOURCE;
		kind = VALUE_REF;
		v64 = MAKE_REF(refType, index);
	}

	// --------------------------------------------------------------------------------------------------
	// unary negate
	// --------------------------------------------------------------------------------------------------
	YYRValue operator-() {
		YYRValue ret;
		ret.kind = kind;
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL: kind = VALUE_REAL;
		case VALUE_REAL: ret.val = -val; break;
		case VALUE_INT32: ret.v32 = -v32; break;
		case VALUE_INT64: ret.v64 = -v64; break;
		default: YYError("Invalid type for negate operation\n"); break;
		} // end switch
		return ret;
	} // end operator-

	  // --------------------------------------------------------------------------------------------------
	  // assignment operators
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator=(const YYRValue& _v) {
		if (&_v != this) {
			DValue temp;
			memcpy(&temp, &_v, sizeof(YYRValue));
#ifdef YYLLVM_COPY_ON_WRITE
			bool fIsArray = ((temp.kind & MASK_KIND_RVALUE) == VALUE_ARRAY);
			if (fIsArray)  ++((RValue*)&temp)->pRefArray->refcount;
#endif
			__localFree();
#ifdef YYLLVM_COPY_ON_WRITE
			if (fIsArray) --((RValue*)&temp)->pRefArray->refcount;
#endif
			__localCopy(*(YYRValue*)&temp);
		} // end if
		return *this;
	} // end operator=

	YYRValue& operator=(const YYRValue* _pV) {
		if (_pV != this) {
			DValue temp;
			memcpy(&temp, _pV, sizeof(YYRValue));
#ifdef YYLLVM_COPY_ON_WRITE
			bool fIsArray = ((temp.kind & MASK_KIND_RVALUE) == VALUE_ARRAY);
			if (fIsArray)  ++((RValue*)&temp)->pRefArray->refcount;
#endif
			__localFree();
#ifdef YYLLVM_COPY_ON_WRITE
			if (fIsArray) --((RValue*)&temp)->pRefArray->refcount;
#endif
			__localCopy(*(YYRValue*)&temp);
		} // end if
		return *this;
	} // end operator=

	YYRValue& operator=(double _v) {
		__localFree();
		kind = VALUE_REAL;
		val = _v;
		YYASSUME(kind == VALUE_REAL);
		return *this;
	} // end operator=

	YYRValue& operator=(float _v) {
		__localFree();
		kind = VALUE_REAL;
		val = _v;
		YYASSUME(kind == VALUE_REAL);
		return *this;
	} // end operator=

	YYRValue& operator=(int _v) {
		__localFree();
		kind = VALUE_REAL;
		val = _v;
		YYASSUME(kind == VALUE_REAL);
		return *this;
	} // end operator=

	YYRValue& operator=(long _v) {
		__localFree();
		kind = VALUE_REAL;
		val = _v;
		YYASSUME(kind == VALUE_REAL);
		return *this;
	} // end operator=

	YYRValue& operator=(long long _v) {
		__localFree();
		kind = VALUE_INT64;
		v64 = _v;
		YYASSUME(kind == VALUE_INT64);
		return *this;
	} // end operator=

	YYRValue& operator=(const char* _pStr) {
		__localFree();
		YYCreateString(this, _pStr);
		YYASSUME(kind == VALUE_STRING);
		return *this;
	} // end operator=

	YYRValue& operator=(bool _v) {
		__localFree();
		kind = VALUE_BOOL;
		val = _v ? 1.0 : 0.0;
		YYASSUME(kind == VALUE_BOOL);
		return *this;
	} // end operator=


	  // --------------------------------------------------------------------------------------------------
	  // cast operators
	  // --------------------------------------------------------------------------------------------------
	operator bool() const {
		return BOOL_RValue(this);
	} // end cast operator

	operator char*() const {
		return ((kind & MASK_KIND_RVALUE) == VALUE_STRING) ? ((pRefString != NULL) ? (char*)pRefString->get() : NULL) : NULL;
	} // end cast operator

	operator const char*() const {
		return ((kind & MASK_KIND_RVALUE) == VALUE_STRING) ? ((pRefString != NULL) ? pRefString->get() : NULL) : NULL;
	} // end cast operator

	operator double() const {
		return REAL_RValue(this);
	} // end cast operator

	operator float() const {
		return (float)REAL_RValue(this);
	} // end cast operator

	operator int() const {
		return INT32_RValue(this);
	} // end cast operator

	operator unsigned int() const {
		return (unsigned int)INT32_RValue(this);
	} // end cast operator

	operator long() const {
		return INT32_RValue(this);
	} // end cast operator

	operator unsigned long() const {
		return (unsigned long)INT32_RValue(this);
	} // end cast operator

	operator long long() const {
		return INT64_RValue(this);
	} // end cast operator	

	operator unsigned long long() const {
		return (unsigned long long)INT64_RValue(this);
	} // end cast operator	

	  // --------------------------------------------------------------------------------------------------
	  // operator  post ++ and pre ++
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator++() {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
			// fall through
		case VALUE_REAL:
			++val;
			break;
		case VALUE_INT32:
			++v32;
			break;
		case VALUE_INT64:
			++v64;
			break;
		case VALUE_STRING:
		{
			double v = asReal();
			val = ++v;
			kind = VALUE_REAL;
		}
		break;
		default:
			YYOpError("++", this, this);
			break;
		} // end switch
		return *this;
	} // end operator++
	YYRValue operator++(int) {
		YYRValue tmp(*this);
		operator++();
		return tmp;
	} // end operator++

	  // --------------------------------------------------------------------------------------------------
	  // operator  post -- and pre --
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator--() {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			--val;
			break;
		case VALUE_INT32:
			--v32;
			break;
		case VALUE_INT64:
			--v64;
			break;
		case VALUE_STRING:
		{
			double v = asReal();
			val = --v;
			kind = VALUE_REAL;
		}
		break;
		default:
			YYOpError("--", this, this);
			break;
		} // end switch
		return *this;
	} // end operator--
	YYRValue operator--(int) {
		YYRValue tmp(*this);
		operator--();
		return tmp;
	} // end operator--

	  // --------------------------------------------------------------------------------------------------
	  // operator + and +=
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator+=(const YYRValue& rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			val += REAL_RValue(&rhs);
			break;
		case VALUE_INT32:
			if ((rhs.kind & MASK_KIND_RVALUE) == VALUE_REAL ) {
				val = (double)v32 + rhs.val;
				kind = VALUE_REAL;
			}
			else if((rhs.kind & MASK_KIND_RVALUE) == VALUE_INT64)
			{
				v64 = v32 + rhs.v64;
				kind = VALUE_INT64;
			}
			else {
				v32 += INT32_RValue(&rhs);
			} // end else
			break;
		case VALUE_INT64:
			if ((rhs.kind & MASK_KIND_RVALUE) == VALUE_REAL) {
				val = (double)v64 + rhs.val;
				kind = VALUE_REAL;
			}
			else {
				v64 += INT64_RValue(&rhs);
			} // end else
			break;
		case VALUE_STRING:
		{
			if ((rhs.kind & MASK_KIND_RVALUE) == VALUE_STRING) {
				YYGML_AddString(*this, rhs);
			} // end if
			else {
				YYError("unable to add a number to string");
			} // end else
			break;
		} // end block
		default:
			YYOpError("+=", this, &rhs);
			break;
		} // end switch
		return *this;
	} // end operator+=
	YYRValue& operator+=(const double rhs) {
		switch (kind) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			val += rhs;
			break;
		case VALUE_INT32:
			val = (double)v32 + rhs;
			kind = VALUE_REAL;
			break;
		case VALUE_INT64:
			val = (double)v64 + rhs;
			kind = VALUE_REAL;
			break;
		case VALUE_STRING:
			// if string then concatenate, if not then convert to string
			YYError("unable to add a number to string");
			break;
		default:
		{
			YYRValue yyrhs(rhs);
			YYOpError("+=", this, &yyrhs);
		} // end block
		break;
		} // end switch
		return *this;
	} // end operator+=
	YYRValue& operator+=(const int rhs) {
		switch (kind) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			//if (rhs.kind != VALUE_REAL){ error message }
			val += rhs;
			break;
		case VALUE_INT32:
			v32 += rhs;
			break;
		case VALUE_INT64:
			v64 += rhs;
			break;
		case VALUE_STRING:
			// if string then concatenate, if not then convert to string
			YYError("unable to add a number to string");
			break;
		default:
		{
			YYRValue yyrhs(rhs);
			YYOpError("+=", this, &yyrhs);
		} // end block
		break;
		} // end switch
		return *this;
	} // end operator+=
	YYRValue& operator+=(const long long rhs) {
		switch (kind) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			//if (rhs.kind != VALUE_REAL){ error message }
			val += rhs;
			break;
		case VALUE_INT32:
			v32 += (int32)rhs;
			break;
		case VALUE_INT64:
			v64 += rhs;
			break;
		case VALUE_STRING:
			// if string then concatenate, if not then convert to string
			YYError("unable to add a number to string");
			break;
		default:
		{
			YYRValue yyrhs(rhs);
			YYOpError("+=", this, &yyrhs);
		} // end block
		break;
		} // end switch
		return *this;
	} // end operator+=
	YYRValue& operator+=(const char* rhs) {
		switch (kind) {
		case VALUE_BOOL:
		case VALUE_INT32:
		case VALUE_INT64:
		case VALUE_REAL:
			*this += YYRValue(rhs);
			break;
		case VALUE_STRING:
		{
			// if string then concatenate, if not then convert to string
			const char* pFirst = (pRefString != NULL) ? pRefString->get() : NULL;
			char* pNew = (char*)YYGML_AddString(pFirst, rhs);
			YYCreateString(this, pNew);
			YYFree(pNew);
		} // end block
		break;
		default:
		{
			YYRValue yyrhs(rhs);
			YYOpError("+=", this, &yyrhs);
		} // end block
		break;
		} // end switch
		return *this;
	} // end operator+=	
	friend YYRValue operator+(const YYRValue& lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(const YYRValue& lhs, const char* rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(const YYRValue& lhs, double rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(double lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(const YYRValue& lhs, int rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(int lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(const YYRValue& lhs, long long rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(long long lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(const YYRValue& lhs, float rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(float lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+
	friend YYRValue operator+(const char* lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} // end operator+


	  // --------------------------------------------------------------------------------------------------
	  // operator - and -=
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator-=(const YYRValue& rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			val -= REAL_RValue(&rhs);
			break;
		case VALUE_INT32:
			if ((rhs.kind & MASK_KIND_RVALUE) == VALUE_REAL) {
				val = (double)v32 - rhs.val;
				kind = VALUE_REAL;
			}
			else if ((rhs.kind & MASK_KIND_RVALUE) == VALUE_INT64)
			{
				v64 = v32 - rhs.v64;
				kind = VALUE_INT64;
			}
			else {
				v32 -= INT32_RValue(&rhs);
			} // end else
			break;
		case VALUE_INT64:
			if ((rhs.kind & MASK_KIND_RVALUE) == VALUE_REAL) {
				val = (double)v64 - rhs.val;
				kind = VALUE_REAL;
			}
			else {
				v64 -= INT64_RValue(&rhs);
			} // end else
			break;
		case VALUE_STRING:
		{
			double db = REAL_RValue(this);
			kind = VALUE_REAL;
			val = (db - REAL_RValue(&rhs));
		} // end block
		break;
		default:
			YYOpError("-=", this, &rhs);
			break;
		}
		return *this;
	} // end operator-=
	friend YYRValue operator-(const YYRValue& lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(const YYRValue& lhs, double rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(double lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(const YYRValue& lhs, int rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(int lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(const YYRValue& lhs, long long rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(long long lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(const YYRValue& lhs, float rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-
	friend YYRValue operator-(float lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} // end operator-


	  // --------------------------------------------------------------------------------------------------
	  // operator / and /=
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator/=(const YYRValue& rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			switch ((rhs.kind & MASK_KIND_RVALUE)) {
			case VALUE_PTR:
				YYOpError("/=", this, &rhs);
				break;
			default:
				val /= REAL_RValue(&rhs);
				break;
			} // end switch
			break;
		case VALUE_INT32:
			switch ((rhs.kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				if (rhs.v32 == 0) YYError("divide by zero");
				v32 /= rhs.v32;
				break;
			case VALUE_INT64:
				kind = VALUE_INT64;
				if (rhs.v64 == 0) YYError("divide by zero");
				v64 /= rhs.v64;
				break;
			default:
				kind = VALUE_REAL;
				val = ((double)v32 / REAL_RValue(&rhs));
				break;
			} // end switch
			break;
		case VALUE_INT64:
			switch ((rhs.kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				if (rhs.v32 == 0) YYError("divide by zero");
				v64 /= rhs.v32;
				break;
			case VALUE_INT64:
				if (rhs.v64 == 0) YYError("divide by zero");
				v64 /= rhs.v64;
				break;
			case VALUE_PTR:
				YYOpError("/=", this, &rhs);
				break;
			default:
				kind = VALUE_REAL;
				val = ((double)v64 / REAL_RValue(&rhs));
				break;
			} // end switch
			break;
		case VALUE_STRING:
		{
			double db = REAL_RValue(this);
			kind = VALUE_REAL;
			val = (db / REAL_RValue(&rhs));
		} // end block
		break;
		default:
			YYOpError("/=", this, &rhs);
			break;
		}
		return *this;

	}
	friend YYRValue operator/(const YYRValue& lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(const YYRValue& lhs, double rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(double lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(const YYRValue& lhs, int rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(int lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(const YYRValue& lhs, long long rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(long long lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(const YYRValue& lhs, float rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/
	friend YYRValue operator/(float lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} // end operator/

	  // --------------------------------------------------------------------------------------------------
	  // operator * and *=
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator*=(const YYRValue& rhs) {
		if (is_number() && ((rhs.kind & MASK_KIND_RVALUE) == VALUE_STRING)) {
			YYDuplicateMultiply(this, rhs);
		} // end if
		else {
			switch ((kind & MASK_KIND_RVALUE)) {
			case VALUE_BOOL:
				kind = VALUE_REAL;
			case VALUE_REAL:
				val *= REAL_RValue(&rhs);
				break;
			case VALUE_INT32:
				switch ((rhs.kind & MASK_KIND_RVALUE)) {
				case VALUE_INT32:
					v32 *= rhs.v32;
					break;
				case VALUE_INT64:
					kind = VALUE_INT64;
					v64 *= rhs.v64;
					break;
				default:
					kind = VALUE_REAL;
					val = ((double)v32 * REAL_RValue(&rhs));
					break;
				} // end switch
				break;
			case VALUE_INT64:
				switch ((rhs.kind & MASK_KIND_RVALUE)) {
				case VALUE_INT32:
					v64 *= rhs.v32;
					break;
				case VALUE_INT64:
					v64 *= rhs.v64;
					break;
					//	case VALUE_REAL:
					//		v64 *= INT64_RValue(&rhs);
					//		break;
				default:
					kind = VALUE_REAL;
					val = ((double)v64 * REAL_RValue(&rhs));
					break;
				} // end switch
				break;
			default:
				YYOpError("*=", this, &rhs);
				break;
			}
		} // end else
		return *this;
	}
	friend YYRValue operator*(const YYRValue& lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(const YYRValue& lhs, double rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(double lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(const YYRValue& lhs, int rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(int lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(const YYRValue& lhs, long long rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(long long lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(const YYRValue& lhs, float rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*
	friend YYRValue operator*(float lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} // end operator*

	  // --------------------------------------------------------------------------------------------------
	  // operator % and %=
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator%=(const YYRValue& rhs) {
		switch ((kind&MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
			switch ((rhs.kind & MASK_KIND_RVALUE)) {
			case VALUE_PTR:
				YYOpError("%=", this, &rhs);
				break;
			default:
			{
				double db = REAL_RValue(&rhs);
				if (db == 0) {
					YYError("unable to mod by 0");
				} // end if
				val = fmod(val, db);
			} // end block
			break;
			} // end switch
			break;
		case VALUE_INT32:
			switch ((rhs.kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				v32 %= rhs.v32;
				break;
			case VALUE_INT64:
				kind = VALUE_INT64;
				v64 %= rhs.v64;
				break;
			case VALUE_PTR:
				YYOpError("%=", this, &rhs);
				break;
			default:
				kind = VALUE_REAL;
				{
					double db = REAL_RValue(&rhs);
					if (db == 0) {
						YYError("unable to mod by 0");
					} // end if
					val = fmod((double)v32, db);
				} // end block
				break;
			} // end switch
			break;
		case VALUE_INT64:
			switch ((rhs.kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				v64 %= rhs.v32;
				break;
			case VALUE_INT64:
				v64 %= rhs.v64;
				break;
			case VALUE_PTR:
				YYOpError("%=", this, &rhs);
				break;
			default:
				kind = VALUE_REAL;
				{
					double db = REAL_RValue(&rhs);
					if (db == 0) {
						YYError("unable to mod by 0");
					} // end if
					val = fmod((double)v64, db);
				} // end block
				break;
			} // end switch
			break;
		case VALUE_STRING:
		{
			double db = REAL_RValue(&rhs);
			if (db == 0) {
				YYError("unable to mod by 0");
			} // end if
			double dbV = REAL_RValue(this);
			val = fmod(dbV, db);
			kind = VALUE_REAL;
		} // end block
		break;
		default:
			YYOpError("%=", this, &rhs);
			break;
		} // end if
		return *this;
	}
	friend YYRValue operator%(const YYRValue& lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(const YYRValue& lhs, double rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(double lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(const YYRValue& lhs, int rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(int lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(const YYRValue& lhs, long long rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(long long lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(const YYRValue& lhs, float rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%
	friend YYRValue operator%(float lhs, const YYRValue& rhs) {
		YYRValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} // end operator%

	  // --------------------------------------------------------------------------------------------------
	  // operator [] (used for array rvalue dereference) and () - used for array lvalue dereference
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator[](const int _index) const
	{
		YYRValue* pV = NULL;
		if (((kind & MASK_KIND_RVALUE) == VALUE_ARRAY) && (pRefArray != NULL)) {
#ifdef YYLLVM_COPY_ON_WRITE
			if (pRefArray->owner == 0L) pRefArray->owner = g_CurrentArrayOwner;
#endif

			// find the entry
			if ((_index >= 0) && (_index < pRefArray->length)) {
				pV = (YYRValue*)&pRefArray->pArray[_index];
			} // end if
			else {
				YYError("index out of bounds request %d maximum size is %d", _index, pRefArray->length);
			} // end if
		}
		/* else this is not an array */
		else {
			YYError("trying to index variable that is not an array");
			pV = (YYRValue*)this;
		} // end else
		return *pV;
	}

	YYRValue& operator()(const int _index, bool _foo) const
	{
		YYRValue& ret = (*this)[_index];
		PushContextStack(pRefArray);
		return ret;
	}

	YYRValue& operator()(const int _index)
	{
		YYRValue& ret = *(YYRValue*)ARRAY_LVAL_RValue(this, _index);
		PushContextStack(pRefArray);
		return ret;
	}

	bool is_number() const
	{
		return (kind == VALUE_REAL || kind == VALUE_INT32 || kind == VALUE_INT64 || kind == VALUE_BOOL);
	}

	// --------------------------------------------------------------------------------------------------
	// operator ==
	// --------------------------------------------------------------------------------------------------
	friend bool operator==(const YYRValue& _v1, const YYRValue& _v2) {
		return (YYCompareVal(_v1, _v2, g_GMLMathEpsilon, false) == 0);
	}
	friend bool operator==(const YYRValue& _v1, double _v2) {
		return (_v1 == YYRValue(_v2));
	}
	friend bool operator==(double _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) == _v2);
	}
	friend bool operator==(const YYRValue& _v1, float _v2) {
		return (_v1 == YYRValue(_v2));
	}
	friend bool operator==(float _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) == _v2);
	}
	friend bool operator==(const YYRValue& _v1, int _v2) {
		return (_v1 == YYRValue(_v2));
	}
	friend bool operator==(int _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) == _v2);
	}
	friend bool operator==(const YYRValue& _v1, long long _v2) {
		return (_v1 == YYRValue(_v2));
	}
	friend bool operator==(long long _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) == _v2);
	}
	friend bool operator==(bool _v1, const YYRValue& _v2) {
		return (_v1 == (bool)_v2);
	}
	friend bool operator==(const YYRValue& _v1, bool _v2) {
		return ((bool)_v1 == _v2);
	}

	// --------------------------------------------------------------------------------------------------
	// operator !=
	// --------------------------------------------------------------------------------------------------
	friend bool operator!=(const YYRValue& _v1, const YYRValue& _v2) {
		return !(_v1 == _v2);
	}
	friend bool operator!=(const YYRValue& _v1, double _v2) {
		return !(_v1 == _v2);
	}
	friend bool operator!=(double _v1, const YYRValue& _v2) {
		return !(_v1 == _v2);
	}
	friend bool operator!=(const YYRValue& _v1, int _v2) {
		return !(_v1 == _v2);
	}
	friend bool operator!=(int _v1, const YYRValue& _v2) {
		return !(_v1 == _v2);
	}
	friend bool operator!=(const YYRValue& _v1, long long _v2) {
		return !(_v1 == _v2);
	}
	friend bool operator!=(long long _v1, const YYRValue& _v2) {
		return !(_v1 == _v2);
	}
	friend bool operator!=(bool _v1, const YYRValue& _v2) {
		return !(_v1 == (bool)_v2);
	}
	friend bool operator!=(const YYRValue& _v1, bool _v2) {
		return !((bool)_v1 == _v2);
	}

	// --------------------------------------------------------------------------------------------------
	// operator <
	// --------------------------------------------------------------------------------------------------
	friend bool operator<(const YYRValue& _v1, const YYRValue& _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans < 0);
	} // end 
	friend bool operator<(const YYRValue& _v1, double _v2) {
		return (_v1 < YYRValue(_v2));
	} // end 
	friend bool operator<(double _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) < _v2);
	} // end 
	friend bool operator<(const YYRValue& _v1, int _v2) {
		return (_v1 < YYRValue(_v2));
	} // end 
	friend bool operator<(int _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) < _v2);
	} // end 
	friend bool operator<(const YYRValue& _v1, long long _v2) {
		return (_v1 < YYRValue(_v2));
	} // end 
	friend bool operator<(long long _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) < _v2);
	} // end 
	friend bool operator<(const YYRValue& _v1, bool _v2) {
		return (_v1 < YYRValue(_v2));
	} // end 


	  // --------------------------------------------------------------------------------------------------
	  // operator <=
	  // --------------------------------------------------------------------------------------------------
	friend bool operator<=(const YYRValue& _v1, const YYRValue& _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans <= 0);
	} // end 
	friend bool operator<=(const YYRValue& _v1, double _v2) {
		return (_v1 <= YYRValue(_v2));
	} // end 
	friend bool operator<=(double _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) <= _v2);
	} // end 
	friend bool operator<=(const YYRValue& _v1, int _v2) {
		return (_v1 <= YYRValue(_v2));
	} // end 
	friend bool operator<=(int _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) <= _v2);
	} // end 
	friend bool operator<=(const YYRValue& _v1, long long _v2) {
		return (_v1 <= YYRValue(_v2));
	} // end 
	friend bool operator<=(long long _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) <= _v2);
	} // end 
	friend bool operator<=(const YYRValue& _v1, bool _v2) {
		return (_v1 <= YYRValue(_v2));
	} // end 

	  // --------------------------------------------------------------------------------------------------
	  // operator >
	  // --------------------------------------------------------------------------------------------------
	friend bool operator>(const YYRValue& _v1, const YYRValue& _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans > 0);
	} // end 
	friend bool operator>(const YYRValue& _v1, double _v2) {
		return (_v1 > YYRValue(_v2));
	} // end 
	friend bool operator>(double _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) > _v2);
	} // end 
	friend bool operator>(const YYRValue& _v1, int _v2) {
		return (_v1 > YYRValue(_v2));
	} // end 
	friend bool operator>(int _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) > _v2);
	} // end 
	friend bool operator>(const YYRValue& _v1, long long _v2) {
		return (_v1 > YYRValue(_v2));
	} // end 
	friend bool operator>(long long _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) > _v2);
	} // end 
	friend bool operator>(const YYRValue& _v1, bool _v2) {
		return (_v1 > YYRValue(_v2));
	} // end 


	  // --------------------------------------------------------------------------------------------------
	  // operator >=
	  // --------------------------------------------------------------------------------------------------
	friend bool operator>=(const YYRValue& _v1, const YYRValue& _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans >= 0);
	} // end 
	friend bool operator>=(const YYRValue& _v1, double _v2) {
		return (_v1 >= YYRValue(_v2));
	} // end 
	friend bool operator>=(double _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) >= _v2);
	} // end 
	friend bool operator>=(const YYRValue& _v1, int _v2) {
		return (_v1 >= YYRValue(_v2));
	} // end 
	friend bool operator>=(int _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) >= _v2);
	} // end 
	friend bool operator>=(const YYRValue& _v1, long long _v2) {
		return (_v1 >= YYRValue(_v2));
	} // end 
	friend bool operator>=(long long _v1, const YYRValue& _v2) {
		return (YYRValue(_v1) >= _v2);
	} // end 
	friend bool operator>=(const YYRValue& _v1, bool _v2) {
		return (_v1 >= YYRValue(_v2));
	} // end 

	  // --------------------------------------------------------------------------------------------------
	  // operator |=
	  // --------------------------------------------------------------------------------------------------
	YYRValue& operator|=(const YYRValue& _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v |= INT64_RValue(&_rhs);
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 |= INT32_RValue(&_rhs);
			break;
		case VALUE_INT64:
			v64 |= INT64_RValue(&_rhs);
			break;
		default:
			YYOpError("|=", this, &_rhs);
			break;
		}
		return *this;
	}
	YYRValue& operator|=(double _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v |= (long long)_rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 |= (long long)_rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("|=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}
	YYRValue& operator|=(long long _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v |= _rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 |= _rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("|=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}
	YYRValue& operator|=(int _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v |= (long long)_rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 |= (long long)_rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("|=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}

	// --------------------------------------------------------------------------------------------------
	// operator &=
	// --------------------------------------------------------------------------------------------------
	YYRValue& operator&=(const YYRValue& _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v &= INT64_RValue(&_rhs);
			val = (double)v;
		} // end block 
		break;
		case VALUE_INT32:
			v32 &= INT32_RValue(&_rhs);
			break;
		case VALUE_INT64:
			v64 &= INT64_RValue(&_rhs);
			break;
		default:
			YYOpError("&=", this, &_rhs);
			break;
		}
		return *this;
	}
	YYRValue& operator&=(double _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v &= (long long)_rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 &= (long long)_rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("&=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}
	YYRValue& operator&=(long long _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v &= _rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 &= _rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("&=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}
	YYRValue& operator&=(int _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v &= (long long)_rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 &= (long long)_rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("&=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}

	// --------------------------------------------------------------------------------------------------
	// operator ^=
	// --------------------------------------------------------------------------------------------------
	YYRValue& operator^=(const YYRValue& _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v ^= INT64_RValue(&_rhs);
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 ^= INT32_RValue(&_rhs);
			break;
		case VALUE_INT64:
			v64 ^= INT64_RValue(&_rhs);
			break;
		default:
			YYOpError("^=", this, &_rhs);
			break;
		}
		return *this;
	}
	YYRValue& operator^=(double _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v ^= (long long)_rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 ^= (long long)_rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("^=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}
	YYRValue& operator^=(long long _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v ^= _rhs;
			val = (double)v;
		} //end block
		break;
		case VALUE_INT32:
			v32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 ^= _rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("^=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}
	YYRValue& operator^=(int _rhs) {
		switch ((kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)val;
			v ^= (long long)_rhs;
			val = (double)v;
		} // end block
		break;
		case VALUE_INT32:
			v32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			v64 ^= (long long)_rhs;
			break;
		default:
		{
			YYRValue rhs(_rhs);
			YYOpError("^=", this, &rhs);
		} // end block
		break;
		}
		return *this;
	}

};

#else
struct YYRValue : RValue
{
};
#endif

typedef	void(*PFUNC_YYGML)(CInstance* _pSelf, CInstance* _pOther);
struct YYGMLFuncs
{
	const char* pName;
	PFUNC_YYGML pFunc;
	YYVAR*		pFuncVar;
};

//extern YYGMLFuncs g_GMLFuncs[];
extern YYObjectBase* g_pGlobal;
extern YYObjectBase* g_pGCObjectContainer;

extern bool g_fGarbageCollection;

extern void AddGlobalObject(YYObjectBase* _pObj);
extern void RemoveGlobalObject(YYObjectBase* _pObj);

#if defined(YYLLVM)
inline char* YYGML_string(double val) {
	YYRValue a(val);
	return YYGML_string(a);
}
inline char* YYGML_string(int val) {
	YYRValue a(val);
	return YYGML_string(a);
}
inline char* YYGML_string(const char* str) { return (char*)str; }

inline char* YYGML_AddString(const YYRValue& _val, const char* _str) { const char* pS = (_val.pRefString != NULL) ? _val.pRefString->get() : NULL; return YYGML_AddString(pS, _str); }
inline char* YYGML_AddString(const char* _str, const YYRValue& _val) { const char* pS = (_val.pRefString != NULL) ? _val.pRefString->get() : NULL; return YYGML_AddString(_str, pS); }
inline char* YYGML_AddString(const YYRValue& _val1, const YYRValue& _val2) {
	const char* pF = (_val1.pRefString != NULL) ? _val1.pRefString->get() : NULL;
	const char* pS = (_val2.pRefString != NULL) ? _val2.pRefString->get() : NULL;
	return YYGML_AddString(pF, pS);
}
inline int strcmp(const YYRValue& _val1, const char* _str) {
	return (
		(((_val1.kind&MASK_KIND_RVALUE) == VALUE_STRING) && ((_val1.pRefString == NULL) || (_val1.pRefString->get() == NULL)) && (_str != NULL) && (*_str == '\0')) ||
		(((_val1.kind&MASK_KIND_RVALUE) == VALUE_STRING) && (_val1.pRefString != NULL) && (_str != NULL) && (_val1.pRefString->get() != NULL) && (strcmp(_val1.pRefString->get(), _str) == 0))
		) ? 0 : 1;
}
inline int strcmp(const char* _str, const YYRValue& _val1) {
	return (
		(((_val1.kind&MASK_KIND_RVALUE) == VALUE_STRING) && ((_val1.pRefString == NULL) || (_val1.pRefString->get() == NULL)) && (_str != NULL) && (*_str == '\0')) ||
		(((_val1.kind&MASK_KIND_RVALUE) == VALUE_STRING) && (_val1.pRefString != NULL) && (_str != NULL) && (_val1.pRefString->get() != NULL) && (strcmp(_str, _val1.pRefString->get()) == 0))
		) ? 0 : 1;
}
#endif

class YYStrBuilder
{
public:
	char*	m_pBuffer;
	int		m_len;
	int		m_curr;

	YYStrBuilder() {
		m_pBuffer = NULL;
		m_len = 0;
		m_curr = 0;
	} // end constructor

	~YYStrBuilder() {
		if (m_pBuffer != NULL) {
			YYFree(m_pBuffer);
			m_pBuffer = NULL;
			m_len = 0;
			m_curr = 0;
		} // end if
	} // end destructor

	char* buffer(void)
	{
		char* pRet = (m_curr == 0) ? (char*)&m_curr : m_pBuffer;
		return pRet;
	} // end answer

	char* answer(void)
	{
		char* pRet = buffer();
		m_curr = 0;
		return pRet;
	} // end answer

	void reset(void)
	{
		m_curr = 0;
	} // end reset

	char* ensureSpace(int _size)
	{
		// check to see if we have enough space
		int freeSpace = (m_len - (m_curr + 1));
		if (_size > freeSpace) {

			// not enough space so grow the amount of memory we have
			int oldSize = (m_len == 0) ? _size : m_len;
			int newSize = (oldSize * 3) / 2;
			if (newSize < (m_curr + _size)) {
				newSize = ((m_curr + _size) * 3) / 2;
			} // end if

			  // allocate the new space
			char* pOldBuffer = m_pBuffer;
			m_pBuffer = (char*)YYAlloc(newSize);

			// copy over the old buffer to the new one
			memcpy(m_pBuffer, pOldBuffer, m_len);
			m_len = newSize;

			// free the old buffer
			if (pOldBuffer != NULL) {
				YYFree(pOldBuffer);
			} // end if
		} // end if


		return m_pBuffer + m_curr;
	} // end ensureSpace

	YYStrBuilder& operator<<(const char* _pStr)
	{
		if (_pStr != NULL) {

			int len = (int)strlen(_pStr) + 1;
			char* pCopy = ensureSpace(len);
			strcpy(pCopy, _pStr);
			m_curr += len - 1;

		} // endif
		return *this;
	} // end constant string

	YYStrBuilder& operator<<(const YYRValue& _val)
	{
		int maxLen = 256;
		char* pBase = (char*)YYAlloc(maxLen);
		char* pCurrent = pBase;
		*pBase = '\0';
		STRING_RValue(&pCurrent, &pBase, &maxLen, &_val);
		int len = (int)((pCurrent - pBase) + 1);
		char* pCopy = ensureSpace(len);
		strcpy(pCopy, pBase);
		m_curr += len - 1;
		YYFree(pBase);

		return *this;
	} // end YYRValue

	YYStrBuilder& operator<<(int _n)
	{
		char a[256];
		yyitoa(_n, a, 10);
		int len = (int)strlen(a);
		char* pCopy = ensureSpace(len + 1);
		strcpy(pCopy, a);
		m_curr += len;
		return *this;
	} // end integer

	YYStrBuilder& operator<<(char _c)
	{
		char* pCopy = ensureSpace(2);
		*pCopy++ = _c;
		*pCopy = '\0';
		++m_curr;
		return *this;
	} // end char

	YYStrBuilder& operator<<(double _d)
	{
		RValue a = { 0 };
		a.val = _d;
		a.kind = VALUE_REAL;
		return *this << *(YYRValue*)&a;
	} // end integer

};

#if defined(YYLLVM)
inline void YYOpError(const char* pOp, const YYRValue* _lhs, const YYRValue* _rhs)
{
	YYStrBuilder sbLHS, sbRHS;
	sbLHS << *_lhs;
	sbRHS << *_rhs;
	YYError("invalid type for %s lhs=%s (type=%d), rhs=%s (type=%d)", pOp, sbLHS.answer(), (_lhs->kind & MASK_KIND_RVALUE), sbRHS.answer(), (_rhs->kind & MASK_KIND_RVALUE));
}
#endif

struct YYLocalArgs
{
	int m_count;
	YYRValue** m_args;
	YYLocalArgs(int _count, int _expected, YYRValue** _args) {
		m_count = (_expected > _count) ? _expected : _count;
		m_args = (YYRValue**)YYAlloc(m_count * (sizeof(YYRValue**) + sizeof(YYRValue)));
		YYRValue* _newarguments = (YYRValue*)&m_args[m_count];
		int n = 0;
		for (; n<_count; ++n) {
			COPY_RValue__Post_nodest(&_newarguments[n], _args[n]);
			m_args[n] = &_newarguments[n];
		} // end for
		YYRValue* pV = &_newarguments[n];
		for (; n<m_count; ++n, ++pV) {
			pV->kind = VALUE_UNDEFINED;
			pV->v64 = 0;
			pV->flags = 0;
			m_args[n] = pV;
		} // end for
	} // end YYLocalArgs

	~YYLocalArgs() {
		for (int n = 0; n<m_count; ++n) {
			FREE_RValue(m_args[n]);
		} // end for
		YYFree(m_args);
	} // end YYLocalArgs
};




//extern char* g_pWAD;
//extern int g_nWADFileLength;
extern int g_nGlobalVariables;
extern int g_nInstanceVariables;
//extern int g_nYYCode;
//extern YYVAR** g_ppVars;
//extern YYVAR** g_ppFuncs;
//extern YYGMLFuncs* g_pGMLFuncs;

// Structure for passing information out of a dynamic library when using DLL's (see WinPhone, Win8Native)
struct SLLVMVars {
	char*			pWad;				// pointer to the Wad
	int				nWadFileLength;		// the length of the wad
	int				nGlobalVariables;	// global varables
	int				nInstanceVariables;	// instance variables
	int				nYYCode;
	YYVAR**			ppVars;
	YYVAR**			ppFuncs;
	YYGMLFuncs*		pGMLFuncs;
	void*			pYYStackTrace;		// pointer to the stack trace
};
extern SLLVMVars*	g_pLLVMVars;
typedef void(*PFUNC_InitYYC)(SLLVMVars* _pVars);


YYCEXPORT void InitLLVM(SLLVMVars* _pVars);



enum eLLVMVars
{
	eYYVar_Global,
	eYYVar_MathEpsilon,
	eYYVar_YYStackTrace
};

typedef void(*PFUNC_SetLLVMVar)(eLLVMVars _key, const YYRValue* _pVal);
extern PFUNC_SetLLVMVar	g_pSetLLVMVar;

struct SYYCaseEntry
{
	YYRValue entry;
	int		value;
};

FORCEINLINE  CInstance*  	YYGML_FindInstance(const YYRValue& _val) FORCEINLINE_ATTR;
FORCEINLINE  CInstance*  	YYGML_FindInstance(const YYRValue& _val) {
	switch (_val.kind) {
	case VALUE_OBJECT: return (CInstance*)_val.pObj;
	case VALUE_REAL:
	case VALUE_INT64:
	case VALUE_INT32:
	case VALUE_BOOL:
		return YYGML_FindInstance((int)_val.asInt64());
	case VALUE_REF:
	{
		int refType = ((int64)(((int64)_val.v64) >> 32));
		switch (refType) {
		case (0 | REFCAT_RESOURCE):
		case (0x00000001 | REFCAT_INSTANCE):
			return YYGML_FindInstance((int)_val.asInt64());
			break;
		} // end switch
	} //end block
	break;
	}
	return NULL;
}

#if defined(YYLLVM_SEP_DLL)
YYCEXTERN int YYGML_array_length(const YYRValue& _arg0);
#else
FORCEINLINE int NOTHROW_ATTR YYGML_array_length(const YYRValue& _arg0) FORCEINLINE_ATTR;
FORCEINLINE int NOTHROW_ATTR YYGML_array_length(const YYRValue& _arg0)
{
	int ret = 0;
	if ((_arg0.kind == VALUE_ARRAY) && (_arg0.pRefArray != NULL)) {
		ret = _arg0.pRefArray->length;
	} // end if
	return ret;
}
#endif

#endif
