#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795

// プロトタイプ宣言
void dct(double **, double **, int N, int mode);
void dct2(double **, double **, int N, int mode);

int main()
{
	char in_name[128], out_name[128]; //入出力ファイル名
	char buff[128]; //ファイルの行数チェック用の配列
	long sample = 0; //サンプル数カウント用変数
	int  i, j;
	int N; //点数
	double **xn, **Xn; //入出力データ(1次元の時はarray[0][n]だけ使う)
	int mode; // DCTは1，IDCTは2
	int flag = 1; // 1次元は1，2次元は2

	FILE *read; //読み込み用ファイルポインタ
	FILE *write; //書き込み用ファイルポインタ


	// 画面表示で説明を記載
	printf("**************************************************************************\n");
	printf(" TEXTデータをDCT(IDCT)します\n");
	printf(" 入力データファイル\n");
	printf("  -> 1次元または2次元の実数配列 (2次元の場合はスペースかタブで区切る)\n");
	printf(" 出力ファイル\n");
	printf("  -> 1次元または2次元の実数配列\n\n");
	printf(" 機能限定版\n");
	printf(" 2次元DCT(IDCT)は正方形(縦と横のピクセルが同じ)限定\n");
	printf("**************************************************************************\n\n");

	// DCTかIDCTかを1か2で選択
	printf("DCT -> 1 or IDCT -> 2\n");
	scanf("%d", &mode);
	if(mode!=1 && mode!=2){
		printf("入力をやり直してください\n");
		exit(-1);
	}

	// DCTの点数N
	printf("点数Nを入力してください -> ");
	scanf("%d", &N);
	
	// ファイル名の取得とファイルのオープン
	printf("入力ファイル名を記入してください -> ");
	scanf("%s", in_name);
	if((read=fopen(in_name, "r"))==NULL){
		printf("ファイルが見つかりません．\n");
		exit(-1);
	}

	printf("出力ファイル名を記入してください -> ");
	scanf("%s", out_name);
	if((write=fopen(out_name, "w"))==NULL) exit(-1);

	// 動的配列確保
	printf("ファイルの行数チェック中\n");
	while(fgets(buff, 256, read) != NULL) sample++;

	xn=(double **)calloc(N,sizeof(double));
	if(!xn) exit(-1);
	for(i=0; i<N; i++){
		xn[i] = (double *)calloc(N, sizeof(double));
		if(!xn) exit(-1);
	}

	Xn=(double **)calloc(N,sizeof(double));
	if(!Xn) exit(-1);
	for(i=0; i<N; i++){
		Xn[i] = (double *)calloc(N, sizeof(double));
		if(!Xn) exit(-1);
	}

	printf("ファイルデータ数%ld\n" ,sample);
	printf("%d点DCT(IDCT)を行います\n", N);

	// データがスペースかタブで区切られている場合は2次元DCTと判定
	fseek(read, 0, SEEK_SET);
	fgets(buff, 128, read);
	i = 0;
	while(buff[i]!='\n'){
		if(buff[i]==' ' || buff[i]=='\t') flag=2;
		i++;
	}

	// データ読み込み
	if(sample>N) sample=N; //サンプルがNより大きい場合はN個目まで

	fseek(read, 0, SEEK_SET);
	if(flag==1){ // 1次元
		for(i=0; i<sample; i++) fscanf(read, "%lf", &xn[0][i]);
	}else{ // 2次元
		for(i=0; i<sample; i++){
			for(j=0; j<sample; j++){
				fscanf(read, "%lf", &xn[i][j]);
			}
		}
	}

	// DCT
	if(flag == 1){
		printf("1次元DCT(IDCT)を実行しています．\n");
		dct(xn, Xn, N, mode);
	}else{
		printf("2次元DCT(IDCT)を実行しています．\n");
		dct2(xn, Xn, N, mode);
	}

	printf("DCT(IDCT)完了．\n");

	// データ出力
	if(flag == 1){
		for(i=0; i<N; i++){
			fprintf(write, "%.10lf\n", Xn[0][i]);
		}
	}else{
		for(i=0; i<N; i++){
			for(j=0; j<N; j++){
				fprintf(write, "%.10lf ", Xn[i][j]);
			}
			fprintf(write, "\n");
		}
	}

	printf("DCT結果を %s に出力しました．\n", out_name);

	free(xn);
	free(Xn);

	fclose(read);
	fclose(write);

	getchar();
}

/////////////////////////////////////////////////////
// DCTのアルゴリズム
// in：データ系列を受け取り
// out：計算結果を出力
// N：データ数
// mode：DCTかIDCTの選択(1 or 2)
/////////////////////////////////////////////////////
void dct(double **in, double **out, int N, int mode){
	int i,j;
	double tmp;
	double **c;

	c = (double **)calloc(N, sizeof(double));
	for(i=0; i<N; i++) c[i] = (double *)calloc(N, sizeof(double));

	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			if(i==0){
				c[i][j] = 1/sqrt(N);
			}else{
				c[i][j] = sqrt(2/(double)N) * cos((2*j+1)*i*PI/(2*N));
			}
		}
	}

	// IDCTだったら転置
	if(mode==2){
		for(i=0; i<N; i++){
			for(j=i+1; j<N; j++){
				tmp = c[i][j];
				c[i][j] = c[j][i];
				c[j][i] = tmp;
			}
		}
	}

	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			out[0][i] += in[0][j]*c[i][j];
		}
	}

	free(c);
}

void dct2(double **in, double **out, int N, int mode){
	int i,j,k;
	double tmp;
	double **c, **ans;

	c = (double **)calloc(N, sizeof(double));
	for(i=0; i<N; i++) c[i] = (double *)calloc(N, sizeof(double));

	ans = (double **)calloc(N, sizeof(double));
	for(i=0; i<N; i++) ans[i] = (double *)calloc(N, sizeof(double));

	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			if(i==0){
				c[i][j] = 1/sqrt(N);
			}else{
				c[i][j] = sqrt(2/(double)N) * cos((2*j+1)*i*PI/(2*N));
			}
		}
	}

	// IDCTだったら転置
	if(mode==2){
		for(i=0; i<N; i++){
			for(j=i+1; j<N; j++){
				tmp = c[i][j];
				c[i][j] = c[j][i];
				c[j][i] = tmp;
			}
		}
	}

	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			for(k=0; k<N; k++){
				out[i][j] += c[i][k]*in[k][j];
			}
		}
	}

	// 転置
	for(i=0; i<N; i++){
		for(j=i+1; j<N; j++){
			tmp = c[i][j];
			c[i][j] = c[j][i];
			c[j][i] = tmp;
		}
	}

	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			for(k=0; k<N; k++){
				ans[i][j] += out[i][k]*c[k][j];
			}
		}
	}

	//ansからXnへコピー
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			out[i][j] = ans[i][j];
		}
	}

	free(c);
	free(ans);
}