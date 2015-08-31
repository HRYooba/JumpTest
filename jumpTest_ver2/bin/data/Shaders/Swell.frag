uniform sampler2DRect texture;
uniform sampler2DRect u_camTex;
uniform float u_time;
uniform float u_waterH;
uniform vec2 u_resolution;

void main (void)
{
    //vec2 pos = gl_FragCoord.xy;
    vec2 pos = vec2(gl_FragCoord.x, gl_FragCoord.y + u_waterH);
    
    
    float swell = 0.0;
    for (float i=0.0; i<10.0; i++) {
        swell += sin(pos.y * sin(i) * 0.07 + u_time * (i+10.0)) * (1.2);
    }
    
    float a = ( 1.0 - exp( -gl_FragCoord.y * 0.025) );
    swell *= a;
    
    vec4 col = texture2DRect(u_camTex, pos + vec2(swell, 0));
    vec4 colPre = texture2DRect(u_camTex, pos);
    
    gl_FragColor = vec4(col.rgb, a);
}
