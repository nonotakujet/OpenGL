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
// Viewportの定義
// -------------------------

double ViewPointX = 0.0;
double ViewPointY = -200.0;
double ViewPointZ = 20.0;

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

int face[][4] = { // 面の定義
    { 0, 1, 2, 3 },
    { 1, 5, 6, 2 },
    { 5, 4, 7, 6 },
    { 4, 0, 3, 7 },
    { 4, 5, 1, 0 },
    { 3, 2, 6, 7 }
};
GLdouble normal[][3] = { // 面の法線ベクトル
    { 0.0, 0.0,-1.0 },
    { 1.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0 },
    {-1.0, 0.0, 0.0 },
    { 0.0,-1.0, 0.0 },
    { 0.0, 1.0, 0.0 }
};
// -------------------------
// マテリアルの定義
// -------------------------

// ambient : 環境光に対する反射設定 (陰には影響しない)
// diffuse : 拡散光に対する反射設定 (陰に影響する)
// specular : 鏡面光に対する反射設定
// shininess : 鏡面係数、きらめき具合？

struct MaterialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};

// jade
MaterialStruct ms_jade = {
    { 0.135, 0.2225, 0.1575, 1.0 }, // ambient
    { 0.54, 0.89, 0.63, 1.0 }, // diffuse
    { 0.316228, 0.316228, 0.316228, 1.0 }, // speculat
    12.8
};

//ruby(ルビー)
MaterialStruct ms_ruby  = {
    {0.1745,   0.01175,  0.01175,   1.0},
    {0.61424,  0.04136,  0.04136,   1.0},
    {0.727811, 0.626959, 0.626959,  1.0},
    76.8
};

//----------------------------------------------------
// 色の定義の定義
//----------------------------------------------------
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 }; //赤色
GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };//緑色
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };//青色
GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };//黄色
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };//白色
GLfloat shininess = 30.0;//光沢の強さ

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
    
    // 光源の設定
    // NOTE : OpenGLでは、光源は GL_LIGHT0からGL_LIGHT7番までの 8 つの光源が有効化できます
    
    GLfloat light_position0[] = { -50.0, -50.0, 20.0, 1.0 }; // 光源0の座標
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0); // 光源0の設定を行う
 }

// -------------------------
// 初期設定の関数
// -------------------------

void Display(void) {
    
    // バッファのクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // カラーバッファとZバッファの消去 (ステンシルバッファ使うときは、別途(GL_STENCIL_BUFFER_BIT)指定する必要あり)
 
    // 座標変換 (ワールド変換 -> ビュー座標変換(カメラ) -> 射影変換 -> クリッピング)
    // NOTE : なぜか 通常の順序とは逆に、Viewport変換 -> 射影変換 -> モデルビュー変換の順に設定する.
    // OpenGL的には、カレント変換行列の生成を行っているが、逆からかけていくことによって作られたカレント行列に、モデル行列をかけるだけで、描画位置が決まるようになる.

    glViewport(0, 0, WindowWidth, WindowHeight);

    // 射影変換の際に必要な情報
    glMatrixMode(GL_PROJECTION); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
    glLoadIdentity(); // カレント行列の初期化
    gluPerspective(30.0, (double)WindowWidth/(double)WindowHeight, 0.1, 1000.0); // 透視投影の視体積

    // ビュー変換に必要な情報 (毎loopでカメラを動かしているイメージ)
    
    ViewPointY += ( -50.0 - ViewPointY) * 0.001;
    
    gluLookAt(
              0.0,  ViewPointY     ,  ViewPointZ, // 視点の位置x,y,z;
              0.0,  ViewPointY+200 ,   0.0,   // 視界の中心位置の参照点座標x,y,z
              0.0,    0.0,   1.0 );  //視界の上方向のベクトルx,y,z
  
    //モデルビュー変換行列の設定--------------------------
    glMatrixMode(GL_MODELVIEW);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
    glLoadIdentity();//行列の初期化
    
    // 陰影ON---------------------
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // 光源0を利用
    // --------------------------
    
    // NOTE : 光源を利用する際は、glColor3dが無効になるので、オブジェクトの色は`glMaterialfv(適用する面, 光源の種類, 反射する色)`で指定する
    
    /*
     * 複数のオブジェクトを描画する際は、Initializeで作成したカレント行列を元に描画する必要があるので、オブジェクトの描画ごとにカレント行列をPushして、描画おわり次第Popする.
     */
    
    // 球
    glPushMatrix(); // カレント行列をスタックに保存しておく (popされるまでのカレント行列への変更を無視できる)
    glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
    glTranslated(0.0, 10.0, 20.0); // 平行移動値の設定 (OpenGL内部では、カレント行列に平行移動行列を乗算して、カレント行列を更新している)
    glutSolidSphere(4.0, 20, 20);
    glPopMatrix();

    // 立方体
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
    glTranslated(-20.0, 0.0, 20.0);
    glutSolidCube(10.0); // 引数: 一辺の長さ
    glPopMatrix();

    // 円錐
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glTranslated(20.0, 100.0, 0.0);
    glutSolidCone(5.0, 10.0, 20, 20); // 引数: (半径、高さ、Z軸まわりの分割数、Z軸に沿った分割数)
    glPopMatrix();
    
    // 直方体
    // NOTE 「GLUT」に関数が与えられている球（glutSolidSphere）や立方体（glutSolidCube）の場合には、
    // 物体の「表」「裏」の情報に与えたれているが、任意の物体を自前で定義した場合には、各面に対して法線ベクトルを定義する必要がある
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, ms_jade.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_jade.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ms_jade.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &ms_jade.shininess);
    glTranslated(30.0, 50.0, 0.0); // 平行移動値の設定
    glBegin(GL_QUADS);
    for (int j = 0; j < 6; j++) {
        glNormal3dv(normal[j]); //法線ベクトルの指定
        for (int i = 0; i < 4; i++) {
            glVertex3dv(vertex[face[j][i]]);
        }
    }
    glEnd();
    glPopMatrix();
    
    //陰影OFF-----------------------------
    glDisable(GL_LIGHTING);
    //-----------------------------------

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