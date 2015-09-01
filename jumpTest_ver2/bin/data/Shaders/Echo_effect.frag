uniform sampler2DRect texture; // ディスプレイの画像
uniform sampler2DRect u_effect; // エフェクトかけた後の画像
uniform vec2 u_resolution; //　画面の大きさ

void main() {
    //現在の座標を読み込む
    vec2 pos = gl_FragCoord.xy;
    
    vec4 col_disp = texture2DRect(texture, pos);
    vec4 col_past = texture2DRect(u_effect, pos);
    
    float l = length(col_disp.rgb);
    
    vec4 col_pink = vec4(l, l, l, col_disp.a) * vec4(1.0, 0.6, 0.9, 1.0);
    
    vec4 color = (col_pink + col_past * 15.0) / 16.0;
    
    gl_FragColor = color;
}
