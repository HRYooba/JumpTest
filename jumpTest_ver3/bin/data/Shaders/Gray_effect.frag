uniform sampler2DRect texture; // ディスプレイの画像
uniform sampler2DRect u_effect; // エフェクトかけた後の画像
uniform vec2 u_resolution; //　画面の大きさ
uniform float u_random;

void main() {
    //現在の座標を読み込む
    vec2 pos = gl_FragCoord.xy;
    
    vec4 col_disp = texture2DRect(texture, pos);
    
    vec4 sum = vec4(0.0);
    for(float i =-1.0; i<=1.0; i++) {
        for (float j=-1.0; j<=1.0; j++) {
            // ガウシアンフィルター
            float rate = 0.0;
            if (i == -1.0) {
                if (j == -1.0) {
                    rate = 1.0;
                } else if (j == 0.0) {
                    rate = 2.0;
                } else {
                    rate = 1.0;
                }
            } else if (i == 0.0) {
                if (j == -1.0) {
                    rate = 2.0;
                } else if (j == 0.0) {
                    rate = 4.0;
                } else {
                    rate = 2.0;
                }
            } else {
                if (j == -1.0) {
                    rate = 1.0;
                } else if (j == 0.0) {
                    rate = 2.0;
                } else {
                    rate = 1.0;
                }
            }
            sum += texture2DRect(texture, pos + vec2(j, i) + sin(pos.x * 0.6 + max(u_random * 10.0, 2.0)) * max(u_random * 4.0, 2.0)) * rate / 16.0;
        }
    }
    
    float d = length(sum.rgb);
    
    vec4 col_gray = vec4(d*1.1, d*1.1, d*1.1, sum.a);
    vec4 color = col_gray;
    
    gl_FragColor = color;
}
