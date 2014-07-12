/*
 * =====================================================================================
 *
 *       Filename:  WMM.c
 *
 *    Description:  FIR Filter
 *
 *        Version:  1.0
 *        Created:  2014/06/13 10时51分30秒
 *       Revision:  none
 *       Compiler:  IAR
 *
 *         Author:  XiaoGo (Manyu Gou), 32911290@qq.com
 *        Company:  202 Studio.
 *
 * =====================================================================================
 */
#include "reg52.h"

INT16U ADC_Result=0;
INT16U ADC_Array[32];
INT16U DAC_Array[32];
const float32_t firCoeffs32[NUM_TAPS] ={
0,-0.000543756042743212 ,-0.00184360941864792 ,-0.00245912712019295 ,1.42727156109310e-18 ,0.00806910340109852 ,0.0233666341953186,0.0456721530939323,0.0725279706509183,0.0995814746163354,0.121622235158948,0.134006921465033,0.134006921465033,0.121622235158948,0.0995814746163354,0.0725279706509183,0.0456721530939323,0.0233666341953186,0.00806910340109852,1.42727156109310e-18,-0.00245912712019295,-0.00184360941864792,-0.000543756042743212,0}
int main (void )
{
    arm_fir_instance_f32 S; 
    INT32U blockSize = 32;
    float32_t  *inputF32, *outputF32; 
    System_Init();
    arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], blockSize); 
    outputF32 = *DAC_Array[32];
    intputF32 = *ADC_Array[32];
    while(1)
    {
        ADC_Array[ctr++] = (ADC0H << 8 ) + ADC0L;
        DAC0H = (DAC_Array[ctr]&0xff00) >> 8;
        DAC0L = DAC_Array[ctr] & 0xff;
        if(ctr == 32)
        {

            arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);  
        }

    }


}				/* ----------  end of function main  ---------- */
void System_Init (void  )
{
    ADCCON1=0xBC;
    ADCCON2=0x60;
    ADCCON3=0xF5;
    DACCON =0x1F;
    DAC0H  =0x08;
    DAC0L  =0x00;
}		/* -----  end of function System_Init  ----- */
void arm_fir_init_f32(
        arm_fir_instance_f32 * S,
        uint16_t numTaps,
        float32_t * pCoeffs,
        float32_t * pState,
        uint32_t blockSize)
{
    /* Assign filter taps */
    S->numTaps = numTaps;

    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;

    /* Clear state buffer and the size of state buffer is (blockSize + numTaps - 1) */
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(float32_t));

    /* Assign state pointer */
    S->pState = pState;

}
void arm_fir_f32(
        const arm_fir_instance_f32 * S,
        float32_t * pSrc,
        float32_t * pDst,
        uint32_t blockSize)
{

    float32_t *pState = S->pState;                 /* State pointer */
    float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
    float32_t *pStateCurnt;                        /* Points to the current sample of the state */
    float32_t *px, *pb;                            /* Temporary pointers for state and coefficient buffers */
    uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
    uint32_t i, tapCnt, blkCnt;                    /* Loop counters */


#ifndef ARM_MATH_CM0

    /* Run the below code for Cortex-M4 and Cortex-M3 */

    float32_t acc0, acc1, acc2, acc3;              /* Accumulators */
    float32_t x0, x1, x2, x3, c0;                  /* Temporary variables to hold state and coefficient values */


    /* S->pState points to state array which contains previous frame (numTaps - 1) samples */
    /* pStateCurnt points to the location where the new input data should be written */
    pStateCurnt = &(S->pState[(numTaps - 1u)]);

    /* Apply loop unrolling and compute 4 output values simultaneously.   
     * The variables acc0 ... acc3 hold output values that are being computed:   
     *   
     *    acc0 =  b[numTaps-1] * x[n-numTaps-1] + b[numTaps-2] * x[n-numTaps-2] + b[numTaps-3] * x[n-numTaps-3] +...+ b[0] * x[0]   
     *    acc1 =  b[numTaps-1] * x[n-numTaps] +   b[numTaps-2] * x[n-numTaps-1] + b[numTaps-3] * x[n-numTaps-2] +...+ b[0] * x[1]   
     *    acc2 =  b[numTaps-1] * x[n-numTaps+1] + b[numTaps-2] * x[n-numTaps] +   b[numTaps-3] * x[n-numTaps-1] +...+ b[0] * x[2]   
     *    acc3 =  b[numTaps-1] * x[n-numTaps+2] + b[numTaps-2] * x[n-numTaps+1] + b[numTaps-3] * x[n-numTaps]   +...+ b[0] * x[3]   
     */
    blkCnt = blockSize >> 2;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.   
     ** a second loop below computes the remaining 1 to 3 samples. */
    while(blkCnt > 0u)
    {
        /* Copy four new input samples into the state buffer */
        *pStateCurnt++ = *pSrc++;
        *pStateCurnt++ = *pSrc++;
        *pStateCurnt++ = *pSrc++;
        *pStateCurnt++ = *pSrc++;

        /* Set all accumulators to zero */
        acc0 = 0.0f;
        acc1 = 0.0f;
        acc2 = 0.0f;
        acc3 = 0.0f;

        /* Initialize state pointer */
        px = pState;

        /* Initialize coeff pointer */
        pb = (pCoeffs);

        /* Read the first three samples from the state buffer:  x[n-numTaps], x[n-numTaps-1], x[n-numTaps-2] */
        x0 = *px++;
        x1 = *px++;
        x2 = *px++;

        /* Loop unrolling.  Process 4 taps at a time. */
        tapCnt = numTaps >> 2u;

        /* Loop over the number of taps.  Unroll by a factor of 4.   
         ** Repeat until we've computed numTaps-4 coefficients. */
        while(tapCnt > 0u)
        {
            /* Read the b[numTaps-1] coefficient */
            c0 = *(pb++);

            /* Read x[n-numTaps-3] sample */
            x3 = *(px++);

            /* acc0 +=  b[numTaps-1] * x[n-numTaps] */
            acc0 += x0 * c0;

            /* acc1 +=  b[numTaps-1] * x[n-numTaps-1] */
            acc1 += x1 * c0;

            /* acc2 +=  b[numTaps-1] * x[n-numTaps-2] */
            acc2 += x2 * c0;

            /* acc3 +=  b[numTaps-1] * x[n-numTaps-3] */
            acc3 += x3 * c0;

            /* Read the b[numTaps-2] coefficient */
            c0 = *(pb++);

            /* Read x[n-numTaps-4] sample */
            x0 = *(px++);

            /* Perform the multiply-accumulate */
            acc0 += x1 * c0;
            acc1 += x2 * c0;
            acc2 += x3 * c0;
            acc3 += x0 * c0;

            /* Read the b[numTaps-3] coefficient */
            c0 = *(pb++);

            /* Read x[n-numTaps-5] sample */
            x1 = *(px++);

            /* Perform the multiply-accumulates */
            acc0 += x2 * c0;
            acc1 += x3 * c0;
            acc2 += x0 * c0;
            acc3 += x1 * c0;

            /* Read the b[numTaps-4] coefficient */
            c0 = *(pb++);

            /* Read x[n-numTaps-6] sample */
            x2 = *(px++);

            /* Perform the multiply-accumulates */
            acc0 += x3 * c0;
            acc1 += x0 * c0;
            acc2 += x1 * c0;
            acc3 += x2 * c0;

            tapCnt--;
        }

        /* If the filter length is not a multiple of 4, compute the remaining filter taps */
        tapCnt = numTaps % 0x4u;

        while(tapCnt > 0u)
        {
            /* Read coefficients */
            c0 = *(pb++);

            /* Fetch 1 state variable */
            x3 = *(px++);

            /* Perform the multiply-accumulates */
            acc0 += x0 * c0;
            acc1 += x1 * c0;
            acc2 += x2 * c0;
            acc3 += x3 * c0;

            /* Reuse the present sample states for next sample */
            x0 = x1;
            x1 = x2;
            x2 = x3;

            /* Decrement the loop counter */
            tapCnt--;
        }

        /* Advance the state pointer by 4 to process the next group of 4 samples */
        pState = pState + 4;

        /* The results in the 4 accumulators, store in the destination buffer. */
        *pDst++ = acc0;
        *pDst++ = acc1;
        *pDst++ = acc2;
        *pDst++ = acc3;

        blkCnt--;
    }

    /* If the blockSize is not a multiple of 4, compute any remaining output samples here.   
     ** No loop unrolling is used. */
    blkCnt = blockSize % 0x4u;

    while(blkCnt > 0u)
    {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc0 = 0.0f;

        /* Initialize state pointer */
        px = pState;

        /* Initialize Coefficient pointer */
        pb = (pCoeffs);

        i = numTaps;

        /* Perform the multiply-accumulates */
        do
        {
            acc0 += *px++ * *pb++;
            i--;

        } while(i > 0u);

        /* The result is store in the destination buffer. */
        *pDst++ = acc0;

        /* Advance state pointer by 1 for the next sample */
        pState = pState + 1;

        blkCnt--;
    }

    /* Processing is complete.   
     ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.   
     ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    tapCnt = (numTaps - 1u) >> 2u;

    /* copy data */
    while(tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        tapCnt--;
    }

    /* Calculate remaining number of copies */
    tapCnt = (numTaps - 1u) % 0x4u;

    /* Copy the remaining q31_t data */
    while(tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        tapCnt--;
    }
}
