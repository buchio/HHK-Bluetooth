#ifndef __QUEUE_H_
#define __QUEUE_H_

#define QUEUE_VALS(xName,xValname) xName ## _ ## xValname
#define QUEUE_INIT(xName, xSize, xNum ) \
    volatile const int QUEUE_VALS(xName,buffermask)   = ((1 << xSize)-1); \
    volatile const int QUEUE_VALS(xName,buffersize)   = (1 << xSize);            \
    volatile int QUEUE_VALS(xName,wp)[ xNum ];                                   \
    volatile int QUEUE_VALS(xName,rp)[ xNum ];                                   \
    volatile unsigned char QUEUE_VALS(xName,buffer)[ xNum ][(1 << xSize)]

#define QUEUE_EXT(xName, xSize, xNum ) \
    extern const int QUEUE_VALS(xName,buffermask); \
    extern const int QUEUE_VALS(xName,buffersize); \
    extern int QUEUE_VALS(xName,wp)[ xNum ]; \
    extern int QUEUE_VALS(xName,rp)[ xNum ]; \
    extern unsigned char QUEUE_VALS(xName,buffer)[ xNum ][(1 << xSize)]

#define QUEUE_IN(xName, xNum, xVal) \
    QUEUE_VALS(xName,buffer)[ xNum ][QUEUE_VALS(xName,wp)[ xNum ]++] = xVal; \
    QUEUE_VALS(xName,wp)[ xNum ] &= QUEUE_VALS(xName,buffermask)

#define QUEUE_OUT(xName, xNum, xVal) \
    xVal = QUEUE_VALS(xName,buffer)[ xNum ][QUEUE_VALS(xName,rp)[ xNum ]++]; \
    QUEUE_VALS(xName,rp)[ xNum ] &= QUEUE_VALS(xName,buffermask)

#define QUEUE_STATUS(xName, xNum) \
    (QUEUE_VALS(xName,wp)[ xNum ] >= QUEUE_VALS(xName,rp)[ xNum ]) ? \
        (QUEUE_VALS(xName,wp)[ xNum ] - QUEUE_VALS(xName,rp)[ xNum ]) : \
        (QUEUE_VALS(xName,wp)[ xNum ] + QUEUE_VALS(xName,buffersize) - QUEUE_VALS(xName,rp)[ xNum ])

#define QUEUE_ISFULL(xName, xNum)  (QUEUE_VALS(xName,rp)[ xNum ] == (((QUEUE_VALS(xName,wp)[ xNum ])+1) & QUEUE_VALS(xName,buffermask)))
#define QUEUE_ISEMPTY(xName, xNum) (QUEUE_VALS(xName,rp)[ xNum ] == QUEUE_VALS(xName,wp)[ xNum ])
#define QUEUE_CLEAR(xName, xNum) QUEUE_VALS(xName,rp)[ xNum ] = QUEUE_VALS(xName,wp)[ xNum ] = 0


#endif //__QUEUE_H_


#ifdef __RUN_MODULE_TEST__
#include <stdio.h>
#include <assert.h>

#define TEST_QUEUE_SIZE 4
#define TEST_QUEUE_NUM  4


QUEUE_INIT(TEST_QUE, TEST_QUEUE_SIZE, TEST_QUEUE_NUM);


int main( int argc, char **argv)
{
    int i, j;
    unsigned char tmp;

    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( QUEUE_ISEMPTY(TEST_QUE, j) );
    }


    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ )
    {
        for ( j = 0; j < TEST_QUEUE_NUM; j++ )
        {
            QUEUE_IN( TEST_QUE, j, i*j );
            assert( QUEUE_STATUS( TEST_QUE, j ) == i+1 );
        }
    }
    
    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( !QUEUE_ISEMPTY(TEST_QUE, j) );
    }

    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( QUEUE_ISFULL(TEST_QUE, j) );
    }

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ )
    {
        for ( j = 0; j < TEST_QUEUE_NUM; j++ )
        {
            QUEUE_OUT( TEST_QUE, j, tmp );
            assert( tmp == i*j );
        }
    }

    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( QUEUE_ISEMPTY(TEST_QUE, j) );
    }

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)/2-1; i++ )
    {
        for ( j = 0; j < TEST_QUEUE_NUM; j++ )
        {
            QUEUE_IN( TEST_QUE, j, i*j );
            assert( QUEUE_STATUS( TEST_QUE, j ) == i+1 );
        }
    }
    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( !QUEUE_ISFULL(TEST_QUE, j) );
    }
    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( !QUEUE_ISEMPTY(TEST_QUE, j) );
    }

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)/2-1; i++ )
    {
        for ( j = 0; j < TEST_QUEUE_NUM; j++ )
        {
            QUEUE_OUT( TEST_QUE, j, tmp );
            assert( tmp == i*j );
        }
    }

    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( !QUEUE_ISFULL(TEST_QUE, j) );
    }
    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( QUEUE_ISEMPTY(TEST_QUE, j) );
    }

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)*2; i++ )
    {
        for ( j = 0; j < TEST_QUEUE_NUM; j++ )
        {
            if ( !QUEUE_ISFULL( TEST_QUE, j) )
            {
                QUEUE_IN( TEST_QUE, j, i*j );
                assert( QUEUE_STATUS( TEST_QUE, j ) == i+1 );
            }
        }
    }
    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( QUEUE_ISFULL(TEST_QUE, j) );
    }
    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( !QUEUE_ISEMPTY(TEST_QUE, j) );
    }

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ )
    {
        for ( j = 0; j < TEST_QUEUE_NUM; j++ )
        {
            QUEUE_OUT( TEST_QUE, j, tmp );
            assert( tmp == i*j );
        }
    }

    for ( j = 0; j < TEST_QUEUE_NUM; j++ )
    {
        assert( QUEUE_ISEMPTY(TEST_QUE, j) );
    }


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

