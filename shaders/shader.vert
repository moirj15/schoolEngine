#version 450

layout (location = 0) in vec4 vPosition;

// layout (location = 1) in vec4 vNormal;

//layout (location = 2) in vec2 uv;

uniform mat4 projection = mat4(1.0);
uniform mat4 transform = mat4(1.0);
uniform mat4 camera = mat4(1.0);
uniform vec4 light = vec4(1.0);

out vec4 normal;
out vec4 lightPos;
out vec4 pos;
out vec2 tex_coord;

void main() {
    gl_PointSize = 10.0;
	gl_Position = projection * camera * transform * vPosition;
    //normal = normalize((camera * transform) * vNormal);
    //lightPos = camera * light;
    //pos = (camera * transform) * vPosition;
    // tex_coord = uv;
}
