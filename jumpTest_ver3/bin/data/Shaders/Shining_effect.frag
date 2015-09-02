//数値が大きいほどオーラの進む量が大きい
#define DIST_RATE 100.0

uniform sampler2DRect texture; // ディスプレイの画像
uniform sampler2DRect u_effect; // 
uniform vec2 u_resolution;

void main() {
    //現在の座標を読み込む
    vec2 pos = gl_FragCoord.xy;
    //画面中央からの距離
    vec2 centerDist = u_resolution/2.0 - pos;
    
    //テクスチャの読み込み
    vec4 col_disp = texture2DRect(texture, pos) * vec4(1.9, 1.9, 1.2, 1.0);
    vec4 col_effect = texture2DRect(u_effect, pos);
    
    
    //現在の周りの平均値を取る
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
            sum += texture2DRect(u_effect, pos + vec2(j, i) + centerDist / DIST_RATE) * rate / 16.0;
        }
    }
    
    vec4 color = (col_disp + sum * 9.0) / 10.0;
    
    gl_FragColor = color;
}
