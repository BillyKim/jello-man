MathHelper.h contains helpful math functions and constants

# Content #
```
const float INFINITY = FLT_MAX; 
const float EPSILON  = 0.001f;
const float Pi       = 3.1415927f;
const float TwoPi    = 6.2831853f;
const float PiOver2  = 1.5707963f;
const float PiOver4  = 0.7853982f;

D3DX10INLINE T Min(const T& a, const T& b)
D3DX10INLINE T Max(const T& a, const T& b)
D3DX10INLINE T Lerp(const T& a, const T& b, float t)
D3DX10INLINE T Clamp(const T& x, const T& low, const T& high)
D3DX10INLINE T sqr(const T& x) // x * x
D3DX10INLINE float ToDegrees(float radians)
D3DX10INLINE float ToRadians(float degrees)
```