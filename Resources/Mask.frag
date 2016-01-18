#version 110

uniform sampler2D u_texture;

varying vec3 v_texCoord;

void main() {
	gl_FragColor = texture2D(u_texture, v_texCoord.xy);
}