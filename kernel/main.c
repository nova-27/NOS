//フレームバッファに関する情報
struct fb {
	unsigned long long base;
	unsigned long long size;
	unsigned int hr;
	unsigned int vr;
};

//スタック用変数
unsigned char kernel_stack[1024 * 1024] __attribute__ ((aligned(16)));

/* エントリポイント */
extern void KernelMain(struct fb *fb) {
	for(int i = 0; i < 1000; i++) {
		char *p = (char *)fb->base + i;
		*p = 0xFF;
	}

	return;
}