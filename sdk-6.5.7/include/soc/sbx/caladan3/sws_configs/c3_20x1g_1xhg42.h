
/*
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * $Id: c3_48x1g_2xhg25.h,v 1.1 Broadcom SDK $
 */
#ifdef BCM_CALADAN3_SUPPORT

sws_config_t c3_20x1g_1xhg42_cfg = {


    /*
     * QM Config
     */
     {
        /* Buffer Thresholds */
        {
           15384, 11538, 7692, 11219, 3776, 1539, 2880, 2160, 
           1440, 0, 288, 12195, 9147, 6098, 10895, 1220, 2
        },

        /* Queue Config */
        48, 
        51, 
        {
            {64, 48, 32, 0x3FFF, 17, 17}, /* 0 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 1 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 2 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 3 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 4 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 5 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 6 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 7 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 8 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 9 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 10 */
            {64, 48, 32, 0x3FFF, 17, 17}, /* 11 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 12 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 13 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 14 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 15 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 16 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 17 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 18 */
            {64, 48, 32, 0x3FFF, 17, 17 }, /* 19 */
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
            {271, 204, 136, 121, 17, 17  }, /* 0 */
            {271, 204, 136, 121, 17, 17  }, /* 1 */
            {271, 204, 136, 121, 17, 17  }, /* 2 */
            {271, 204, 136, 121, 17, 17  }, /* 3 */
            {271, 204, 136, 121, 17, 17  }, /* 4 */
            {271, 204, 136, 121, 17, 17  }, /* 5 */
            {271, 204, 136, 121, 17, 17  }, /* 6 */
            {271, 204, 136, 121, 17, 17  }, /* 7 */
            {271, 204, 136, 121, 17, 17  }, /* 8 */
            {271, 204, 136, 121, 17, 17  }, /* 9 */
            {271, 204, 136, 121, 17, 17  }, /* 10 */
            {271, 204, 136, 121, 17, 17  }, /* 11 */
            {271, 204, 136, 121, 17, 17  }, /* 12 */
            {271, 204, 136, 121, 17, 17  }, /* 13 */
            {271, 204, 136, 121, 17, 17  }, /* 14 */
            {271, 204, 136, 121, 17, 17  }, /* 15 */
            {271, 204, 136, 121, 17, 17  }, /* 16 */
            {271, 204, 136, 121, 17, 17  }, /* 17 */
            {271, 204, 136, 121, 17, 17  }, /* 18 */
            {271, 204, 136, 121, 17, 17  }, /* 19 */
            {271, 204, 136, 121, 17, 17  }, /* 20 */
            {271, 204, 136, 121, 17, 17  }, /* 21 */
            {271, 204, 136, 121, 17, 17  }, /* 22 */
            {271, 204, 136, 121, 17, 17  }, /* 23 */
            {271, 204, 136, 121, 17, 17  }, /* 24 */
            {271, 204, 136, 121, 17, 17  }, /* 25 */
            {271, 204, 136, 121, 17, 17  }, /* 26 */
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
                 { 0, 0,2,1 },    /* Port 0 */
                 { 1,16,2,1 },    /* Port 1 */
                 { 2,32,2,1 },    /* Port 2 */
                 { 3,48,2,1 },    /* Port 3 */
                 { 4,64,2,1 },    /* Port 4 */
                 { 5,80,2,1 },    /* Port 5 */
                 { 6,96,2,1 },    /* Port 6 */
                 { 7,112,2,1 },    /* Port 7 */
                 { 8,128,2,1 },    /* Port 8 */
                 { 9,144,2,1 },    /* Port 9 */
                 {10,160,2,1 },    /* Port 10 */
                 {11,176,2,1 },    /* Port 11 */
                 {12,192,2,1 },    /* Port 12 */
                 {13,208,2,1 },    /* Port 13 */
                 {14,224,2,1 },    /* Port 13 */
                 {15,240,2,1 },    /* Port 14 */
                 {16,256,2,1 },    /* Port 15 */
                 {17,272,2,1 },    /* Port 16 */
                 {18,288,2,1 },    /* Port 17 */
                 {19,304,2,1 },    /* Port 18 */
                 {20,320,2,1 },    /* Port 19 */
                 {21,336,2,1 },    /* Port 20 */
                 {22,352,2,1 },    /* Port 21 */
                 {23,368,2,1 },    /* Port 22 */
                 {24,384,2,1 },    /* Port 23 */
                 {25,400,2,1 },    /* Port 24 */
                 {26,416,2,1 },    /* Port 25 */
                 {27,432,2,1 },    /* Port 26 */
                 {28,448,2,1 },    /* Port 27 */
                 {29,464,2,1 },    /* Port 28 */
                 {30,480,2,1 },    /* Port 29 */
                 {31,496,2,1 },    /* Port 30 */
                 {32,512,2,1 },    /* Port 31 */
                 {33,528,2,1 },    /* Port 32 */
                 {34,544,2,1 },    /* Port 33 */
                 {35,560,2,1 },    /* Port 34 */
                 {36,576,2,1 },    /* Port 35 */
                 {37,592,2,1 },    /* Port 36 */
                 {38,608,2,1 },    /* Port 37 */
                 {39,624,2,1 },    /* Port 38 */
                 {40,640,2,1 },    /* Port 39 */
                 {41,656,2,1 },    /* Port 40 */
                 {42,672,2,1 },    /* Port 41 */
                 {43,688,2,1 },    /* Port 43 */
                 {44,704,2,1 },    /* Port 44 */
                 {45,720,2,1 },    /* Port 45 */
                 {46,736,2,1 },    /* Port 46 */
                 {47,752,2,1 },    /* Port 47 */
                 {48,768,2,1 },    /* Port 48 */
                 {49,784,2,1 },    /* Port 49 */
                 {50,800,2,1 },    /* Port 50 */
              }
        },

        /* Client Cal */
        {
            78,
            {
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT4, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT4, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT5, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0,
              CLIENT0, CLIENT4, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT0, CLIENT0, CLIENT0, 
              CLIENT0, CLIENT0, CLIENT5 
            },
        },

        /* Port Cal */
        {
            99,
            {
              0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 48,
              0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 49,
              0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 50,
              0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0,
              1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1,
              2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 
              3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 
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
            12,
            {
                {0,0,26,7},    /* Port 0 */
                 { 1,208,2,1 },    /* Port 1 */
                 { 2,224,2,1 },    /* Port 2 */
                 { 3,240,2,1 },    /* Port 3 */
                 { 4,256,2,1 },    /* Port 4 */
                 { 5,272,2,1 },    /* Port 5 */
                 { 6,288,2,1 },    /* Port 6 */
                 { 7,304,2,1 },    /* Port 7 */
                 { 8,320,2,1 },    /* Port 8 */
                 { 9,336,2,1 },    /* Port 9 */
                 {10,352,2,1 },    /* Port 10 */
                 {11,368,2,1 },    /* Port 11 */
                 {12,384,2,1 },    /* Port 12 */
                 {13,400,2,1 },    /* Port 13 */
                 {14,416,2,1 },    /* Port 13 */
                 {15,432,2,1 },    /* Port 14 */
                 {16,448,2,1 },    /* Port 15 */
                 {17,464,2,1 },    /* Port 16 */
                 {18,480,2,1 },    /* Port 17 */
                 {19,496,2,1 },    /* Port 18 */
                 {20,512,2,1 },    /* Port 19 */
                 {21,528,2,1 },    /* Port 20 */
                 {22,544,2,1 },    /* Port 21 */
                 {23,560,2,1 },    /* Port 22 */
                 {24,576,2,1 },    /* Port 23 */
                 {25,592,2,1 },    /* Port 24 */
                 {26,608,2,1 },    /* Port 25 */
                 {27,624,2,1 },    /* Port 26 */
                 {28,640,2,1 },    /* Port 27 */
                 {29,656,2,1 },    /* Port 28 */
                 {30,672,2,1 },    /* Port 29 */
                 {31,688,2,1 },    /* Port 30 */
                 {32,704,2,1 },    /* Port 31 */
                 {33,720,2,1 },    /* Port 32 */
                 {34,736,2,1 },    /* Port 33 */
                 {35,752,2,1 },    /* Port 34 */
                 {36,576,2,1 },    /* Port 35 */
                 {37,592,2,1 },    /* Port 36 */
                 {38,608,2,1 },    /* Port 37 */
                 {39,624,2,1 },    /* Port 38 */
                 {40,0,0,0 },    /* Port 39 */
                 {41,0,0,0 },    /* Port 40 */
                 {42,0,0,0 },    /* Port 41 */
                 {43,0,0,0 },    /* Port 43 */
                 {44,0,0,0 },    /* Port 44 */
                 {45,0,0,0 },    /* Port 45 */
                 {46,0,0,0 },    /* Port 46 */
                 {47,0,0,0 },    /* Port 47 */
                 {48,768,2,1 },    /* Port 48 */
                 {49,784,2,1 },    /* Port 49 */
                 {50,800,2,1 },    /* Port 50 */
            }
        },

        /* Client Cal */
        {
            2,
            {
              CLIENT0, CLIENT0
            },
        },

        /* Port Cal */
        {
            56,
            {
                 0, 0, 0, 0, 0, 0, 4,
                 0, 0, 0, 0, 0, 0, 5,
                 0, 0, 0, 0, 0, 0, 6,
                 0, 0, 0, 0, 0, 0, 7,
                 0, 0, 0, 0, 0, 0, 8,
                 0, 0, 0, 0, 0, 0, 9,
                 0, 0, 0, 0, 0, 0, 10,
                 0, 0, 0, 0, 0, 0, 11,
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
                { 0,  0, 0},
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
            12,
            {
                {0, 0, 0x64},
                {1, 0, 0},
                {2, 0, 0},
                {3, 0, 0},
                {4, 0x130, 16 },
                {5, 0x140, 16 },
                {6, 0x150, 16 },
                {7, 0x160, 16 },
                {8, 0x170, 16 },
                {9, 0x180, 16 },
                {10, 0x190, 16 },
                {11, 0x1a0, 16 },
            }
        },

        /* IDP Thresholds */
        { 255, 192, 128, 0, 227 },
    },


};

#endif
