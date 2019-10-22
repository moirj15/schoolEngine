#version 450

in vec4 normal;
in vec4 pos;
in vec4 lightPos;
in vec2 tex_coord;

uniform vec3 color = vec3(1.0, 1.0, 1.0);
out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(color, 1.0);
//    fragmentColor = vec4(1.0, 0.0, 0.0, 0.0);//color;
}

