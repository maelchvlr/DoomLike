#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 camMatrix;
uniform mat4 orthoProjection;
uniform bool is2D;

void main() {
    if (is2D) {
        gl_Position = orthoProjection * vec4(aPos, 1.0);
    } else {
        gl_Position = camMatrix * model * vec4(aPos, 1.0);
    }
    texCoord = aTexCoord;
}