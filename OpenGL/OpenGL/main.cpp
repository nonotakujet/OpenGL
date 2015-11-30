//
//  main.cpp
//  OpenGL
//
//  Created by Taku Nonomura on 2015/11/09.
//  Copyright (c) 2015年 Taku Nonomura. All rights reserved.
//

// 参考: http://www.natural-science.or.jp/article/20091107221452.php

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
}

// -------------------------
// 初期設定の関数
// -------------------------

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // カラーバッファとZバッファの消去 (ステンシルバッファ使うときは、別途(GL_STENCIL_BUFFER_BIT)指定する必要あり)
    glutSwapBuffers(); // glutInitDisplayModel(GLUT_DOUBLE)でダブルバッファリングを可にしているので必要
}