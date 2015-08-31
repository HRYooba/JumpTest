uniform sampler2DRect texture; // ディスプレイの画像
uniform sampler2DRect u_effect; // エフェクトかけた後の画像
uniform vec2 u_resolution; //　画面の大きさ

void main() {
    //現在の座標を読み込む
    vec2 pos = gl_FragCoord.xy;
    
    vec4 col_disp = texture2DRect(texture, pos);
    vec4 col_past = texture2DRect(u_effect, pos);
    
    gl_FragColor = (col_disp + col_past * 9.0) / 10.0;
}
