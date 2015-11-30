//
//  main.cpp
//  OpenGL
//
//  Created by Taku Nonomura on 2015/11/09.
//  Copyright (c) 2015年 Taku Nonomura. All rights reserved.
//

// サンプルコード: http://www.natural-science.or.jp/article/20091107221452.php
// OpenGL入門: http://www.center.nitech.ac.jp/~kenji/Lib/ogl/

#include <iostream>
#include <GLUT/GLUT.h>

int WindowPositionX = 100; // 生成するウィンドウ位置のX座標
int WindowPositionY = 100; // 生成するウィンドウ位置のY座標
int WindowWidth = 512; // 生成するウィンドウの幅
int WindowHeight = 512; // 生成するウィンドウの高さ
char WindowTitle[] = "世界の始まり"; // ウィンドウのタイトル

// -------------------------
// 関数プロトタイプ(宣言)
// -------------------------

void Initialize(void);
void Display(void);
void Ground(void);

// -------------------------
// メイン関数
// -------------------------

int main(int argc, char *argv[]) {
    glutInit(&argc, argv); // 環境の初期化 (&でintのポインタを取得している)
    
    // Windowの設定をつらつらと
    glutInitWindowPosition(WindowPositionX, WindowPositionY); // ウィンドウの位置の指定
    glutInitWindowSize(WindowWidth, WindowHeight); // ウィンドウサイズの指定
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // ディスプレイモードの指定
    
    // 指定した設定でWindowをCreate
    glutCreateWindow(WindowTitle); // ただし、この関数を呼び出した時点では、まだウィンドウは表示されない

    // プログラムは最後にglutMainLoop()関数を呼び出します
    // この関数は、GLUTがイベント処理ループに入ることをあらわします
    // この関数を呼び出せば、トップレベルウィンドウが破棄されるまで処理は戻ってきません
    
    // しかし、これだけでは、ウィンドウが処理するべき基本的な処理を実現することができません
    // ウィンドウ領域が再描画されようとした時、GLUTは何をすればいいのかわからない
    // そこで、GLUTはコールバック処理を登録することで、イベントを処理します。
    // それがglutDisplayFunc(void(*func)(void))です
    
    glutDisplayFunc(Display); // 再描画時に呼び出される関数を指定する (関数名: Display)
    
    Initialize(); // 初期設定
    
    glutMainLoop(); // 上記にも書いたように、この関数を呼んだらトップレベルウィンドウが破棄されるまで処理は返ってこない.
    
    return 0;
}

// -------------------------
// 初期設定の関数
// -------------------------

void Initialize(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Bufferの初期化 (=これが背景色となる)
    glEnable(GL_DEPTH_TEST); // Z Testを有効にする
    
    // 座標変換 (ワールド変換 -> ビュー座標変換(カメラ) -> 射影変換 -> クリッピング)
    
    // 射影変換の際に必要な情報
    gluPerspective(30.0, (double)WindowWidth/(double)WindowHeight, 0.1, 1000.0); // 透視投影の視体積
    
    // ビュー座標変換の際に必要な情報
    gluLookAt(
        0.0, -100.0, 50.0, // 視点の位置 x, y, z
        0.0, 100.0, 0.0, // 視界の中心位置座標 x, y, z
        0.0, 0.0, 1.0 // 視界の上方向のベクトル x, y, z
    );
}

// -------------------------
// 初期設定の関数
// -------------------------

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // カラーバッファとZバッファの消去 (ステンシルバッファ使うときは、別途(GL_STENCIL_BUFFER_BIT)指定する必要あり)
    
    Ground();
    
    glutSwapBuffers(); // glutInitDisplayModel(GLUT_DOUBLE)でダブルバッファリングを可にしているので必要
}

//----------------------------------------------------
// 大地の描画
//----------------------------------------------------

void Ground(void) {
    double ground_max_x = 300.0;
    double ground_max_y = 300.0;
    
    glColor3d(0.8, 0.8, 0.8); // これから書こうとする命令の色を指定します glColor3d(r, g, b);
    
    /*
     * GL_POINTS : 点を打ちます
     * GL_LINES : 2点を対にして、その間を直線で結びます
     * GL_LINE_STRIP : 折れ線を描きます
     * GL_LINE_LOOP : 折れ線を描きます. 始点と終点の間も結ばれます
     * GL_POLYGON : 多角形を描きます
     */
    
    glBegin(GL_LINES); // glBegin() ~ glEnd()の間に、その図形の各頂点の座標値を設定する関数を書く.
    
    for(double ly = -ground_max_y; ly <= ground_max_y; ly += 10.0){
        glVertex3d(-ground_max_x, ly, 0);
        glVertex3d(ground_max_x, ly, 0);
    }
    
    for(double lx = -ground_max_x; lx <= ground_max_x; lx += 10.0){
        glVertex3d(lx, ground_max_y, 0);
        glVertex3d(lx, -ground_max_y, 0);
    }
    
    glEnd();
}