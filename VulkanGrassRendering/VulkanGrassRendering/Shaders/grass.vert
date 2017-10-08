#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform MvpBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

layout(location = 0) in vec4 v0;
layout(location = 1) in vec4 v1;
layout(location = 2) in vec4 v2;
layout(location = 3) in vec4 up;

layout(location = 0) out vec4 vertV1;
layout(location = 1) out vec4 vertV2;
layout(location = 2) out vec3 vertUp;
layout(location = 3) out vec3 vertDirection;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	vec4 position = mvp.model * vec4(v0.xyz, 1.0);
	vertV1 = vec4((mvp.model * vec4(v1.xyz, 1.0)).xyz, v1.w);
	vertV2 = vec4((mvp.model * vec4(v2.xyz, 1.0)).xyz, v2.w);

	vertUp = normalize(vertV1.xyz - position.xyz);

	float orientation = v0.w;
	float sinTheta = sin(orientation);
	float cosTheta = cos(orientation);
	vec3 perpendicular = normalize(vec3(sinTheta, sinTheta + cosTheta, cosTheta));
	vertDirection = normalize(cross(vertUp, perpendicular));

	gl_Position = position;
}