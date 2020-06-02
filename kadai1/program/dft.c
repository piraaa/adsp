#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define pi 3.1415926535897932384626433832795

//comp型として複素数用構造体を用意
typedef struct{
	double re; //実部用
	double im; //虚部用
}comp;


// 関数のプロトタイプを宣言
comp *dft(comp *, comp *, int, int);//この型でDFTの関数を作成します．

//ここからプログラムのメイン/////////////////////////////////////////////////
int main()
{
	//各種変数を定義///////////////////////////////////////////////////
	char in_name[128], out_name[128]; //入力・出力ファイルの名前保存用配列
	char buff[128]; //ファイルの行数チェック用の配列
	long sample=0; //サンプル数カウント用変数
	comp *xn, *Xn; //複素数型でxn，Xn配列を作り，xnに元データを，XnにDFT・IDFT結果を入れます．
	int check=0, flg=0, c, i, N, inv;
	long count=0, sum=0;

	FILE *read; //読み込み用ファイルポインタの名前をreadに
	FILE *write; //書き込み用ファイルポインタの名前をwriteに


	//画面表示で説明を記載/////////////////////////////////////////////
	printf("********************************************************\n");
	printf("*  TEXTデータをDFT(IDFT)します                         *\n");
	printf("*    入力データファイル                                *\n");
	printf("*    (DFTのときは1列)                                  *\n");
	printf("*    (IDFTのときは実部と虚部の2列:タブまたはスペース)  *\n");
	printf("*    DFT(IDFT)出力ファイル                             *\n");
	printf("*    (DFTのときは実部と虚部の2列:タブ)                 *\n");
	printf("*    (IDFTのときは1列)                                 *\n");
	printf("********************************************************\n\n\n");

	//DFTかIDFTかを1か2で選択します．cに1か2が保存されます///////////////
	printf("DFT -> 1 or IDFT -> 2\n"); scanf("%d",&c);
	if(c!=1 && c!=2){
		printf("入力をやり直してください\n");
		exit(-1);
	}

	//cが1(DFT)のときはinvに1を保存，cが2のときはinvに-1を保存します．////
	if(c==1) inv = 1;
	else inv = -1;

	//DFTの点数をNに保存します////////////////////////////////////////////
	printf("点数Nを入力してください -> "); scanf("%d",&N);

	/////////////////////////////////////////////////////
	// ファイルのオープン
	/////////////////////////////////////////////////////
	printf("入力ファイル名を記入してください -> ");
	scanf("%s",in_name);
	if((read=fopen(in_name,"r"))==NULL) exit(-1);

	printf("出力ファイル名を記入してください -> ");
	scanf("%s",out_name);
	if((write=fopen(out_name,"w"))==NULL) exit(-1);

	/////////////////////////////////////////////////////
	// 配列確保
	/////////////////////////////////////////////////////
	printf("ファイルチェック中\n");

	while(fgets(buff,256,read) != NULL) sample++; //ファイルの行数チェック

	xn=(comp *)calloc(N,sizeof(comp)); if(!xn) exit(-1);
	Xn=(comp *)calloc(N,sizeof(comp)); if(!Xn) exit(-1);

	printf("ファイルデータ数%ld\n",sample);
	printf("%d点FFT(IFFT)を行います\n",N);

	/////////////////////////////////////////////////////
	// データがスペースかタブで区切られている場合は複素形式として
	/////////////////////////////////////////////////////
	fseek(read, 0, SEEK_SET);
	fgets(buff, 128, read);
	i=0;
	while(buff[i]!='\n'){
		if(buff[i]==' ' || buff[i]=='\t') flg=1;
		i++;
	}

	/////////////////////////////////////////////////////
	// データ読み込み
	/////////////////////////////////////////////////////
	if(sample>N) sample=N; //サンプルがNより大きい場合はNとする
	fseek(read, 0, SEEK_SET);
	if(flg==0){
		for(i=0; i<sample; i++){
			fscanf(read,"%lf",&xn[i].re); //実部
		}
	}else{
		for(i=0; i<sample; i++){
			fscanf(read,"%lf",&xn[i].re); //実部
			fscanf(read,"%lf",&xn[i].im); //虚部
		}
	}

	/////////////////////////////////////////////////////
	// ここでDFT・IDFTの関数を呼び出します
	/////////////////////////////////////////////////////
	Xn = dft(xn, Xn, N, inv); //DFT

	/////////////////////////////////////////////////////
	// 出力
	/////////////////////////////////////////////////////
	if(inv == 1){
		for(i=0; i<N; i++){
			fprintf(write,"%lf \t %lf\n",Xn[i].re,Xn[i].im);
		}
	}else{
		for(i=0; i<N; i++){
			fprintf(write,"%.15lf\n",Xn[i].re);
		}
	}

	free(xn);
	free(Xn);

	fclose(read);
	fclose(write);

	getchar();
}

/////////////////////////////////////////////////////
// DFTのアルゴリズム
// in：データ系列を受け取り
// out：計算結果を出力
// N：データ数
// inv：DFTかIDFTの選択(1 or -1)
/////////////////////////////////////////////////////
comp *dft(comp *in, comp *out, int N, int inv)
{
	int i,j;

	//DFT(IDFT)
	for(i=0; i<N; i++){
		out[i].re=0.0;
		out[i].im=0.0;
		for(j=0; j<N; j++){
			out[i].re += in[j].re*cos(2.0*pi*i*j/N) + inv*in[j].im*sin(2.0*pi*i*j/N);
			out[i].im += in[j].im*cos(2.0*pi*i*j/N) - inv*in[j].re*sin(2.0*pi*i*j/N);
		}

		// IDFTの時はNで割る
		if(inv == -1){
			out[i].re /= (double)N;
			out[i].im /= (double)N;
		}
	}

	return out;
}