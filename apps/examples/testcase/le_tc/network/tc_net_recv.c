/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

// @file tc_net_recv.c
// @brief Test Case Example for recv() API
#include <tinyara/config.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <protocols/netlib.h>
#include "tc_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORTNUM 1109
#define MAXRCVLEN 20
int mutex = 0;
/**
   * @fn                   :recv_wait
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void recv_wait(void)
{
	while (mutex <= 0) {

		printf("");
	}
	mutex--;
}

/**
   * @fn                   :recv_signal
   * @brief                :function to signal semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void recv_signal(void)
{
	mutex++;
}

/**
   * @testcase		   :tc_net_recv_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :recv()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recv_p(int fd)
{
	char buffer[MAXRCVLEN];
	int ret = recv(fd, buffer, MAXRCVLEN, 0);
	buffer[ret] = '\0';

	TC_ASSERT_NEQ("recv", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_recv_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :recv()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recv_n(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recv(-1, buffer, MAXRCVLEN, 0);
	buffer[ret] = '\0';

	TC_ASSERT_EQ("recv", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_recv_shutdown_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :recv()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recv_shutdown_n(int fd)
{
	char buffer[MAXRCVLEN];
	shutdown(fd, SHUT_RD);
	int ret = recv(fd, buffer, MAXRCVLEN, 0);
	buffer[ret] = '\0';

	TC_ASSERT_EQ("recv", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_recv_close_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :recv()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recv_close_n(int fd)
{
	char buffer[MAXRCVLEN];
	close(fd);
	int ret = recv(fd, buffer, MAXRCVLEN, 0);
	buffer[ret] = '\0';

	TC_ASSERT_EQ("recv", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @fn                   :recv_server
   * @brief                :
   * @scenario             :
   * API's covered         :socket,bind,listen,accept,send,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *recv_server(void *args)
{

	char *msg = "Hello World !\n";
	struct sockaddr_in sa;
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));

	listen(SocketFD, 2);
	recv_signal();
	int ConnectFD = accept(SocketFD, NULL, NULL);
	int i;
	for (i = 0; i < 6; i++)
		send(ConnectFD, msg, strlen(msg), 0);

	close(ConnectFD);

	close(SocketFD);
	return 0;

}

/**
   * @fn                   :recv_client
   * @brief                :
   * @scenario             :
   * API's covered         :socket,connect,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *recv_client(void *args)
{

	int mysocket;
	struct sockaddr_in dest;

	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	recv_wait();

	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	tc_net_recv_p(mysocket);
	tc_net_recv_n(mysocket);
	tc_net_recv_shutdown_n(mysocket);
	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	tc_net_recv_close_n(mysocket);
	close(mysocket);
	return 0;

}

/****************************************************************************
 * Name: recv()
 ****************************************************************************/
int net_recv_main(void)
{

	pthread_t Server, Client;

	pthread_create(&Server, NULL, recv_server, NULL);
	pthread_create(&Client, NULL, recv_client, NULL);

	pthread_join(Server, NULL);

	pthread_join(Client, NULL);

	return 0;
}
