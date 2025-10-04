#ifndef __Q3__
#define __Q3__

#define BUF_SIZE 32
#define STRING_SIZE 10

typedef struct circular_buf {
  int head;
  int tail;
  char buffer[BUF_SIZE];
} circular_buf;

#define PRODUCER_FILE_TO_RUN "producer.dlx.obj"
#define CONSUMER_FILE_TO_RUN "consumer.dlx.obj"

#endif
