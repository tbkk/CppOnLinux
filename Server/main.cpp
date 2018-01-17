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

		FD_ZERO(&readfd);    //每次循环都要清空集合，否则不能检测描述符变化
		FD_SET(keyboard, &readfd);    //添加描述符

		ret = select(keyboard + 1, &readfd, NULL, NULL, &timeout);


		int rett = FD_ISSET(keyboard, &readfd);
		printf("the rett is %d\n", rett);

		if (rett)  //测试keyboard是否可读，即keyboard是否有数据输入
		{
			i = read(keyboard, &c, 1);
			if ('\n' == c)
			{
				continue;
			}
			printf("the input is %c\n", c);

			if ('q' == c)   //输入q结束
			{
				break;
			}
		}

		int rett2 = FD_ISSET(keyboard, &readfd);
		printf("the22 rett is %d\n", rett2);
	}
    return 0;
}