uniform sampler2DRect u_effect; // エフェクトかけた後の画像
uniform sampler2DRect u_composition; // 背景合成した画像
uniform vec2 u_resolution; //　画面の大きさ
uniform float u_time; // 時間
uniform float u_random; // 乱数

void main() {
    //現在の座標を読み込む
    vec2 pos = gl_FragCoord.xy;
    
    vec4 col_disp = texture2DRect(u_composition, pos);
    
    if ( col_disp.a > 0.0 ) {
        vec4 blue =  col_disp * vec4(0.0, 0.9, 1.0, 0.5) * vec4( max(u_random, 0.7) );
        vec4 stripe = blue * vec4(sin(pos.y * 0.8 - u_time * 0.3) * 0.5 + 1.0);
        vec4 broad =  stripe * 3.0 + 0.25 * sin(pos.y * 0.1 - u_time * 0.307) ;
        
        vec4 color = broad;
        gl_FragColor = color;
    } else {
        gl_FragColor = vec4(0.0);
    }
}
