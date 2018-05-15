#version 450 core

#include "inc.glsl"
#include MY_EXTERNAL_INCLUDE
#define ENABLE_BACKFACE_CULLING 0

void main()
{
    MY_EXTERNAL_DEFINE = vec4(BLUB, BLUB, BLUB, 1) * MY_EXTERNAL_DEFINE;

#if ENABLE_BACKFACE_CULLING
    if(det < detEpsilon) return vec3(-1);	// no hit
#endif
}