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
    int size = 0;//EDB_Tset의 데이터가 있는 갯수

    Index Index_Num[10]; //EDB_Tset의 데이터가 있는 인덱스
    
    memset(Index_Num, NULL, sizeof(Index_Num));

    
    
    int port = 0;
    
    printf("포트번호를 입력하세요: ");
    scanf("%d", &port);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리 초기화
        ErrorHandling("WSAStartup() error!");

    hServSock = socket(PF_INET, SOCK_STREAM, 0); //소켓생성
    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//모든 ip주소를 허용한다
    servAddr.sin_port = htons(port);//포트번호 입력

    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //소켓에 IP주소와 PORT 번호 할당
        ErrorHandling("bind() error");

    if (listen(hServSock, 5) == SOCKET_ERROR) //listen 함수호출을 통해서 생성한 소켓을 서버 소켓으로 완성
        ErrorHandling("listen() error");

    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); //클라이언트 연결요청 수락하기 위해 accept함수 호출

    if (hClntSock == INVALID_SOCKET)
        ErrorHandling("accept() error");


    printf("\nEDB_Tset을 받으시겠습니까?(y/n)");
    scanf("%s", test);
    
    if (strncmp(test, "y", 1) == 0) {

        strLen = recv(hClntSock, &size, sizeof(size), 0);//클라이언트로부터 Stag 받기
        if (strLen == -1)
            ErrorHandling("read() error");
        strLen = recv(hClntSock, Index_Num, sizeof(Index_Num), 0);//클라이언트로부터 Stag 받기
        if (strLen == -1)
            ErrorHandling("read() error");
        
        for (i = 0; i < size; i++) {
            strLen = recv(hClntSock, EDB_Tset[Index_Num[i].i][Index_Num[i].j].label, 27, 0);//클라이언트로부터 Stag 받기
        }

    }

    //EDB_Tset 값 출력
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
        printf("\n\nStag값을 받으시겠습니까?(y/n)");
        scanf("%s", test);
        
        if (strcmp(test, "y") != 0) break;//y아닐시 종료
        else if (strcmp(test, "y") == 0) {
            strLen = recv(hClntSock, Stag, sizeof(Stag), 0);//클라이언트로부터 Stag 받기
            if (strLen == -1)
                ErrorHandling("read() error");
        }

        printf("\nStag= ");
        for (i = 0; i < 16; i++) {
            printf("%x ", Stag[i]);
        }

        vptr = TsetRetrieve(EDB_Tset, Stag);//Stag로 암호화된 id를 받아옴
        memcpy(V, vptr, sizeof(Word) * 4);//V배열에 암호화된 id 저장

        
        printf("\n\n암호화된 파일id\n");
        for (i = 0; i < 4; i++) {
            if (strcmp(V[i].a, "") != 0) {
                printf("e%d= ", i);
                for (j = 0; j < 16; j++) {
                    printf("%x ", *((V[i].a) + j));
                }
                printf("\n");
            }

        }

        printf("\n암호화된 파일id를 클라이언트로 보내시겠습니까?(y/n)");
        scanf("%s", test);
        if (strncmp(test, "y", 1) == 0) {
            send(hClntSock, V, sizeof(V), 0); //클라이언트로 V배열 전송
        }
        memset(V, NULL, sizeof(V));
    }
    
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup(); //프로그램 종료 전에 초기화한 소켓 라이브러리 해제
    
    return 0;

}



void ErrorHandling(char* message) //에러

{

    fputs(message, stderr);

    fputc('\n', stderr);

    exit(1);

}



