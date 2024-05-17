#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform vec3 color;
uniform sampler2D tex0; 
uniform bool useTexture = false;

void main() {
    if (!useTexture) {
        FragColor = vec4(color, 1.0);
    } else {
        FragColor = texture(tex0, texCoord);
    }
}