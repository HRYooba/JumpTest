uniform sampler2DRect texture; // ディスプレイの画像
uniform sampler2DRect u_effect; // エフェクトかけた後の画像

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
            sum += texture2DRect(texture, pos + vec2(j, i)) * rate / 16.0;
        }
    }

    vec4 color = sum * vec4(1.0, 1.0, 1.0, 0.15);
    
    gl_FragColor = color;
}
