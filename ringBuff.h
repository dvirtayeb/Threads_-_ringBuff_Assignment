
typedef struct buff {
	int *nums;
	int head;
	int tail;
	int maxlen;
} ringBuff;

int circ_bbuf_push(ringBuff *buff, int data);
int circ_bbuf_pop(ringBuff *buff, int *data);
