#version 330 core

in vec4 ex_Color;
in vec2 tex_Coord;

out vec4 out_Color;		

uniform int codColShader;
uniform sampler2D myTexture;

vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
vec4 green= vec4(0.0, 1.0, 0.0, 1.0);
vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);


void main(void)
  {
    switch(codColShader) {
        case 1: {
            out_Color = mix(black, green, 0.6);
            break;
        }
        case 2: {
            out_Color = vec4(0.22f, 0.23f, 0.22f, 1.0f);
            break;
        }
        case 3: {
            out_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }
        case 4: {
            out_Color = texture(myTexture, tex_Coord);
            break;      
        }
        default: {
            out_Color = ex_Color;
            break;
        }
    }
  }