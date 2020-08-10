struct fb {
	unsigned long long base;
	unsigned long long size;
	unsigned int hr;
	unsigned int vr;
};

//1ピクセルのデータ
typedef struct {
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

unsigned char kernel_main_stack[1024 * 1024];

extern void KernelMainNewStack(struct fb *fb) {
	for(int i = 0; i < 1000; i++) {
		char *p = (char *)fb->base + i;
		*p = 0xFF;
	}

	return;
}