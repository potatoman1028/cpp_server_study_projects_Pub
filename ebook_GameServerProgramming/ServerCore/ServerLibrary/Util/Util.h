#pragma once
#include <algorithm>

//string �ʱ�ȭ
#define UNDEFINE_NAME		L"Undefine_Name"

//snprintf ������, ������ ���� array ����� �⺻���� �մϴ�.
#define snprintf(dst, format, ...)     _snprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)
#define snwprintf(dst, format, ...)    _snwprintf_s(dst.data(), dst.size(), _TRUNCATE, format, __VA_ARGS__)

//���� ���� �� üũ
#define fixInRange(minimum, x, maximum)     min(maximum, max(x, minimum)) 
#define isInRange(minimum, x, maximum)      (x == fixInRange(minimum, x, maximum)) ? true : false

//overflow üũ
inline bool isOverFlower_uint(unsigned int original, unsigned int add)
{
	unsigned int before = original;
	unsigned int after = original + add;
    if ((original & 0x80000000) != (after & 0x80000000)) {
        return false;
    }
    return true;
}

//������ �⺻ ��ũ�� ȸ�ǿ� __FUNCTION__ ����..
#define __W(x)              L##x
#define _W(x)               __W(x)

//-------------------------------------------------------------------//
//���ڿ� ��ȯ
inline void StrConvA2T(CHAR *src, TCHAR *dest, size_t destLen) {
#ifdef  UNICODE                     // r_winnt
    if (destLen < 1) {
        return;
    }
    MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int) destLen - 1);
#endif
}

inline void StrConvT2A(TCHAR *src, CHAR *dest, size_t destLen) {
#ifdef  UNICODE                     // r_winnt
    if (destLen < 1) {
        return;
    }
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int) destLen, NULL, FALSE);
#endif
}

inline void StrConvA2W(CHAR *src, WCHAR *dest, size_t destLen) {
    if (destLen < 1) {
        return;
    }
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int) destLen - 1);
}
inline void StrConvW2A(WCHAR *src, CHAR *dest, size_t destLen) {
    if (destLen < 1) {
        return;
    }
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int) destLen, NULL, FALSE);
}
//-------------------------------------------------------------------//

// delete object
#undef	SAFE_DELETE
#define SAFE_DELETE(obj)						\
{												\
	if ((obj)) delete(obj);		    			\
    (obj) = 0L;									\
}
// delete object array
#undef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(arr)					\
{												\
	if ((arr)) delete [] (arr);		    		\
    (arr) = 0L;									\
}

// delete gameObject
#define SAFE_FREE(obj)							\
{												\
	if ((obj)) obj->free();						\
    (obj) = 0L;									\
}

#define SAFE_RELEASE(obj)                       \
{                                               \
	if (obj) { obj.release(); }                 \
}

