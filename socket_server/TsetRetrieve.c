#include "Head.h"

Word* TsetRetrieve(Pair(*Tset)[128], unsigned char *Stag) {

	int i, j, m, z;
	char* b = (char*)malloc(sizeof(char));
	unsigned char* Beta = (char*)malloc(sizeof(unsigned char));
	unsigned char res;
	unsigned char L[10] = { '\0' };
	unsigned char K[17] = { '\0' };
	unsigned char smallv[32] = { '\0' };//임시배열
	unsigned char EncryptStag[16] = { "\0" };//암호화된 Stag
	unsigned char hashStag[32] = { '\0' }; //hash한 Stag
	unsigned char tmpXor[32] = { '\0' };//추가
	unsigned char x[16] = { '\0' }; //i값을 AES시킬때 인풋

	struct Flags f;
	

	Pair T[128];
	Word V[4];

	SHA256_CTX ctx;

	memset(T, NULL, sizeof(T));
	memset(V, NULL, sizeof(V));

	strncpy(Beta, "1", 1); //베타를 1로 설정
	f.b = *Beta;//Beta의 1비트값 저장
	
	i = 0;

	while (f.b == 1) {

		AES_ECB_Encrypt(Stag, _itoa(i, x, 16), EncryptStag, 16); //Stag를 AES
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, EncryptStag, 16);
		SHA256_Final(hashStag, &ctx);
		
		memcpy(b, hashStag, 1); //b추출

		f.a = *b;//문자 b값을 정수로변환
		if (f.a < 0) f.a = -(f.a) - 1;
		memcpy(L, hashStag + 1, 10);//hash된 Stag 1-10까지를 L로저장
		memcpy(K, (hashStag + 11), 17);//hash된 Stag 11-17까지를 K로저장
		memmove(T, Tset + (f.a), 128 * (10 + 17)); //T에 Tset[b]값 통째로 복사
		
		
		for (j = 0; j < 128; j++) {
			if (memcmp((T[j].label), L, 10) == 0) {//L과 label이 맞는 j값 선택
				memcpy(tmpXor, T[j].value, 17); 
				for (m = 0; m < 17; m++) {
					res = tmpXor[m] ^ K[m]; //K값을 xor해서 암호화된 Beta || id값 smallv에 저장
					memcpy(smallv + m, &res, 1);
				}
				memcpy(Beta, smallv, 1); //Beta에 smallv의 첫번째 바이트 저장
				f.b = *Beta;
				memcpy((V + i)->a, smallv + 1, 16);
				break;
			}
			
		}
		memset(tmpXor, NULL, sizeof(tmpXor));
		memset(smallv, NULL, sizeof(smallv));//추가
		memset(T, NULL, sizeof(T));//추가
		
		i++;
	}
	
	return V;
}

 