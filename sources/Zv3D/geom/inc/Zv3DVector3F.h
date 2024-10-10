// Zv3D_hcom_placeholder
#ifndef ZV3D_VECTOR3F_H
#define ZV3D_VECTOR3F_H

// Vector3 with float to store coordinates.
typedef struct ZvdVector3F_t
{
	float x, y, z;
} ZvdVector3F;

ZvdVector3F& __stdcall ZvdVectorScale(const ZvdVector3F& v, float s, ZvdVector3F& result);
double __stdcall ZvdVectorLength(const ZvdVector3F& v);
double __stdcall ZvdVectorNormalize(ZvdVector3F& v);
ZvdVector3F& __stdcall ZvdVectorCross(const ZvdVector3F& v1, const ZvdVector3F& v2, ZvdVector3F& result);

#endif // ZV3D_VECTOR3F_H
