/*########################################################################
 
  The contents of this file are subject to the Mozilla Public License
  Version 1.0(the "License");   You  may  NOT  use this file except in
  compliance with the License. You may obtain a copy of the License at
                http:// www.mozilla.org/MPL/
  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
  the License for the specific language governing rights and limitations
  under the License.
 
  The Initial Developer of the Original Code is Shivang Patel.
 
  Copyright(C) 2002. All Rights Reserved.
 
  Authors: Shivang Patel
           Jaap de Haan(jdh)
  
  This file contains function related to the matching of fingerprints 
 
########################################################################*/

#if !(CFG_MATCHING_V1)
#if 1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif
 
#include "matching.h"

#define MAX_CNT 150
 
#define REF_X (FvsInt_t) 0
#define REF_Y (FvsInt_t) 0
#define REF_THETA (FvsFloat_t) 0.0
#define PI (double)3.14285714285714285714285714285714
/*!
  Polar Minutia structure. We use here the New model.
*/
typedef struct Fvs_PolarMinutia_t
{
    /*! r position */
    FvsFloat_t    r;
    /*! e position */
    FvsFloat_t    e;
    /*! theta position */
    FvsFloat_t    angle;
 
} Fvs_PolarMinutia_t;
 
FvsError_t Insertion_Sort(Fvs_PolarMinutia_t*, FvsInt_t) ;
FvsError_t fInsertion_Sort(FvsFloat_t* v, FvsInt_t v_length);
 
// Define the arrays of size 100 for :
// 1. Cartesian to Polar conversion.
// 2. Input & Tmplt minutiaes represented ..
// .. in polar co-ordinates system in the ..
// .. increasing order of radial angle.

#if !(CFG_ENABLE_LOWER_RAM)
Fvs_PolarMinutia_t p_polar_input[MAX_CNT];
Fvs_PolarMinutia_t p_polar_tmplt[MAX_CNT];
 
Fvs_PolarMinutia_t s_polar_input[MAX_CNT];
Fvs_PolarMinutia_t s_polar_tmplt[MAX_CNT];
#endif
 

/* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO */
 
FvsError_t MatchingCompareImages
(
    /*@unused@*/const FvsImage_t image1,
    /*@unused@*/const FvsImage_t image2,
    /*@unused@*/FvsInt_t* pgoodness
)
{
    /* original code from Shivang Patel tries to make this */
    /* that is an image based matching, Shivang also extracts
    the minutia from the images and makes a comparison of these
    features */
    
    /* I would rather drop completely that code and only retain the
    good ideas in it and implement a cross-corelation algorithm
    without any feature extraction */
    
    return FvsOK;
}
 
//============ Data declaration for Find Edit Distance code ============
//=========================================================
 
# define ALPHA (float) 1.0
# define BETA (float) 2.0
# define GAMMA (float) 0.1
# define OHM (float) (200.0 * (ALPHA + BETA + GAMMA))
# define ETA (float) 0.5
 
# define DEL_LO_M_N (float) -8.0
# define DEL_HI_M_N (float) +8.0
# define SI_LO_M_N (float) -7.5
# define SI_HI_M_N (float) +7.5
 
# define DEL_REF (float) (DEL_HI_M_N - DEL_LO_M_N)
# define SI_REF  (float) (SI_HI_M_N  - SI_LO_M_N )
# define EPS_REF (int) 30
# define EDIT_DIST_THRESHOLD (int) 10
 
FvsError_t MatchingCompareMinutiaSets (
    const FvsMinutiaSet_t set1,         // I/P
    const FvsMinutiaSet_t set2,         // I/P
    FvsInt_t* pgoodness                 // O/P
)
{
 
    /* I already provided functions to extract the minutia from a
    fingerprint image. What we have to implement here is a minutia
    based matching algorithm */
 
// TODO: Add your code here... //
 
    /*@madhav@*/
 
    FvsInt_t i = 0, n = 0, m = 0;
    FvsInt_t ix, tx;
    FvsInt_t iy, ty;
    FvsFloat_t itheta, ttheta;
    FvsFloat_t ir, tr;
    FvsFloat_t ie, te;
    FvsFloat_t ftmp1, ftmp2, ftmp3 = 0.0;
    FvsInt_t ltmp1 = 0, ltmp2 = 0, ltmp3 = 0;
    FvsFloat_t fatmp1[MAX_CNT];
    FvsFloat_t fatmp2[MAX_CNT];
    FvsFloat_t edit_dist[MAX_CNT][MAX_CNT];
 
    // for edit distance computation
    FvsFloat_t diff_r, diff_e, diff_theta, ftmp;
    FvsFloat_t window_mn, a;
    FvsFloat_t edit_dist_m1_n;
    FvsFloat_t edit_dist_m_n1;
    FvsFloat_t edit_dist_m1_n1;
    FvsFloat_t edit_dist_m_n = (float) 0.0;
    int window_flag;
    int nb_pair;
    int nb_minutiae;
    float Mpq;

#if (CFG_ENABLE_LOWER_RAM)
    Fvs_PolarMinutia_t p_polar_input[MAX_CNT];
    Fvs_PolarMinutia_t p_polar_tmplt[MAX_CNT];
 
    Fvs_PolarMinutia_t s_polar_input[MAX_CNT];
    Fvs_PolarMinutia_t s_polar_tmplt[MAX_CNT];
#endif
 
    FvsMinutia_t* input_minutia = MinutiaSetGetBuffer(set1);
    FvsInt_t nb_input_minutia   = MinutiaSetGetCount(set1);
    FvsInt_t tablesize1 = MinutiaSetGetSize(set1);
 
    FvsMinutia_t* tmplt_minutia = MinutiaSetGetBuffer(set2);
    FvsInt_t nb_tmplt_minutia   = MinutiaSetGetCount(set2);
    FvsInt_t tablesize2 = MinutiaSetGetSize(set2);
 
    if (input_minutia==NULL)
        return FvsMemory;
 
    if (tmplt_minutia==NULL)
        return FvsMemory;

    //printf("Number of minutiae in the images are %d %d\n", nb_input_minutia, nb_tmplt_minutia);
    
//============ Cartesian to Polar ===============================
//=========================================================
 
    for (n = 0; n < nb_input_minutia; n++) {
 
        ix = (FvsInt_t)input_minutia[n].x;
        iy = (FvsInt_t)input_minutia[n].y;
        itheta = (FvsFloat_t) input_minutia[n].angle * (180 / PI);
        
        ftmp1 = (FvsFloat_t) (ix - REF_X) * (ix - REF_X);
        ftmp2 = (FvsFloat_t) (iy - REF_Y) * (iy - REF_Y);
        p_polar_input[n].r = sqrt (ftmp1 + ftmp2);
 
        ftmp1 = (FvsFloat_t) (ix - REF_X);
        ftmp2 = (FvsFloat_t) (iy - REF_Y);
 
        p_polar_input[n].e = (FvsFloat_t) atan (ftmp2 / ftmp1);
 
        p_polar_input[n].angle = (itheta - REF_THETA);
    }

    for (n = 0; n < nb_tmplt_minutia; n++) {

        tx = (FvsInt_t)tmplt_minutia[n].x;
        ty = (FvsInt_t)tmplt_minutia[n].y;
        ttheta = (FvsFloat_t) tmplt_minutia[n].angle * (180 / PI);
        
        ftmp1 = (FvsFloat_t) (tx - REF_X) * (tx - REF_X);
        ftmp2 = (FvsFloat_t) (ty - REF_Y) * (ty - REF_Y);
        p_polar_tmplt[n].r = sqrt (ftmp1 + ftmp2);
 
        ftmp1 = (FvsFloat_t) (tx - REF_X);
        ftmp2 = (FvsFloat_t) (ty - REF_Y);
 
        p_polar_tmplt[n].e = (FvsFloat_t) atan (ftmp2 / ftmp1);
 
        p_polar_tmplt[n].angle = (ttheta - REF_THETA);
 
    }

    /*
        Arrange the polar minutiae in the
        increasing order of radial angle.
 
        "INSERTION SORTING IS USED FOR POLAR INPUT MINUTIAE.."
    */
 
    for (n = 0; n < nb_input_minutia; n++) {
 
        s_polar_input[n].r = p_polar_input[n].r;
        s_polar_input[n].e = p_polar_input[n].e;
        s_polar_input[n].angle = p_polar_input[n].angle;
        fatmp1[n] = p_polar_input[n].angle;
        fatmp2[n] = fatmp1[n];
    }

    Insertion_Sort (&s_polar_input[0], nb_input_minutia);

    /*
        Arrange the polar minutiae in the
        increasing order of radial angle.
 
        "INSERTION SORTING IS USED FOR POLAR TMPLT MINUTIAE.."
    */
 
    for (n = 0; n < nb_input_minutia; n++) {
 
        s_polar_tmplt[n].r = p_polar_tmplt[n].r;
        s_polar_tmplt[n].e = p_polar_tmplt[n].e;
        s_polar_tmplt[n].angle = p_polar_tmplt[n].angle;
        fatmp1[n] = p_polar_tmplt[n].angle;
        fatmp2[n] = fatmp1[n];
    }

    Insertion_Sort (&s_polar_tmplt[0], nb_tmplt_minutia);

//============ Find Edit Distance ===============================
//=========================================================
 
    //  if m = 0 or n = 0
    //  edit ditance = 0

    for (m = 0; m < nb_tmplt_minutia; m++)
        edit_dist[m][0] = 0.0;
 
    for (n = 0; n < nb_input_minutia; n++)
        edit_dist[0][n] = 0.0;
 
    for (m = 1; m <= nb_tmplt_minutia; m++) {
 
        tr = s_polar_tmplt[m].r;
        te = s_polar_tmplt[m].e;
        ttheta = s_polar_tmplt[m].angle;
 
        for (n = 1; n <= nb_input_minutia; n++) {
 
            ir = s_polar_input[n].r;
            ie = s_polar_input[n].e;
            itheta = s_polar_input[n].angle;
 
            // calculate window function w(m,n)
    
            // 1. calculate diff r
 
            diff_r = tr - ir;
 
            // 2. calculate diff e
 
            ftmp = te - ie;
            ftmp += 360.0;
 
            // a = (ie - te + 360) mod 360
 
            if (ftmp > 360.0) {
 
                a = ftmp - 360.0;
 
            } else {
 
                a = 360.0 - ftmp;
            }                
 
            if (a < 180.0) {
 
                diff_e = a;
 
            } else {
 
                diff_e = a - 180.0;
            }
    
            // 3. calculate diff theta
 
            ftmp = ttheta - itheta;
            ftmp += 360.0;
 
            // a = (itheta - ttheta + 360) mod 360
            
            if (ftmp > 360.0) {
 
                a = ftmp - 360.0;
 
            } else {
 
                a = 360.0 - ftmp;
            }                
 
            if (a < 180.0) {
 
                diff_theta = a;
 
            } else {
 
                diff_theta = a - 180.0;
            }
 
            // find window function w(m,n)
 
            window_flag = 0;
            
            if((diff_r < DEL_REF) | (diff_e < SI_REF) |\
                (diff_theta < EPS_REF)) {
 
                window_flag = 1;
 
            }
 
            if (window_flag == 1) {
 
                window_mn = (ALPHA * diff_r);
                window_mn += (BETA * diff_e);
                window_mn += (GAMMA * diff_theta);  // w(m,n)
 
            } else {
 
                window_mn = OHM;                    // w(m,n)
            }
 
            //=========================
            // find edit distance:
            //=========================
            // edit_dist[m][n] = ?
            //=========================
 
            edit_dist_m1_n  = edit_dist[m-1][n+0] + OHM;
            edit_dist_m_n1  = edit_dist[m+0][n-1] + OHM;
            edit_dist_m1_n1 = edit_dist[m-1][n-1] + window_mn;
 
            edit_dist_m_n = 0.0;
            
            if (edit_dist_m1_n < edit_dist_m_n1)
                edit_dist_m_n = edit_dist_m1_n;
            else
                edit_dist_m_n = edit_dist_m_n1;
 
            if (edit_dist_m1_n1 < edit_dist_m_n)
                edit_dist_m_n = edit_dist_m1_n1;
 
            edit_dist[m][n] = edit_dist_m_n;
            //if (m == n){
                //printf("%s:%d ... [%d,%d] %f\n", __func__, __LINE__, m, n, edit_dist[m][n]);
            //}
 
        }
    }
    //printf("%s:%d ... %d\n", __func__, __LINE__, nb_tmplt_minutia);
    nb_pair = 0;
 
    for (m = 1; m <= nb_tmplt_minutia; m++) {
        //printf("%s:%d ... %f\n", __func__, __LINE__, edit_dist[m][m]);
        if (edit_dist[m][m] < (float) EDIT_DIST_THRESHOLD) {
            nb_pair++;
        }
 
    }
 
    nb_minutiae = nb_tmplt_minutia;
    
    if (nb_input_minutia < nb_tmplt_minutia) {
        nb_minutiae = nb_input_minutia;
    }

    //printf("%s:%d ... %d %d %d\n", __func__, __LINE__, nb_pair, nb_minutiae, nb_tmplt_minutia);
  
    Mpq = ((float)100.0 * (float)nb_pair) / (float)nb_minutiae;
 
    //printf("matching %d %d %d ...\n", (FvsInt_t)nb_pair, (FvsInt_t)nb_minutiae, (FvsInt_t)Mpq);
    if (Mpq > 70.0) {// Compare 70% minutiae match
        //printf ("Templated matched with input\n");
    } else {
        //printf ("Templated mot matched with input\n");
    }
 
    *pgoodness = (FvsInt_t)Mpq;
    //printf ("matching application !!!\n\n\n\n");
 
//============ End Find Edit Distance ===========================
//=========================================================
 
    /*@madhav_end@*/
 
// TODO: Add your code here... //
    
    return FvsOK;
}
 
FvsError_t Insertion_Sort (
    Fvs_PolarMinutia_t* v,      // I/O
    FvsInt_t v_length           // I/P
) 
{
 
    FvsFloat_t item_to_insert1; // On the kth pass, insert item k into its correct 
    FvsFloat_t item_to_insert2; // On the kth pass, insert item k into its correct 
    FvsFloat_t item_to_insert3; // On the kth pass, insert item k into its correct 
    int still_looking, j, k;    // position among the first k entries in vector. 
    
    for (k = 1; k < v_length; ++k) {
        //printf("%s:%d ... %d\n", __func__, __LINE__, k);
        // Walk backwards through list,
        // looking for slot to insert v[k] 
 
        item_to_insert1 = v[k].angle;
        item_to_insert2 = v[k].r;
        item_to_insert3 = v[k].e;
 
        j = k - 1; 
        still_looking = 1; 
        
        while ((j >= 0) && (still_looking==1))  {
            //printf("%s:%d ... %d\n", __func__, __LINE__, j);
            if (item_to_insert1 < v[j].angle) {
 
                v[j + 1].angle = v[j].angle; 
                v[j + 1].r = v[j].r; 
                v[j + 1].e = v[j].e; 
 
                v[j].angle = item_to_insert1;       // where item_to_insert belongs 
                v[j].r = item_to_insert2;           // where item_to_insert belongs 
                v[j].e = item_to_insert3;           // where item_to_insert belongs 
 
                --j; 
 
            } else  {
 
                still_looking = 0;                  // Upon leaving loop, j + 1 is the index 
                v[j + 1].angle = item_to_insert1;   // where item_to_insert belongs 
                v[j + 1].r = item_to_insert2;       // where item_to_insert belongs 
                v[j + 1].e = item_to_insert3;       // where item_to_insert belongs 
            }
        }
    }

    //printf("%s:%d ... %d\n", __func__, __LINE__, k);
 
    return FvsOK;
}

#else

/*#############################################################################
 * �ļ�����matching.cpp
 * ���ܣ�  ʵ����ָ��ƥ���㷨
#############################################################################*/

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "matching.h"

#define REF_X (FvsInt_t) 0
#define REF_Y (FvsInt_t) 0
#define REF_THETA (FvsFloat_t) 0.0
#define PI (double)3.14285714285714285714285714285714

#define MM 150

typedef struct Fvs_PolarMinutia_t {
    /* rλ�� */
    FvsFloat_t    r;
    /* eλ�� */
    FvsFloat_t    e;
    /* �Ƕ�  */
    FvsFloat_t    angle;

} Fvs_PolarMinutia_t;

FvsError_t Insertion_Sort(Fvs_PolarMinutia_t*, FvsInt_t) ;
FvsError_t fInsertion_Sort(FvsFloat_t* v, FvsInt_t v_length);


Fvs_PolarMinutia_t p_polar_input[MM];
Fvs_PolarMinutia_t p_polar_tmplt[MM];

Fvs_PolarMinutia_t s_polar_input[MM];
Fvs_PolarMinutia_t s_polar_tmplt[MM];


/******************************************************************************
  * ���ܣ�ƥ������ָ��
  * ������image1      ָ��ͼ��1
  *       image2      ָ��ͼ��2
  *       pgoodness   ƥ��ȣ�Խ��Խ��
  * ���أ�������
******************************************************************************/
FvsError_t MatchingCompareImages
(
    const FvsImage_t image1,
    const FvsImage_t image2,
    FvsInt_t* pgoodness
) {
    /* �������㷨��δʵ�֣����߿����Լ�����ʵ�֣��� */
    return FvsOK;
}


# define ALPHA (float) 1.0
# define BETA (float) 2.0
# define GAMMA (float) 0.1
# define OHM (float) (200.0 * (ALPHA + BETA + GAMMA))
# define ETA (float) 0.5

# define DEL_LO_M_N (float) -8.0
# define DEL_HI_M_N (float) +8.0
# define SI_LO_M_N (float) -7.5
# define SI_HI_M_N (float) +7.5

# define DEL_REF (float) (DEL_HI_M_N - DEL_LO_M_N)
# define SI_REF  (float) (SI_HI_M_N  - SI_LO_M_N )
# define EPS_REF (int) 30
# define EDIT_DIST_THRESHOLD (int) 10


/******************************************************************************
  * ���ܣ�ƥ��ָ��ϸ�ڵ�
  * ������minutia1      ϸ�ڵ㼯��1
  *       minutia2      ϸ�ڵ㼯��2
  *       pgoodness   ƥ��ȣ�Խ��Խ��
  * ���أ�������
******************************************************************************/
FvsError_t MatchingCompareMinutiaSets (
    const FvsMinutiaSet_t set1,
    const FvsMinutiaSet_t set2,
    FvsInt_t* pgoodness
) {
    FvsInt_t i = 0, n = 0, m = 0;
    FvsInt_t ix, tx;
    FvsInt_t iy, ty;
    FvsFloat_t itheta, ttheta;
    FvsFloat_t ir, tr;
    FvsFloat_t ie, te;
    FvsFloat_t ftmp1, ftmp2, ftmp3 = 0.0;
    FvsInt_t ltmp1 = 0, ltmp2 = 0, ltmp3 = 0;
    FvsFloat_t fatmp1[MM];
    FvsFloat_t fatmp2[MM];
    FvsFloat_t edit_dist[MM][MM];
    // �������֮��
    FvsFloat_t diff_r, diff_e, diff_theta, ftmp;
    FvsFloat_t window_mn, a;
    FvsFloat_t edit_dist_m1_n;
    FvsFloat_t edit_dist_m_n1;
    FvsFloat_t edit_dist_m1_n1;
    FvsFloat_t edit_dist_m_n = (float) 0.0;
    int window_flag;
    int nb_pair;
    int nb_minutiae;
    float Mpq;
    FvsMinutia_t* input_minutia = MinutiaSetGetBuffer(set1);
    FvsInt_t nb_input_minutia   = MinutiaSetGetCount(set1);
    FvsMinutia_t* tmplt_minutia = MinutiaSetGetBuffer(set2);
    FvsInt_t nb_tmplt_minutia   = MinutiaSetGetCount(set2);
    if (input_minutia == NULL)
        return FvsMemory;
    if (tmplt_minutia == NULL)
        return FvsMemory;
    printf("\nNumber of minutiae in the input image is = %d", nb_input_minutia);
    printf("\nNumber of minutiae in the tmplt image is = %d", nb_tmplt_minutia);
    for (n = 0; n < nb_input_minutia; n++) {
        ix = (FvsInt_t)input_minutia[n].x;
        iy = (FvsInt_t)input_minutia[n].y;
        itheta = (FvsFloat_t) input_minutia[n].angle * (180 / PI);
        ftmp1 = (FvsFloat_t) (ix - REF_X) * (ix - REF_X);
        ftmp2 = (FvsFloat_t) (iy - REF_Y) * (iy - REF_Y);
        p_polar_input[n].r = sqrt (ftmp1 + ftmp2);
        ftmp1 = (FvsFloat_t) (ix - REF_X);
        ftmp2 = (FvsFloat_t) (iy - REF_Y);
        p_polar_input[n].e = (FvsFloat_t) atan (ftmp2 / ftmp1);
        p_polar_input[n].angle = (itheta - REF_THETA);
    }
    for (n = 0; n < nb_tmplt_minutia; n++) {
        tx = (FvsInt_t)tmplt_minutia[n].x;
        ty = (FvsInt_t)tmplt_minutia[n].y;
        ttheta = (FvsFloat_t) tmplt_minutia[n].angle * (180 / PI);
        ftmp1 = (FvsFloat_t) (tx - REF_X) * (tx - REF_X);
        ftmp2 = (FvsFloat_t) (ty - REF_Y) * (ty - REF_Y);
        p_polar_tmplt[n].r = sqrt (ftmp1 + ftmp2);
        ftmp1 = (FvsFloat_t) (tx - REF_X);
        ftmp2 = (FvsFloat_t) (ty - REF_Y);
        p_polar_tmplt[n].e = (FvsFloat_t) atan (ftmp2 / ftmp1);
        p_polar_tmplt[n].angle = (ttheta - REF_THETA);
    }
    /* ��������ϸ�ڵ� */
    for (n = 0; n < nb_input_minutia; n++) {
        s_polar_input[n].r = p_polar_input[n].r;
        s_polar_input[n].e = p_polar_input[n].e;
        s_polar_input[n].angle = p_polar_input[n].angle;
        fatmp1[n] = p_polar_input[n].angle;
        fatmp2[n] = fatmp1[n];
    }
    Insertion_Sort (&s_polar_input[0], nb_input_minutia);
    for (n = 0; n < nb_input_minutia; n++) {
        s_polar_tmplt[n].r = p_polar_tmplt[n].r;
        s_polar_tmplt[n].e = p_polar_tmplt[n].e;
        s_polar_tmplt[n].angle = p_polar_tmplt[n].angle;
        fatmp1[n] = p_polar_tmplt[n].angle;
        fatmp2[n] = fatmp1[n];
    }
    Insertion_Sort (&s_polar_tmplt[0], nb_tmplt_minutia);
    //  ��� m = 0 �� n = 0����edit ditance = 0
    for (m = 0; m < nb_tmplt_minutia; m++)
        edit_dist[m][0] = 0.0;
    for (n = 0; n < nb_input_minutia; n++)
        edit_dist[0][n] = 0.0;
    for (m = 1; m <= nb_tmplt_minutia; m++) {
        tr = s_polar_tmplt[m].r;
        te = s_polar_tmplt[m].e;
        ttheta = s_polar_tmplt[m].angle;
        for (n = 1; n <= nb_input_minutia; n++) {
            ir = s_polar_input[n].r;
            ie = s_polar_input[n].e;
            itheta = s_polar_input[n].angle;
            // ���㴰�ں��� w(m,n)
            // 1. ������ r
            diff_r = tr - ir;
            // 2. ������ e
            ftmp = te - ie;
            ftmp += 360.0;
            // a = (ie - te + 360) mod 360
            if (ftmp > 360.0) {
                a = ftmp - 360.0;
            }
            else {
                a = 360.0 - ftmp;
            }
            if (a < 180.0) {
                diff_e = a;
            }
            else {
                diff_e = a - 180.0;
            }
            // 3. ������ theta
            ftmp = ttheta - itheta;
            ftmp += 360.0;
            // a = (itheta - ttheta + 360) mod 360
            if (ftmp > 360.0) {
                a = ftmp - 360.0;
            }
            else {
                a = 360.0 - ftmp;
            }
            if (a < 180.0) {
                diff_theta = a;
            }
            else {
                diff_theta = a - 180.0;
            }
            // ���㴰�ں��� w(m,n)
            window_flag = 0;
            if((diff_r < DEL_REF) | (diff_e < SI_REF) | \
                    (diff_theta < EPS_REF)) {
                window_flag = 1;
            }
            if (window_flag == 1) {
                window_mn = (ALPHA * diff_r);
                window_mn += (BETA * diff_e);
                window_mn += (GAMMA * diff_theta);  // w(m,n)
            }
            else {
                window_mn = OHM;                    // w(m,n)
            }
            //=========================
            // ���� edit distance:
            //=========================
            // edit_dist[m][n] = ?
            //=========================
            edit_dist_m1_n  = edit_dist[m - 1][n + 0] + OHM;
            edit_dist_m_n1  = edit_dist[m + 0][n - 1] + OHM;
            edit_dist_m1_n1 = edit_dist[m - 1][n - 1] + window_mn;
            edit_dist_m_n = 0.0;
            if (edit_dist_m1_n < edit_dist_m_n1)
                edit_dist_m_n = edit_dist_m1_n;
            else
                edit_dist_m_n = edit_dist_m_n1;
            if (edit_dist_m1_n1 < edit_dist_m_n)
                edit_dist_m_n = edit_dist_m1_n1;
            edit_dist[m][n] = edit_dist_m_n;
            //printf("%s:%d ... [%d,%d] %f\n", __func__, __LINE__, m, n, edit_dist[m][n]);
        }
    }
    nb_pair = 0;
    for (m = 1; m <= nb_tmplt_minutia; m++) {
        if (edit_dist[m][m] < (float) EDIT_DIST_THRESHOLD)
            nb_pair++;
    }
    nb_minutiae = nb_tmplt_minutia;
    if (nb_input_minutia < nb_tmplt_minutia)
        nb_minutiae = nb_input_minutia;
    Mpq = ((float)100.0 * (float)nb_pair) / (float)nb_minutiae;
    if (Mpq > 70.0)// ����70%��ϸ�ڵ�ƥ��
        printf ("\nTemplated matched with input");
    else
        printf ("\nTemplated not matched with input");
    *pgoodness = Mpq;
    printf ("\nmatching application !!!\n\n\n\n");
    return FvsOK;
}

/* ���������� */
FvsError_t Insertion_Sort (
    Fvs_PolarMinutia_t* v,
    FvsInt_t v_length
) {
    FvsFloat_t item_to_insert1;
    FvsFloat_t item_to_insert2;
    FvsFloat_t item_to_insert3;
    int still_looking, j, k;
    for (k = 1; k < v_length; ++k) {
        item_to_insert1 = v[k].angle;
        item_to_insert2 = v[k].r;
        item_to_insert3 = v[k].e;
        j = k - 1;
        still_looking = 1;
        while ((j >= 0) && (still_looking == 1))  {
            if (item_to_insert1 < v[j].angle) {
                v[j + 1].angle = v[j].angle;
                v[j + 1].r = v[j].r;
                v[j + 1].e = v[j].e;
                v[j].angle = item_to_insert1;
                v[j].r = item_to_insert2;
                v[j].e = item_to_insert3;
                --j;
            }
            else  {
                still_looking = 0;
                v[j + 1].angle = item_to_insert1;
                v[j + 1].r = item_to_insert2;
                v[j + 1].e = item_to_insert3;
            }
        }
    }
    return FvsOK;
}

#endif

#endif
