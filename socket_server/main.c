#include "Head.h"


int main()

{

    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    Pair EDB_Tset[128][128];
    unsigned char Stag[16] = { '\0' }; 
    unsigned char test[10] = { '\0' };
    Word V[4];
    Word* vptr;

    memset(EDB_Tset, NULL, sizeof(EDB_Tset));
    memset(V, NULL, sizeof(V));
    
    
    int i, j, strLen, szClntAddr;
    int g = 0;
    int h = 0;
    int f = 0;
    int size = 0;//EDB_Tset�� �����Ͱ� �ִ� ����

    Index Index_Num[10]; //EDB_Tset�� �����Ͱ� �ִ� �ε���
    
    memset(Index_Num, NULL, sizeof(Index_Num));

    
    
    int port = 0;
    
    printf("��Ʈ��ȣ�� �Է��ϼ���: ");
    scanf("%d", &port);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //���� ���̺귯�� �ʱ�ȭ
        ErrorHandling("WSAStartup() error!");

    hServSock = socket(PF_INET, SOCK_STREAM, 0); //���ϻ���
    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//��� ip�ּҸ� ����Ѵ�
    servAddr.sin_port = htons(port);//��Ʈ��ȣ �Է�

    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //���Ͽ� IP�ּҿ� PORT ��ȣ �Ҵ�
        ErrorHandling("bind() error");

    if (listen(hServSock, 5) == SOCKET_ERROR) //listen �Լ�ȣ���� ���ؼ� ������ ������ ���� �������� �ϼ�
        ErrorHandling("listen() error");

    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); //Ŭ���̾�Ʈ �����û �����ϱ� ���� accept�Լ� ȣ��

    if (hClntSock == INVALID_SOCKET)
        ErrorHandling("accept() error");


    printf("\nEDB_Tset�� �����ðڽ��ϱ�?(y/n)");
    scanf("%s", test);
    
    if (strncmp(test, "y", 1) == 0) {

        strLen = recv(hClntSock, &size, sizeof(size), 0);//Ŭ���̾�Ʈ�κ��� Stag �ޱ�
        if (strLen == -1)
            ErrorHandling("read() error");
        strLen = recv(hClntSock, Index_Num, sizeof(Index_Num), 0);//Ŭ���̾�Ʈ�κ��� Stag �ޱ�
        if (strLen == -1)
            ErrorHandling("read() error");
        
        for (i = 0; i < size; i++) {
            strLen = recv(hClntSock, EDB_Tset[Index_Num[i].i][Index_Num[i].j].label, 27, 0);//Ŭ���̾�Ʈ�κ��� Stag �ޱ�
        }

    }

    //EDB_Tset �� ���
    for (h = 0; h < 128; h++) {
        for (g = 0; g < 128; g++) {
            if (strcmp(EDB_Tset[h][g].label, "") != 0) {
                printf("\n\nEDB_Tset[%d][%d].label= ", h, g);
                for (f = 0; f < 27; f++) {
                    if (f < 10) printf("%x ", *((EDB_Tset[h][g].label) + f));
                    else if (f == 10) printf("\nvalue= %x ", *((EDB_Tset[h][g].label) + f));
                    else printf("%x ", *((EDB_Tset[h][g].label) + f));
                }
            }
        }
    }

    while (1) {
        
        memset(V, NULL, sizeof(V)); 
        memset(Stag, NULL, sizeof(Stag));
        printf("\n\nStag���� �����ðڽ��ϱ�?(y/n)");
        scanf("%s", test);
        
        if (strcmp(test, "y") != 0) break;//y�ƴҽ� ����
        else if (strcmp(test, "y") == 0) {
            strLen = recv(hClntSock, Stag, sizeof(Stag), 0);//Ŭ���̾�Ʈ�κ��� Stag �ޱ�
            if (strLen == -1)
                ErrorHandling("read() error");
        }

        printf("\nStag= ");
        for (i = 0; i < 16; i++) {
            printf("%x ", Stag[i]);
        }

        vptr = TsetRetrieve(EDB_Tset, Stag);//Stag�� ��ȣȭ�� id�� �޾ƿ�
        memcpy(V, vptr, sizeof(Word) * 4);//V�迭�� ��ȣȭ�� id ����

        
        printf("\n\n��ȣȭ�� ����id\n");
        for (i = 0; i < 4; i++) {
            if (strcmp(V[i].a, "") != 0) {
                printf("e%d= ", i);
                for (j = 0; j < 16; j++) {
                    printf("%x ", *((V[i].a) + j));
                }
                printf("\n");
            }

        }

        printf("\n��ȣȭ�� ����id�� Ŭ���̾�Ʈ�� �����ðڽ��ϱ�?(y/n)");
        scanf("%s", test);
        if (strncmp(test, "y", 1) == 0) {
            send(hClntSock, V, sizeof(V), 0); //Ŭ���̾�Ʈ�� V�迭 ����
        }
        memset(V, NULL, sizeof(V));
    }
    
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup(); //���α׷� ���� ���� �ʱ�ȭ�� ���� ���̺귯�� ����
    
    return 0;

}



void ErrorHandling(char* message) //����

{

    fputs(message, stderr);

    fputc('\n', stderr);

    exit(1);

}



