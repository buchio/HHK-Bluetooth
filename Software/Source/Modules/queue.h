/*
 *  Copyright (c) 2013  Yukio Obuchi
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction,
 *  including without limitation the rights to use, copy, modify, merge,
 *  publish, distribute, sublicense, and/or sell copies of the Software,
 *  and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 *  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 *  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

/// \ingroup Modules
/// \defgroup Queue 軽量キュー
/// @{

/**
 * \file
 * \author Yukio Obuchi
 * \date 2013-10-06 
 *
 * 軽量キュー
 *
 * キューサイズを2の冪乗として、キューポインタの計算時に比較を実施しな
 * いようにし、処理中にallocationは行なわないことで、 処理量が少なく、
 * ISR等でも使い易いキューを提供します。
 * 
 */


#ifndef __QUEUE_H_
#define __QUEUE_H_


/**
 * 内部変数用マクロ
 *
 * \param xName    キューの名前
 * \param xValname
 *
 * キュー定義の内部で用いる変数を定義するためのマクロです。グローバルに
 * 定義しないと動作しないため、グローバルに定義してしまっていますが、ユー
 * ザーが使用することは想定していません。
 *
 */
#define QUEUE_VALS(xName,xValname) xName ## _ ## xValname

/**
 * 初期化
 *
 * \param xName    キューの名前
 * \param xSize    キューサイズ。実際のサイズは2のxSize乗となる。
 * \param xType    キューに格納する値の型
 *
 * \ref xType で指定した型の配列を要素数(1<< xSize)で宣言し、リードポイ
 * ンタとライトポインタを定義します。全ての要素をstaticで宣言するため、
 * 宣言したスコープ内でしか利用できません。スコープ外からのアクセスを行
 * いたい場合には適宜アクセス関数を定義することで対応してください。
 *
 */
#define QUEUE_INIT(xName, xSize, xType )                                 \
    static const int QUEUE_VALS(xName,buffermask)  =  ((1 << xSize)-1);  \
    static const int QUEUE_VALS(xName,buffersize)  =  (1 << xSize);      \
    static volatile int QUEUE_VALS(xName,wp);                            \
    static volatile int QUEUE_VALS(xName,rp);                            \
    static volatile xType QUEUE_VALS(xName,buffer)[(1 << xSize)]

/**
 * 値をキューに格納する
 *
 * \param xName    キューの名前
 * \param xval     格納するデータ
 *
 */
#define QUEUE_IN(xName, xVal) \
    QUEUE_VALS(xName,buffer)[QUEUE_VALS(xName,wp)++] = xVal; \
    QUEUE_VALS(xName,wp) &= QUEUE_VALS(xName,buffermask)

/**
 * キューから値を取り出す
 *
 * \param xName    キューの名前
 * \param xval     取り出したデータを格納する変数
 *
 */
#define QUEUE_OUT(xName, xVal) \
    xVal = QUEUE_VALS(xName,buffer)[QUEUE_VALS(xName,rp)++]; \
    QUEUE_VALS(xName,rp) &= QUEUE_VALS(xName,buffermask)

/**
 * キュー先頭の値を覗き見る(キューから削除はしない)
 *
 * \param xName    キューの名前
 *
 */
#define QUEUE_TOP(xName) \
    QUEUE_VALS(xName,buffer)[QUEUE_VALS(xName,rp)]

/**
 * キュー末尾の値を覗き見る(キューから削除はしない)
 *
 * \param xName    キューの名前
 *
 */
#define QUEUE_BOTTOM(xName) \
    QUEUE_VALS(xName,buffer)[(QUEUE_VALS(xName,wp)-1) & QUEUE_VALS(xName,buffermask)]

/**
 * キューのサイズ
 *
 * \param xName    キューの名前
 *
 */
#define QUEUE_STATUS(xName) \
    (QUEUE_VALS(xName,wp) >= QUEUE_VALS(xName,rp)) ? \
        (QUEUE_VALS(xName,wp) - QUEUE_VALS(xName,rp)) : \
        (QUEUE_VALS(xName,wp) + QUEUE_VALS(xName,buffersize) - QUEUE_VALS(xName,rp))

/**
 * キューが一杯かどうか判定する
 *
 * \param xName    キューの名前
 *
 */
#define QUEUE_ISFULL(xName)  (QUEUE_VALS(xName,rp) == (((QUEUE_VALS(xName,wp))+1) & QUEUE_VALS(xName,buffermask)))

/**
 * キューが空かどうか判定する
 *
 * \param xName    キューの名前
 *
 */
#define QUEUE_ISEMPTY(xName) (QUEUE_VALS(xName,rp) == QUEUE_VALS(xName,wp))

/**
 * キューをクリアする
 *
 * \param xName    キューの名前
 *
 */
#define QUEUE_CLEAR(xName) QUEUE_VALS(xName,rp) = QUEUE_VALS(xName,wp) = 0

#endif //__QUEUE_H_

/// @}

#ifdef __RUN_MODULE_TEST__
#include <stdio.h>
#include <assert.h>

#define TEST_QUEUE_SIZE 4


QUEUE_INIT(TEST_QUE, TEST_QUEUE_SIZE, unsigned char);


int main( int argc, char **argv)
{
    int i;
    unsigned char tmp;

    assert( QUEUE_ISEMPTY(TEST_QUE) );


    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ ) {
        QUEUE_IN( TEST_QUE, i*3 );
        assert( QUEUE_BOTTOM( TEST_QUE ) == i*3 );
        assert( QUEUE_STATUS( TEST_QUE ) == i+1 );
    }
    
    assert( !QUEUE_ISEMPTY(TEST_QUE) );
    
    assert( QUEUE_ISFULL(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ ) {
        unsigned char tmp2;
        tmp2 = QUEUE_TOP( TEST_QUE );
        QUEUE_OUT( TEST_QUE, tmp );
        assert( tmp == tmp2);
        assert( tmp == i*3);
    }

    assert( QUEUE_ISEMPTY(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)/2-1; i++ ) {
        QUEUE_IN( TEST_QUE, i*4 );
        assert( QUEUE_BOTTOM( TEST_QUE ) == i*4 );
        assert( QUEUE_STATUS( TEST_QUE ) == i+1 );
    }
    assert( !QUEUE_ISFULL(TEST_QUE) );
    assert( !QUEUE_ISEMPTY(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)/2-1; i++ ) {
        unsigned char tmp2;
        tmp2 = QUEUE_TOP( TEST_QUE );
        QUEUE_OUT( TEST_QUE, tmp );
        assert( tmp == tmp2);
        assert( tmp == i*4 );
    }

    assert( !QUEUE_ISFULL(TEST_QUE) );

    assert( QUEUE_ISEMPTY(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)*2; i++ ) {
        if ( !QUEUE_ISFULL( TEST_QUE) ) {
            QUEUE_IN( TEST_QUE, i*2 );
            assert( QUEUE_BOTTOM( TEST_QUE ) == i*2 );
            assert( QUEUE_STATUS( TEST_QUE ) == i+1 );
        }
    }
    assert( QUEUE_ISFULL(TEST_QUE) );
    assert( !QUEUE_ISEMPTY(TEST_QUE) );
    
    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ ) {
        unsigned char tmp2;
        tmp2 = QUEUE_TOP( TEST_QUE );
        QUEUE_OUT( TEST_QUE, tmp );
        assert( tmp == tmp2);
        assert( tmp == i*2 );
    }

    assert( QUEUE_ISEMPTY(TEST_QUE) );

    printf ( "All test passed\n" );
    return 0;
}

#endif //__RUN_MODULE_TEST__

/*
 * Local variables:
 *  compile-command: "gcc -x c -Wall -g  -I . -D__RUN_MODULE_TEST__ queue.h -o queue && ./queue"
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  tab-width: 8
 * End:
 */

