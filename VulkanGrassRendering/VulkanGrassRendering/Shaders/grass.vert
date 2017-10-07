#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout(location = 0) in vec4 vertex;
layout(location = 3) in vec4 up;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = vec4(1.0);
}