
/*
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * $Id: c3_1x40g_8x10g_1xhg127.h,v 1.1.2.1 Broadcom SDK $
 */
#ifdef BCM_CALADAN3_SUPPORT

sws_config_t c3_1x40g_8x10g_1xhg127_cfg = {


    /*
     * QM Config
     */
     {
        /* Buffer Thresholds */
        {
           15384, 11538, 7692, 11291, 3728, 1539, 3216, 2412, 
           1608, 0, 322, 11861, 8896, 5931, 10616, 1187, 38
        },

        /* Queue Config */
        9, 
        9, 
        {
            {204, 153, 102, 0, 52, 52}, /* 0 */
            { 0 }, /* 1 */
            { 0 }, /* 2 */
            { 0 }, /* 3 */
            {204, 153, 102, 0, 52, 52}, /* 4 */
            {204, 153, 102, 0, 52, 52}, /* 5 */
            {204, 153, 102, 0, 52, 52}, /* 6 */
            {204, 153, 102, 0, 52, 52}, /* 7 */
            {204, 153, 102, 0, 52, 52}, /* 8 */
            {204, 153, 102, 0, 52, 52}, /* 9 */
            {204, 153, 102, 0, 52, 52}, /* 10 */
            {204, 153, 102, 0, 52, 52}, /* 11 */
            { 0 }, /* 12 */
            { 0 }, /* 13 */
            { 0 }, /* 14 */
            { 0 }, /* 15 */
            { 0 }, /* 16 */
            { 0 }, /* 17 */
            { 0 }, /* 18 */
            { 0 }, /* 19 */
            { 0 }, /* 20 */
            { 0 }, /* 21 */
            { 0 }, /* 22 */
            { 0 }, /* 23 */
            { 0 }, /* 24 */
            { 0 }, /* 25 */
            { 0 }, /* 26 */
            { 0 }, /* 27 */
            { 0 }, /* 28 */
            { 0 }, /* 29 */
            { 0 }, /* 30 */
            { 0 }, /* 31 */
            { 0 }, /* 32 */
            { 0 }, /* 33 */
            { 0 }, /* 34 */
            { 0 }, /* 35 */
            { 0 }, /* 36 */
            { 0 }, /* 37 */
            { 0 }, /* 38 */
            { 0 }, /* 39 */
            { 0 }, /* 40 */
            { 0 }, /* 41 */
            { 0 }, /* 42 */
            { 0 }, /* 43 */
            { 0 }, /* 44 */
            { 0 }, /* 45 */
            { 0 }, /* 46 */
            { 0 }, /* 47 */
            { 0 }, /* 48 */
            { 0 }, /* 49 */
            { 0 }, /* 50 */
            { 0 }, /* 51 */
            { 0 }, /* 52 */
            { 0 }, /* 53 */
            { 0 }, /* 54 */
            { 0 }, /* 55 */
            { 0 }, /* 56 */
            { 0 }, /* 57 */
            { 0 }, /* 58 */
            { 0 }, /* 59 */
            { 0 }, /* 60 */
            { 0 }, /* 61 */
            { 0 }, /* 62 */
            { 0 }, /* 63 */
        },
        {
            {638, 479, 319, 229, 49, 49}, /* 0 */
            { 0 }, /* 1 */
            { 0 }, /* 2 */
            { 0 }, /* 3 */
            {638, 479, 319, 229, 49, 49}, /* 4 */
            {638, 479, 319, 229, 49, 49}, /* 5 */
            {638, 479, 319, 229, 49, 49}, /* 6 */
            {638, 479, 319, 229, 49, 49}, /* 7 */
            {459, 345, 230, 122, 24, 24}, /* 8 */
            {459, 345, 230, 122, 24, 24}, /* 9 */
            {459, 345, 230, 122, 24, 24}, /* 10 */
            {459, 345, 230, 122, 24, 24}, /* 11 */
            { 0 }, /* 12 */
            { 0 }, /* 13 */
            { 0 }, /* 14 */
            { 0 }, /* 15 */
            { 0 }, /* 16 */
            { 0 }, /* 17 */
            { 0 }, /* 18 */
            { 0 }, /* 19 */
            { 0 }, /* 20 */
            { 0 }, /* 21 */
            { 0 }, /* 22 */
            { 0 }, /* 23 */
            { 0 }, /* 24 */
            { 0 }, /* 25 */
            { 0 }, /* 26 */
            { 0 }, /* 27 */
            { 0 }, /* 28 */
            { 0 }, /* 29 */
            { 0 }, /* 30 */
            { 0 }, /* 31 */
            { 0 }, /* 32 */
            { 0 }, /* 33 */
            { 0 }, /* 34 */
            { 0 }, /* 35 */
            { 0 }, /* 36 */
            { 0 }, /* 37 */
            { 0 }, /* 38 */
            { 0 }, /* 39 */
            { 0 }, /* 40 */
            { 0 }, /* 41 */
            { 0 }, /* 42 */
            { 0 }, /* 43 */
            { 0 }, /* 44 */
            { 0 }, /* 45 */
            { 0 }, /* 46 */
            { 0 }, /* 47 */
            { 0 }, /* 48 */
            { 0 }, /* 49 */
            { 0 }, /* 50 */
            { 0 }, /* 51 */
            { 0 }, /* 52 */
            { 0 }, /* 53 */
            { 0 }, /* 54 */
            { 0 }, /* 55 */
            { 0 }, /* 56 */
            { 0 }, /* 57 */
            { 0 }, /* 58 */
            { 0 }, /* 59 */
            { 0 }, /* 60 */
            { 0 }, /* 61 */
            { 0 }, /* 62 */
            { 0 }, /* 63 */
        }, 

        /* Cmic queues */
        {258, 194, 129, 0, 64, 64},

        /* XL queues */
        { {256, 192, 128, 0, 64, 64}, {256, 192, 128, 0, 64, 64} },

        /* Ingress Redirection Queues */
        { {128, 96, 64, 0, 0, 64}, {128, 96, 64, 0, 0, 64} },

        /* Egress Redirection Queues */
        { {128, 96, 64, 0, 0, 64}, {128, 96, 64, 0, 0, 64} },

        /* Ingress Bubble Queues */
        {128, 96, 64, 0, 0, 64},

        /* Egress Bubble Queues */
        {128, 96, 64, 0, 0, 64},

    },

    /*
     * Line PT Config
     */
    {
        /* Fifo info */
        {
            51,
            {
                {0,0,26,7},    /* Port 0 */
                {1,0,0,0},    /* Port 1 */
                {2,0,0,0},    /* Port 2 */
                {3,0,0,0},    /* Port 3 */
                {4,256,8,2},    /* Port 4 */
                {5,320,8,2},    /* Port 5 */
                {6,384,8,2},    /* Port 6 */
                {7,448,8,2},    /* Port 7 */
                {8,512,8,2},    /* Port 8 */
                {9,576,8,2},    /* Port 9 */
                {10,640,8,2},    /* Port 10 */
                {11,704,8,2},    /* Port 11 */
                {12,0,0,0},    /* Port 12 */
                {13,0,0,0},    /* Port 13 */
                {14,0,0,0},    /* Port 14 */
                {15,0,0,0},    /* Port 15 */
                {16,0,0,0},    /* Port 16 */
                {17,0,0,0},    /* Port 17 */
                {18,0,0,0},    /* Port 18 */
                {19,0,0,0},    /* Port 19 */
                {20,0,0,0},    /* Port 20 */
                {21,0,0,0},    /* Port 21 */
                {22,0,0,0},    /* Port 22 */
                {23,0,0,0},    /* Port 23 */
                {24,0,0,0},    /* Port 24 */
                {25,0,0,0},    /* Port 25 */
                {26,0,0,0},    /* Port 26 */
                {27,0,0,0},    /* Port 27 */
                {28,0,0,0},    /* Port 28 */
                {29,0,0,0},    /* Port 29 */
                {30,0,0,0},    /* Port 30 */
                {31,0,0,0},    /* Port 31 */
                {32,0,0,0},    /* Port 32 */
                {33,0,0,0},    /* Port 33 */
                {34,0,0,0},    /* Port 34 */
                {35,0,0,0},    /* Port 35 */
                {36,0,0,0},    /* Port 36 */
                {37,0,0,0},    /* Port 37 */
                {38,0,0,0},    /* Port 38 */
                {39,0,0,0},    /* Port 39 */
                {40,0,0,0},    /* Port 40 */
                {41,0,0,0},    /* Port 41 */
                {42,0,0,0},    /* Port 42 */
                {43,0,0,0},    /* Port 43 */
                {44,0,0,0},    /* Port 44 */
                {45,0,0,0},    /* Port 45 */
                {46,0,0,0},    /* Port 46 */
                {47,0,0,0},    /* Port 47 */
                {48,768,2,1},    /* Port 48 */
                {49,784,2,1},    /* Port 49 */
                {50,800,2,1},    /* Port 50 */
            }
        },

        /* Client Cal */
        {
            60,
            {
                 CLIENTX, CLIENT3, CLIENT0, CLIENT2, CLIENT0, 
                 CLIENT0, CLIENT3, CLIENT0, CLIENT0, CLIENT4, 
                 CLIENT0, CLIENT0, CLIENT3, CLIENT0, CLIENT0, 
                 CLIENTX, CLIENT0, CLIENT0, CLIENT3, CLIENT0, 
                 CLIENT0, CLIENT2, CLIENT0, CLIENT0, CLIENT3, 
                 CLIENT0, CLIENT0, CLIENTX, CLIENT0, CLIENT0, 
                 CLIENT3, CLIENT0, CLIENT0, CLIENT2, CLIENT0, 
                 CLIENT0, CLIENT3, CLIENT0, CLIENT0, CLIENT4, 
                 CLIENT0, CLIENT0, CLIENT3, CLIENT0, CLIENT0, 
                 CLIENT2, CLIENT0, CLIENT3, CLIENT0, CLIENT0, 
                 CLIENT5, CLIENT0, CLIENT3, CLIENT0, CLIENT0, 
                 CLIENT2, CLIENT0, CLIENT3, CLIENT0, CLIENT0
            },
        },

        /* Port Cal */
        {
            99,
            {
              0, 0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 11, 48,
              0, 0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 11, 49,
              0, 0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 11, 50,
              0, 0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 11, 0,
              0, 0, 0, 4, 5, 6, 7, 8, 9, 10, 11, 0, 0,
              0, 0, 4, 5, 6, 7, 8, 9, 10, 11, 0, 0, 0,
              0, 4, 5, 6, 7, 8, 9, 10, 11, 0, 0, 0, 0,
              4, 5, 6, 7, 8, 9, 10, 11
            },
        },
    },
    /*
     * Fabric PT Config
     */
    {
        /* Fifo info */
        {
             1, 
             {
                 { 0,0,76,20 },    /* Port 0*/
              }
        },

        /* Client Cal */
        {
            72,
            {
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
                 CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
                 CLIENT0, CLIENT0, CLIENT0, CLIENTX,
            },
        },

        /* Port Cal */
        {
            108,
            {
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
              0, 0
            },
        },
    },

    /*
     * Line PR config
     */
    {

        /* Rx buf config */
        {
            0,
            {
                {0, 0, 0} 
            },
        },

        /* IDP Thresholds */
        { 255, 192, 128, 0, 0 },
    },

    /*
     * Fabric PR config
     */
    {

        /* Rx buf config */
        {
            0,
            {
                {0, 0, 0}
            }
        },

        /* IDP Thresholds */
        { 255, 192, 128, 0, 196 },
    },


};

#endif
