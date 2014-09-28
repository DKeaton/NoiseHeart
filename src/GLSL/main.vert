#version 120

attribute vec4 vertexParams;

varying vec4 params;

void main(void) {
    params = vertexParams;
    gl_Position = gl_Vertex;
}
