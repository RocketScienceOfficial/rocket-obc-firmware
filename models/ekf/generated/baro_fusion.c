

H_Fusion[0] = 0;
H_Fusion[1] = 0;
H_Fusion[2] = 0;
H_Fusion[3] = 0;
H_Fusion[4] = 0;
H_Fusion[5] = 0;
H_Fusion[6] = 0;
H_Fusion[7] = 0;
H_Fusion[8] = 0;
H_Fusion[9] = 1;
H_Fusion[10] = 0;
H_Fusion[11] = 0;
H_Fusion[12] = 0;
H_Fusion[13] = 0;
H_Fusion[14] = 0;
H_Fusion[15] = 0;
H_Fusion[16] = 0;
H_Fusion[17] = 0;
H_Fusion[18] = 0;
H_Fusion[19] = 0;
H_Fusion[20] = 0;
H_Fusion[21] = 0;


const float KS0 = 1.0F/(P[9][9] + R_BARO);


K_Fusion[0] = KS0*P[0][9];
K_Fusion[1] = KS0*P[1][9];
K_Fusion[2] = KS0*P[2][9];
K_Fusion[3] = KS0*P[3][9];
K_Fusion[4] = KS0*P[4][9];
K_Fusion[5] = KS0*P[5][9];
K_Fusion[6] = KS0*P[6][9];
K_Fusion[7] = KS0*P[7][9];
K_Fusion[8] = KS0*P[8][9];
K_Fusion[9] = KS0*P[9][9];
K_Fusion[10] = KS0*P[9][10];
K_Fusion[11] = KS0*P[9][11];
K_Fusion[12] = KS0*P[9][12];
K_Fusion[13] = KS0*P[9][13];
K_Fusion[14] = KS0*P[9][14];
K_Fusion[15] = KS0*P[9][15];
K_Fusion[16] = KS0*P[9][16];
K_Fusion[17] = KS0*P[9][17];
K_Fusion[18] = KS0*P[9][18];
K_Fusion[19] = KS0*P[9][19];
K_Fusion[20] = KS0*P[9][20];
K_Fusion[21] = KS0*P[9][21];


