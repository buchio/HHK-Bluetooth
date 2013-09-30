#ifndef __QUEUE_H_
#define __QUEUE_H_

#define QUEUE_VALS(xName,xValname) xName ## _ ## xValname


#define QUEUE_INIT(xName, xSize, xType )                            \
    volatile const int QUEUE_VALS(xName,buffermask)   = ((1 << xSize)-1); \
    volatile const int QUEUE_VALS(xName,buffersize)   = (1 << xSize);     \
    volatile int QUEUE_VALS(xName,wp);                            \
    volatile int QUEUE_VALS(xName,rp);                            \
    volatile xType QUEUE_VALS(xName,buffer)[(1 << xSize)]

#define QUEUE_IN(xName, xVal) \
    QUEUE_VALS(xName,buffer)[QUEUE_VALS(xName,wp)++] = xVal; \
    QUEUE_VALS(xName,wp) &= QUEUE_VALS(xName,buffermask)

#define QUEUE_OUT(xName, xVal) \
    xVal = QUEUE_VALS(xName,buffer)[QUEUE_VALS(xName,rp)++]; \
    QUEUE_VALS(xName,rp) &= QUEUE_VALS(xName,buffermask)

#define QUEUE_STATUS(xName) \
    (QUEUE_VALS(xName,wp) >= QUEUE_VALS(xName,rp)) ? \
        (QUEUE_VALS(xName,wp) - QUEUE_VALS(xName,rp)) : \
        (QUEUE_VALS(xName,wp) + QUEUE_VALS(xName,buffersize) - QUEUE_VALS(xName,rp))

#define QUEUE_ISFULL(xName)  (QUEUE_VALS(xName,rp) == (((QUEUE_VALS(xName,wp))+1) & QUEUE_VALS(xName,buffermask)))
#define QUEUE_ISEMPTY(xName) (QUEUE_VALS(xName,rp) == QUEUE_VALS(xName,wp))
#define QUEUE_CLEAR(xName) QUEUE_VALS(xName,rp) = QUEUE_VALS(xName,wp) = 0

#endif //__QUEUE_H_


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


    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ )
    {
        QUEUE_IN( TEST_QUE, i*3 );
        assert( QUEUE_STATUS( TEST_QUE ) == i+1 );
    }
    
    assert( !QUEUE_ISEMPTY(TEST_QUE) );
    
    assert( QUEUE_ISFULL(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ )
    {
        QUEUE_OUT( TEST_QUE, tmp );
        assert( tmp == i*3);
    }

    assert( QUEUE_ISEMPTY(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)/2-1; i++ )
    {
        QUEUE_IN( TEST_QUE, i*3 );
        assert( QUEUE_STATUS( TEST_QUE ) == i+1 );
    }
    assert( !QUEUE_ISFULL(TEST_QUE) );
    assert( !QUEUE_ISEMPTY(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)/2-1; i++ )
    {
        QUEUE_OUT( TEST_QUE, tmp );
        assert( tmp == i*3 );
    }

    assert( !QUEUE_ISFULL(TEST_QUE) );

    assert( QUEUE_ISEMPTY(TEST_QUE) );

    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)*2; i++ )
    {
        if ( !QUEUE_ISFULL( TEST_QUE) )
        {
            QUEUE_IN( TEST_QUE, i*2 );
            assert( QUEUE_STATUS( TEST_QUE ) == i+1 );
        }
    }
    assert( QUEUE_ISFULL(TEST_QUE) );
    assert( !QUEUE_ISEMPTY(TEST_QUE) );
    
    for ( i = 0; i < (1<<TEST_QUEUE_SIZE)-1; i++ )
    {
        QUEUE_OUT( TEST_QUE, tmp );
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

