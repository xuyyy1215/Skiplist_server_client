#include <WINSOCK2.H>   
#include <stdio.h>  
#include<string>
#include <iostream>
#include<windows.h>
#include "Command.h"
using namespace std;

#pragma warning( disable : 4996 )
//���������ʹ�õĳ���      
#define SERVER_ADDRESS "127.0.0.1" //��������IP��ַ      
#define PORT           5150         //�������Ķ˿ں�      
#define MSGSIZE        1024         //�շ��������Ĵ�С      
#pragma comment(lib, "ws2_32.lib")      

int main()
{
    WSADATA wsaData;
    //���������׽���      
    SOCKET sClient;
    //����Զ�̷������ĵ�ַ��Ϣ      
    SOCKADDR_IN server;
    //�շ�������      
    char szMessage[MSGSIZE];
    //�ɹ������ֽڵĸ���      
    int ret;

    // Initialize Windows socket library      
    WSAStartup(0x0202, &wsaData);

    // �����ͻ����׽���      
    sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INETָ��ʹ��TCP/IPЭ���壻      
                                                         //SOCK_STREAM, IPPROTO_TCP����ָ��ʹ��TCPЭ��      
    // ָ��Զ�̷������ĵ�ַ��Ϣ(�˿ںš�IP��ַ��)      
    memset(&server, 0, sizeof(SOCKADDR_IN)); //�Ƚ������ַ��server��Ϊȫ0      
    server.sin_family = PF_INET; //������ַ��ʽ��TCP/IP��ַ��ʽ      
    server.sin_port = htons(PORT); //ָ�����ӷ������Ķ˿ںţ�htons()���� converts values between the host and network byte order      
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS); //ָ�����ӷ�������IP��ַ      
                                                        //�ṹSOCKADDR_IN��sin_addr�ֶ����ڱ���IP��ַ��sin_addr�ֶ�Ҳ��һ���ṹ�壬sin_addr.s_addr�������ձ���IP��ַ      
                                                        //inet_addr()���ڽ� �����"127.0.0.1"�ַ���ת��ΪIP��ַ��ʽ      
    //�����ղ�ָ���ķ�������      
    connect(sClient, (struct sockaddr*)&server, sizeof(SOCKADDR_IN)); //���Ӻ������sClient��ʹ���������      
    cout << "Redis_client by Stack start-up...\n";                                                      //server������Զ�̷������ĵ�ַ��Ϣ      
    while (TRUE) {

        cout<<"Stack_Redis>";
        //�Ӽ�������  
        string message;
        getline(cin, message); //The gets() functionreads characters from stdin and loads them into szMessage      
        strcpy(szMessage, message.c_str());
        Command C(message);
        if (!C.is_valid_command()) {
            continue;
        }

        // ��������      
        send(sClient, szMessage, strlen(szMessage), 0); //sClientָ�����ĸ����ӷ��ͣ� szMessageָ�����������ݵı����ַ ��strlen(szMessage)ָ�����ݳ���      
        char szBuffer[MAXBYTE] = { 0 };
        if (recv(sClient, szBuffer, MAXBYTE, 0) <= 0) {
            break;
        }
        //����յ�������
        cout << "Server reply: " << szBuffer << endl;

    }

    // �ͷ����Ӻͽ��н�������      
    closesocket(sClient);
    WSACleanup();
    return 0;
}