#version 450 core

#include "inc.glsl"
#include MY_EXTERNAL_INCLUDE

void main()
{
    MY_EXTERNAL_DEFINE = vec4(BLUB, BLUB, BLUB, 1) * MY_EXTERNAL_DEFINE;
}