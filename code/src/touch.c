#include "touch.h"
extern time_t last_operation_time; 
extern pthread_mutex_t mutex; 

void touch_update_last_operation_time() 
{
    pthread_mutex_lock(&mutex);
    last_operation_time = time(NULL);
    pthread_mutex_unlock(&mutex);
}
int get_xy(int *x, int *y)
{
    //1.�򿪴������豸�ļ�  /dev/input/event0
    int fd = open("/dev/input/event0", O_RDWR);
    if(-1 == fd)
    {
        printf("open touch err!\n");
        return 0;
    }
    
    //2.�����ض��ĸ�ʽ��ͣ���豸�ļ��ж�ȡ����
    struct input_event ts;
    while(1)
    {
        //û�е��������ʱ���豸�ļ���û�����ݣ�read�����������ȴ�
        read(fd, &ts, sizeof(ts));
        
        //3.������ȡ������
        if(ts.type == EV_ABS)        //������
        {
            if(ts.code == ABS_X)    //������
            {
                //printf("x:%d\n", ts.value);
                *x = ts.value;
            }
            
            if(ts.code == ABS_Y)    //������
            {
                //printf("y:%d\n", ts.value);
                *y = ts.value;
            }
            if(ts.code == ABS_PRESSURE)        //    ѹ��ֵ�汾һ
            {
                //printf("(1)PRESSURE:%d\n", ts.value);
                if(ts.value == 1)    //����ʱ����
                {
                    update_last_operation_time();
                    break;
                }
            }
        }
        if(ts.type == EV_KEY && ts.code == BTN_TOUCH)    //    ѹ��ֵ�汾��
        {
            //printf("(2)PRESSURE:%d\n", ts.value);
            if(ts.value == 1)    //����ʱ����
            {
                update_last_operation_time();
                break;
            }
        }
    }
    
    //4.�رմ������豸�ļ�
    close(fd);
    
    return 0;
}

