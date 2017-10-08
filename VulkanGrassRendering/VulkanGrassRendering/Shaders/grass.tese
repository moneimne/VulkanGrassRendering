#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(quads, equal_spacing, ccw) in;

layout(binding = 0) uniform MvpBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

layout(location = 0) patch in vec4 tescV1;
layout(location = 1) patch in vec4 tescV2;
layout(location = 2) patch in vec3 tescUp;
layout(location = 3) patch in vec3 tescDirection;

layout(location = 0) out vec4 tesePosition;
layout(location = 1) out vec3 teseNormal;
layout(location = 2) out vec2 teseUV;

void main() {
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	vec3 widthOffset = tescDirection * tescV2.w;
	vec3 halfOffset = widthOffset * 0.5;

	vec3 p0 = gl_in[0].gl_Position.xyz - halfOffset;
	vec3 p1 = tescV1.xyz - halfOffset;
	vec3 p2 = tescV2.xyz - halfOffset;

	// Find control points on edges of blade
	vec3 h1 = p0 + v * (p1 - p0);
	vec3 h2 = p1 + v * (p2 - p1);
	vec3 i1 = h1 + v * (h2 - h1);
	vec3 i2 = i1 + widthOffset;

	vec3 bitangent = tescDirection;
	vec3 tangent;

	vec3 h1h2 = h2 - h1;
	if (dot(h1h2, h1h2) < 1e-3) {
		tangent = tescUp;
	}
	else {
		tangent = normalize(h1h2);
	}

	teseUV = vec2(u, v);
	teseNormal = normalize(cross(tangent, bitangent));

	// Determine final vertex position based on desired blade shape
	float uOffset = u - 0.5;
	vec3 position = mix(i1, i2, u + (-sign(uOffset) * 0.5 * v));

	gl_Position = mvp.proj * mvp.view * vec4(position, 1.0);
	tesePosition = vec4(position, 1.5 * abs(sin(tescV1.w)));
}