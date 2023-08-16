#include "show_bmp.h"

int show_bmp(char *pathname)
{
	//��ͼƬ�ļ�
	int bmpfd = open(pathname, O_RDWR);
	if(-1 == bmpfd)
	{
		printf("open bmp err!\n");
		return -1;
	}
	
	//����ʾ�����豸�ļ�  open
	int lcdfd = open("/dev/fb0", O_RDWR);
	if(-1 == lcdfd)
	{
		printf("open lcd err!\n");
		return -1;
	}
	
	//4.����ͼƬ�ļ�ǰ54�ֽڵ��ļ���Ϣͷ
	lseek(bmpfd, 54, SEEK_SET);
	
	//5.��ȡ800*480*3 ��1152000���ֽڵ���ɫ����
	char BGR[800*480*3];
	read(bmpfd, BGR, 800*480*3);
	
	//6.��Ҫ��BGR����ת��Ϊ��ĻҪ���ARGB��ʽ
	int ARGB[800*480];
	int i;
	for(i=0; i<800*480; i++)
		ARGB[i] = BGR[3*i] | BGR[3*i+1]<<8 | BGR[3*i+2]<<16;
	//				B         G            R
	
	//7.����BMPͼƬ���������ϴ棬������Ҫ���а��з�ת
	int data[800*480];
	int x, y;
	for(y=0; y<480; y++)
		for(x=0; x<800; x++)
			data[(479-y)*800+x] = ARGB[y*800+x];
		
	//8.������õ�����д����ʾ���豸�ļ�
	//write(lcdfd, data, 800*480*4);
	
	//�ڴ�ӳ�䣬�������д��Ч��
	int *p = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, 
				lcdfd, 0);
				
	for(y=0; y<480; y++)
		for(x=0; x<800; x++)
			*(p+y*800+x) = data[y*800+x];
		
	munmap(p, 800*480*4);
	
	//9.�ر���ʾ���豸�ļ���ͼƬ�ļ�
	close(lcdfd);
	close(bmpfd);
	
}

//��ʾ��ȫ��ͼƬ
int show_bmp_any(char *pathname,int w, int h, int rx, int ry)
{
	if((w+rx>800) || (h+ry>480) || (w*3%4!=0))
	{
		printf("bmp err!\n");
		return -1;
	}
	
	//��ͼƬ�ļ�
	int bmpfd = open(pathname, O_RDWR);
	if(-1 == bmpfd)
	{
		printf("open bmp err!\n");
		return -1;
	}
	
	//����ʾ�����豸�ļ�  open
	int lcdfd = open("/dev/fb0", O_RDWR);
	if(-1 == lcdfd)
	{
		printf("open lcd err!\n");
		return -1;
	}
	
	//4.����ͼƬ�ļ�ǰ54�ֽڵ��ļ���Ϣͷ
	lseek(bmpfd, 54, SEEK_SET);
	
	//5.��ȡw*h*3 �ֽڵ���ɫ����
	char BGR[w*h*3];
	read(bmpfd, BGR, w*h*3);
	
	//6.��Ҫ��BGR����ת��Ϊ��ĻҪ���ARGB��ʽ
	int ARGB[w*h];
	int i;
	for(i=0; i<w*h; i++)
		ARGB[i] = BGR[3*i] | BGR[3*i+1]<<8 | BGR[3*i+2]<<16;
	//				B             G              R
	
	//7.����BMPͼƬ���������ϴ棬������Ҫ���а��з�ת
	int data[w*h];
	int x, y;
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
			data[(h-1-y)*w+x] = ARGB[y*w+x];
		
	//8.������õ�����д����ʾ���豸�ļ�
	//write(lcdfd, data, 800*480*4);
	
	//�ڴ�ӳ�䣬�������д��Ч��
	int *p = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, 
				lcdfd, 0);
				
	for(y=0; y<h; y++)
		for(x=0; x<w; x++)
			*(p+(ry+y)*800+x+rx) = data[y*w+x];
		//	*(p + ry*800 + rx)	= data[0];
			
	munmap(p, 800*480*4);
	
	//9.�ر���ʾ���豸�ļ���ͼƬ�ļ�
	close(lcdfd);
	close(bmpfd);
	
}
