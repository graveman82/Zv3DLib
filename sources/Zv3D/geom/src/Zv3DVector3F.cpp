// Zv3D_hcom_placeholder

// Zv3D headers
#include "geom/inc/Zv3DVector3F.h"

// standard headers
#include <math.h>

ZvdVector3F& __stdcall ZvdVectorScale(const ZvdVector3F& v, float s, ZvdVector3F& result)
{
    result.x = s * v.x;
    result.y = s * v.y;
    result.z = s * v.z;
    return result;
}

double __stdcall ZvdVectorLength(const ZvdVector3F& v)
{
    return ::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double __stdcall ZvdVectorNormalize(ZvdVector3F& v)
{
    double len = ZvdVectorLength(v);
    if (len <= 0.0000001)
        return 0.0;
    double ilen = 1.0 / len;
    ZvdVectorScale(v, (float)ilen, v);
    return len;
}

ZvdVector3F& __stdcall ZvdVectorCross(const ZvdVector3F& v1, const ZvdVector3F& v2, ZvdVector3F& result)
{
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

