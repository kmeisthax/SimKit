//Null vertex transform. Vertexes are rendered directly in clip space.

attribute vec4 vi_vertex;

void main() {
    gl_Position = vi_vertex;
};
