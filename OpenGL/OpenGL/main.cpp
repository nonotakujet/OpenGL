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
// 直方体の定義
// -------------------------

GLdouble vertex[][3] = {
    { 0.0, 0.0, 0.0 },
    { 2.0, 0.0, 0.0 },
    { 2.0, 2.0, 0.0 },
    { 0.0, 2.0, 0.0 },
    { 0.0, 0.0, 30.0 },
    { 2.0, 0.0, 30.0 },
    { 2.0, 2.0, 30.0 },
    { 0.0, 2.0, 30.0 }
};

int face[][4] = {
    { 0, 1, 2, 3 },
    { 1, 5, 6, 2 },
    { 5, 4, 7, 6 },
    { 4, 0, 3, 7 },
    { 4, 5, 1, 0 },
    { 3, 2, 6, 7 }
};

// -------------------------
// 関数プロトタイプ(宣言)
// -------------------------

void Initialize(void);
void Idle(void);
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
    
    glutDisplayFunc(Display); // 描画時に呼び出される関数を指定する (関数名: Display)
    
    glutIdleFunc(Idle); // プログラムがアイドル時に呼び出さされる関数を指定する (Idleと判断した時に再Idle関数が実行され、そこで再描画コマンドを発行することで、Animationを実現する)
    
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
    // NOTE : なぜか 通常の順序とは逆に、Viewport変換 -> 射影変換 -> モデルビュー変換の順に設定する.
    // OpenGL的には、カレント変換行列の生成を行っている.
    
    // 射影変換の際に必要な情報
    glMatrixMode(GL_PROJECTION); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
    glLoadIdentity(); // 行列の初期化
    gluPerspective(30.0, (double)WindowWidth/(double)WindowHeight, 0.1, 1000.0); // 透視投影の視体積
    
    // ビュー座標変換の際に必要な情報
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // 行列の初期化
    gluLookAt(
        20.0, -200.0, 40.0, // 視点の位置 x, y, z
        0.0, 0.0, 0.0, // 視界の中心位置座標 x, y, z
        0.0, 0.0, 1.0 // 視界の上方向のベクトル x, y, z
    );
}

// -------------------------
// 初期設定の関数
// -------------------------

void Display(void) {
    
    // バッファのクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // カラーバッファとZバッファの消去 (ステンシルバッファ使うときは、別途(GL_STENCIL_BUFFER_BIT)指定する必要あり)
    
    /*
     * 複数のオブジェクトを描画する際は、Initializeで作成したカレント行列を元に描画する必要があるので、オブジェクトの描画ごとにカレント行列をPushして、描画おわり次第Popする.
     */
    
    // 球
    glPushMatrix(); // カレント行列をスタックに保存しておく (popされるまでのカレント行列への変更を無視できる)
    glColor3d(1.0, 0.0, 0.0); // 色の設定
    glTranslated(0.0, 10.0, 20.0); // 平行移動値の設定 (OpenGL内部では、カレント行列に平行移動行列を乗算して、カレント行列を更新している)
    glutSolidSphere(4.0, 20, 20);
    glPopMatrix();

    // 立方体
    glPushMatrix();
    glColor3d(0.0, 1.0, 0.0);
    glTranslated(-20.0, 0.0, 20.0);
    glutSolidCube(10.0); // 引数: 一辺の長さ
    glPopMatrix();

    // 円錐
    glPushMatrix();
    glColor3d(0.0, 0.0, 1.0);
    glTranslated(20.0, 100.0, 0.0);
    glutSolidCone(5.0, 10.0, 20, 20); // 引数: (半径、高さ、Z軸まわりの分割数、Z軸に沿った分割数)
    glPopMatrix();
    
    // 直方体
    glPushMatrix();
    glColor3d(0.0, 1.0, 1.0);
    glTranslated(30.0, 50.0, 0.0); // 平行移動値の設定
    glBegin(GL_QUADS);
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 4; i++) {
            glVertex3dv(vertex[face[j][i]]);
        }
    }
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    Ground();
    glPopMatrix();
    
    glutSwapBuffers(); // glutInitDisplayModel(GLUT_DOUBLE)でダブルバッファリングを可にしているので必要
}

// -------------------------
// アイドル時に呼び出される関数
// -------------------------

void Idle(void) {
    glutPostRedisplay(); // glutDisplayFunc()で指定した関数を1回実行する.
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