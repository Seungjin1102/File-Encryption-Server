#include "Head.h"

Word* TsetRetrieve(Pair(*Tset)[128], unsigned char *Stag) {

	int i, j, m, z;
	char* b = (char*)malloc(sizeof(char));
	unsigned char* Beta = (char*)malloc(sizeof(unsigned char));
	unsigned char res;
	unsigned char L[10] = { '\0' };
	unsigned char K[17] = { '\0' };
	unsigned char smallv[32] = { '\0' };//�ӽù迭
	unsigned char EncryptStag[16] = { "\0" };//��ȣȭ�� Stag
	unsigned char hashStag[32] = { '\0' }; //hash�� Stag
	unsigned char tmpXor[32] = { '\0' };//�߰�
	unsigned char x[16] = { '\0' }; //i���� AES��ų�� ��ǲ

	struct Flags f;
	

	Pair T[128];
	Word V[4];

	SHA256_CTX ctx;

	memset(T, NULL, sizeof(T));
	memset(V, NULL, sizeof(V));

	strncpy(Beta, "1", 1); //��Ÿ�� 1�� ����
	f.b = *Beta;//Beta�� 1��Ʈ�� ����
	
	i = 0;

	while (f.b == 1) {

		AES_ECB_Encrypt(Stag, _itoa(i, x, 16), EncryptStag, 16); //Stag�� AES
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, EncryptStag, 16);
		SHA256_Final(hashStag, &ctx);
		
		memcpy(b, hashStag, 1); //b����

		f.a = *b;//���� b���� �����κ�ȯ
		if (f.a < 0) f.a = -(f.a) - 1;
		memcpy(L, hashStag + 1, 10);//hash�� Stag 1-10������ L������
		memcpy(K, (hashStag + 11), 17);//hash�� Stag 11-17������ K������
		memmove(T, Tset + (f.a), 128 * (10 + 17)); //T�� Tset[b]�� ��°�� ����
		
		
		for (j = 0; j < 128; j++) {
			if (memcmp((T[j].label), L, 10) == 0) {//L�� label�� �´� j�� ����
				memcpy(tmpXor, T[j].value, 17); 
				for (m = 0; m < 17; m++) {
					res = tmpXor[m] ^ K[m]; //K���� xor�ؼ� ��ȣȭ�� Beta || id�� smallv�� ����
					memcpy(smallv + m, &res, 1);
				}
				memcpy(Beta, smallv, 1); //Beta�� smallv�� ù��° ����Ʈ ����
				f.b = *Beta;
				memcpy((V + i)->a, smallv + 1, 16);
				break;
			}
			
		}
		memset(tmpXor, NULL, sizeof(tmpXor));
		memset(smallv, NULL, sizeof(smallv));//�߰�
		memset(T, NULL, sizeof(T));//�߰�
		
		i++;
	}
	
	return V;
}

 