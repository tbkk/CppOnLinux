#include <cstdio>
#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

int main()
{
    printf("hello from Server!\n");

	int keyboard;
	int ret, i;
	char c;
	fd_set readfd;
	struct timeval timeout;

	keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	assert(keyboard > 0);

	while (1)
	{
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;

		FD_ZERO(&readfd);    //ÿ��ѭ����Ҫ��ռ��ϣ������ܼ���������仯
		FD_SET(keyboard, &readfd);    //���������

		ret = select(keyboard + 1, &readfd, NULL, NULL, &timeout);


		int rett = FD_ISSET(keyboard, &readfd);
		printf("the rett is %d\n", rett);

		if (rett)  //����keyboard�Ƿ�ɶ�����keyboard�Ƿ�����������
		{
			i = read(keyboard, &c, 1);
			if ('\n' == c)
			{
				continue;
			}
			printf("the input is %c\n", c);

			if ('q' == c)   //����q����
			{
				break;
			}
		}

		int rett2 = FD_ISSET(keyboard, &readfd);
		printf("the22 rett is %d\n", rett2);
	}
    return 0;
}