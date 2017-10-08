#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout(location = 0) in vec4 tesePosition;
layout(location = 1) in vec3 teseNormal;
layout(location = 2) in vec2 teseUV;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 cameraPosition = -transpose(mat3(ubo.view)) * ubo.view[3].xyz;
	vec3 view = tesePosition.xyz - cameraPosition;
	vec3 normal = teseNormal;
	if (dot(view, teseNormal) > 0.0) {
		normal = -normal;
	}

	vec3 baseColor = vec3(0.0, 0.2, 0.0);
	vec3 tipColor = vec3(0.0, 0.6, 0.0);
	vec3 color = mix(baseColor, tipColor, teseUV.y);

	outColor = vec4(color, 1.0);
}