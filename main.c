
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(void)
{
	int fd, ret;

	fd = open("/dev/ttyS2", O_RDWR|O_NOCTTY|O_NONBLOCK);
	if (fd < 0)
	{
		perror("open ttyS2");
		return 1;
	}

	fcntl(fd, F_SETFL, 0); //閲嶈涓哄牭濉炵姸鎬? 鍘绘帀O_NONBLOCK
	//////////////////
	struct termios opts;
	tcgetattr(fd, &opts); //鎶婂師璁剧疆鑾峰彇鍑烘潵锛屽瓨鏀惧湪opts

	//璁剧疆娉㈢壒鐜?
	cfsetispeed(&opts, B115200);
	cfsetospeed(&opts, B115200);

	opts.c_cflag |= CLOCAL|CREAD; //蹇界暐modem鎺у埗绾? 鍚姩鎺ユ敹鍣?

	// 8N1
	opts.c_cflag &= ~PARENB; 
	opts.c_cflag &= ~CSTOPB;
	opts.c_cflag |= CS8;

	opts.c_cflag &= ~CRTSCTS; //鍏抽棴纭欢娴佹帶

	opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //raw input
	opts.c_oflag &= ~OPOST; // raw output          

	tcsetattr(fd, TCSANOW, &opts);  
	//////////////////
	char data[1024];

	while(1)
	{
		ret = read(fd, data, sizeof(data));
		if(ret > 0)
		{
			data[ret] = 0;
			printf("got : %s\n", data);
		}
		else
		{
			printf("not got data ret: %d\n", ret);
		}
		memset(data,0,sizeof(data));
		//sleep(1);

	}
	close(fd);
	return 0;
}
