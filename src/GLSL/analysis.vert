#version 120

varying vec4 vColor;

void main(void) {
    vColor = gl_Color;
    gl_Position = gl_Vertex;
}
