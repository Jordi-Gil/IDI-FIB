#version 330 core

in vec4 gl_FragCoord;
in vec3 fcolor;
out vec4 FragColor;

uniform vec2 WindowSize;

void main() {
    
    if(gl_FragCoord .x <  WindowSize[0]/2 && gl_FragCoord .y > WindowSize[1]/2){
        FragColor = vec4(fcolor,1);
        //FragColor = vec4(1.0,0.0,0.0,1.0);
    }
    else if(gl_FragCoord .x <  WindowSize[0]/2 && gl_FragCoord .y < WindowSize[1]/2){
        FragColor = vec4(fcolor,1);
        //FragColor = vec4(0.0,1.0,0.0,1.0);
    }
    else if(gl_FragCoord .x >  WindowSize[0]/2 && gl_FragCoord .y > WindowSize[1]/2){
        FragColor = vec4(fcolor,1);
        //FragColor = vec4(1.0,1.0,0.0,1.0);
    }
    else{
        FragColor = vec4(fcolor,1);
        //FragColor = vec4(0.0,0.0,1.0,1.0);
    }

}
