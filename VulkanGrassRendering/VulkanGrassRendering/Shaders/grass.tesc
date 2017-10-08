#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(vertices = 1) out;

layout(binding = 0) uniform MvpBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

layout(location = 0) in vec4 vertV1[];
layout(location = 1) in vec4 vertV2[];
layout(location = 2) in vec3 vertUp[];
layout(location = 3) in vec3 vertDirection[];

layout(location = 0) patch out vec4 tescV1;
layout(location = 1) patch out vec4 tescV2;
layout(location = 2) patch out vec3 tescUp;
layout(location = 3) patch out vec3 tescDirection;

void main() {
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	tescV1 = vertV1[0];
	tescV2 = vertV2[0];
	tescUp = vertUp[0];
	tescDirection = vertDirection[0];

	vec3 cameraPosition = -transpose(mat3(mvp.view)) * mvp.view[3].xyz;
	const float distance = distance(gl_in[0].gl_Position.xyz, cameraPosition);
	const float minTessellationLevel = 3.0;
	const float maxTessellationLevel = 5.0;
	const float minDistance = 5.0;
	const float maxDistance = 15.0;
	const float tessellationRange = maxTessellationLevel - minTessellationLevel;
	const float distanceRange = 1.0 / (maxDistance - minDistance);
	const float level = minTessellationLevel + tessellationRange * (1.0 - clamp((distance - minDistance) * distanceRange, 0.0, 1.0));

	gl_TessLevelInner[0] = 1.0;
	gl_TessLevelInner[1] = level;
	gl_TessLevelOuter[0] = level;
	gl_TessLevelOuter[1] = 1.0;
	gl_TessLevelOuter[2] = level;
	gl_TessLevelOuter[3] = 1.0;
}