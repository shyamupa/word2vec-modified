//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

const long long max_size = 5000;         // max length of strings
// const long long N = 40;            // number of closest words that will be shown
const long long N = 10;            // number of closest words that will be shown
const long long max_w = 500;              // max length of vocabulary entries

float* getSimilar(char st1[max_size], float *M, char *vocab, long long words,
		long long size, char *bestw[N]) {
	if (strcmp(st1, "EXIT") == 0)
		return NULL;
	char* ans_buf = (char*) malloc(sizeof(char) * (max_size + 1));
	int ans_length = 0;

	char file_name[max_size], st[100][max_size];
	long long a, b, c, d, cn, bi[100];
	float dist, len, vec[max_size];
	float* bestd = (float*) malloc(sizeof(float) * N);
	for (a = 0; a < N; a++)
		bestd[a] = 0;
	for (a = 0; a < N; a++)
		bestw[a][0] = 0;
	// printf("Enter word or sentence (EXIT to break): ");
	a = 0;
	cn = 0;
	b = 0;
	c = 0;
	while (1) {
		st[cn][b] = st1[c];
		b++;
		c++;
		st[cn][b] = 0;
		if (st1[c] == 0)
			break;
		if (st1[c] == ' ') {
			cn++;
			b = 0;
			c++;
		}
	}
	cn++;
	for (a = 0; a < cn; a++) {
		for (b = 0; b < words; b++)
			if (!strcmp(&vocab[b * max_w], st[a]))
				break;
		if (b == words)
			b = -1;
		bi[a] = b;
		// printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
		if (b == -1) {
		  printf("Out of dictionary word! : %s\n",st1);
		  return NULL;
		}
	}
	// printf(
			// "\n                                              Word       Cosine distance\n------------------------------------------------------------------------\n");
	for (a = 0; a < size; a++)
		vec[a] = 0;
	for (b = 0; b < cn; b++) {
		if (bi[b] == -1)
			continue;
		for (a = 0; a < size; a++)
			vec[a] += M[a + bi[b] * size];
	}
	len = 0;
	for (a = 0; a < size; a++)
		len += vec[a] * vec[a];
	len = sqrt(len);
	for (a = 0; a < size; a++)
		vec[a] /= len;
	for (a = 0; a < N; a++)
		bestd[a] = -1;
	for (a = 0; a < N; a++)
		bestw[a][0] = 0;
	for (c = 0; c < words; c++) {
		a = 0;
		for (b = 0; b < cn; b++)
			if (bi[b] == c)
				a = 1;
		if (a == 1)
			continue;
		dist = 0;
		for (a = 0; a < size; a++)
			dist += vec[a] * M[a + c * size];
		for (a = 0; a < N; a++) {
			if (dist > bestd[a]) {
				for (d = N - 1; d > a; d--) {
					bestd[d] = bestd[d - 1];
					strcpy(bestw[d], bestw[d - 1]);
				}
				bestd[a] = dist;
				strcpy(bestw[a], &vocab[c * max_w]);
				break;
			}
		}
	}
	for (a = 0; a < N; a++) {
		ans_length += snprintf(ans_buf + ans_length, max_size - ans_length,
				"%50s\t\t%f\n", bestw[a], bestd[a]);
//		printf("%50s\t\t%f\n", bestw[a], bestd[a]);
	}
	// printf("WRITTEN!\n");
	// return ans_buf;
	// printf(ans_buf);
	return bestd;
	
}

// void server(float *M, char *vocab, long long words, long long size,
// 		char *bestw[N]) {
// 	char* ans;

// 	int sockfd, newsockfd, portno;
// 	char st1[max_size];

// 	socklen_t clilen;
// 	struct sockaddr_in serv_addr, cli_addr;
// 	int n;

// 	/* First call to socket() function */
// 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (sockfd < 0) {
// 		perror("ERROR opening socket");
// 		exit(1);
// 	}
// 	/* Initialize socket structure */
// 	bzero((char *) &serv_addr, sizeof(serv_addr));
// 	portno = 5001;
// 	serv_addr.sin_family = AF_INET;
// 	serv_addr.sin_addr.s_addr = INADDR_ANY;
// 	serv_addr.sin_port = htons(portno);

// 	/* Now bind the host address using bind() call.*/
// 	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
// 		perror("ERROR on binding");
// 		exit(1);
// 	}

// 	/* Now start listening for the clients, here process will
// 	 * go in sleep mode and will wait for the incoming connection
// 	 */
// 	listen(sockfd, 5);
// 	clilen = sizeof(cli_addr);
// 	printf("Starting Server ...\n");
// 	while (1) {
// 		/* Accept actual connection from the client */
// 		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
// 		if (newsockfd < 0) {
// 			perror("ERROR on accept");
// 			exit(1);
// 		}
// 		while (1) {

// 			/* If connection is established then start communicating */
// 			bzero(st1, max_size);
// 			n = read(newsockfd, st1, max_size - 1);

// 			if (n < 0) {
// 				perror("ERROR reading from socket");
// 				exit(1);
// 			}
// 			printf("SERVER received: %s\n", st1);

// 			// do your evil bidding here
// 			// char* ans = getSimilar(st1, M, vocab, words, size, bestw);
// 			if (strcmp(ans, "-1") == 0) {

// 				n = write(newsockfd, "BYE", strlen("BYE"));
// 				close(newsockfd);
// 				break;
// 			}
// 			// evil bidding should be done now

// 			/* Write a response to the client */
// 			n = write(newsockfd, ans, strlen(ans));
// 			if (n < 0) {
// 				perror("ERROR writing to socket");
// 				exit(1);
// 			}
// 		}
// 	}
// 	return;

// }

int main(int argc, char **argv) {
  FILE *f;
  char st1[max_size];
  char *bestw[N];
  char file_name[max_size], st[100][max_size], title[max_size];
  float dist, len, bestd[N], vec[max_size];
  long long words, size, a, b, c, d, cn, bi[100];
  char ch;
  float *M;
  char *vocab;
  if (argc < 3) {
    printf(
	   "Usage: ./distance <FILE> <INPUT_FILE>\nwhere FILE contains word projections in the BINARY FORMAT\nwhere INPUT_FILE contains list of queries\nwhere OUTPUT_FILE contains list of answers\n");
    return 0;
  }
  strcpy(file_name, argv[1]);
  f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Input file not found\n");
    return -1;
  }
  time_t t0 = time(NULL);
  fscanf(f, "%lld", &words);	// # of words in vocab
  printf("VOCAB: %lld\n",words);
  fscanf(f, "%lld", &size);	// size of vector
  vocab = (char *) malloc((long long) words * max_w * sizeof(char));
  for (a = 0; a < N; a++)
    bestw[a] = (char *) malloc(max_size * sizeof(char));	// init. buf for ans
  M = (float *) malloc((long long) words * (long long) size * sizeof(float)); // all vectors
  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n",
	   (long long) words * size * sizeof(float) / 1048576, words,
	   size);
    return -1;
  }
  for (b = 0; b < words; b++) {
    fscanf(f, "%s%c", &vocab[b * max_w], &ch);	// read vocab word at intervals of max_w
    for (a = 0; a < size; a++)
      fread(&M[a + b * size], sizeof(float), 1, f);
    len = 0;
    for (a = 0; a < size; a++)
      len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    for (a = 0; a < size; a++)
      M[a + b * size] /= len;
  }
  fclose(f);
  // server(M, vocab, words, size, bestw);
  time_t t1 = time(NULL);
  printf("Time taken to load %ld sec\n",(long)(t1-t0));


  freopen(argv[2],"r",stdin);
  FILE* out = fopen(argv[3],"w");
  using namespace std;
  long cnt=0;
  while(true)
    { 
      cin >> title;
      if( cin.eof() ) break;
      // cout << title << endl;
      // t0 = t1;
      float* bestd=getSimilar(title, M, vocab,words,size, bestw);
      // t1 = time(NULL);
      // printf("Time taken to answer %ld sec\n",(long)(t1-t0));
      if(bestd==NULL)
	{
	  fprintf(out,"OUT_OF_DICT %s\n",title);
	  continue;
	}
      // printf("%s\t",title);
      fprintf(out,"%s\t",title);
      for(int i=0;i<N;i++)
	{
	  fprintf(out,"%s\t%f\t",bestw[i],bestd[i]);
	  // printf("%s\t%f\t",bestw[i],bestd[i]);
	}
      fprintf(out,"\n");
      // printf("\n");
      if(cnt++ % 100 == 0)
	printf("DONE %ld\n",cnt);
    }
  fclose(out);
  return 0;
}

