#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

void cat_m(char** res) {
	char buf[512]; // buffer 크기는 512 bytes 입니다. 이 부분은 수정하지 마세요. 
	int fd, n;

	// 코드 작성 
	fd = open(res[1], O_RDONLY);
	if (fd == -1)
		return;
	while (1) {
		n = read(fd, buf, 512);
		if (n == 0)
			return;
		write(1, buf, n);
	}
}

void cd_m(char** res) {

	chdir(res[1]);

}

void cp_m(char** res) {
	char buf[512]; // buffer 크기는 512 bytes 입니다. 이 부분은 수정하지 마세요. 
	int fd1, fd2, n;
	struct stat s;

	// 코드 작성
	fd1 = open(res[1], O_RDONLY);
	if (fd1 == -1)
		return;
	stat(res[1], &s);

	fd2 = open(res[2], O_WRONLY | O_CREAT | O_TRUNC, s.st_mode);

	while (1) {
		n = read(fd1, buf, 512);
		if (n == 0)
			return;
		write(fd2, buf, n);
	}
}

void ls_m(char** res) {
	DIR* dp;
	struct dirent* d;
	struct stat s;

	dp = opendir(".");
	d = readdir(dp);


	while (d != NULL) {

		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) {
			d = readdir(dp);
			continue;
		}
		char* str;
		stat(d->d_name, &s);
		if (S_ISREG(s.st_mode))
			printf("F ");
		else
			printf("D ");
		printf("%04o  %d  ", s.st_mode & 0777, s.st_nlink);
		printf("%d  %d  ", s.st_uid, s.st_gid);
		printf("%ld ", s.st_size);

		str = ctime(&s.st_mtime);
		str[strlen(str) - 1] = '\0';
		printf("%s %s", str, d->d_name);
		d = readdir(dp);
		printf("\n");
	}
}

void mkdir_m(char** res) {

	// 코드 작성 
	mkdir(res[1], 0700);
}


void vi_m(char** res) {
	char buf[512]; // buffer 크기는 512 bytes 입니다. 이 부분은 수정하지 마세요. 
	int fd, n;

	// 코드 작성 
	fd = open(res[1], O_RDWR);
	if (fd == -1) {
		fd = open(res[1], O_WRONLY | O_CREAT, 0600);
		while (1) {
			n = read(0, buf, 512);
			if (strncmp(buf, "quit", 4) == 0)
				break;
			write(fd, buf, n);
		}
	}
	else {
		while (1)
		{
			n = read(fd, buf, 512);
			if (n == 0)
				break;
			write(1, buf, n);
		}
		while (1) {
			n = read(1, buf, 512);
			if (strncmp(buf, "quit", 4) == 0)
				break;
			write(fd, buf, n);
		}
	}
}

int main(void) {
	char in[100], * res[20] = { 0 };
	char curr[200];
	char* inst[6] = { "cat", "cd", "cp", "ls", "mkdir", "vi" };
	void (*f[6])(char**) = { cat_m, cd_m, cp_m, ls_m, mkdir_m, vi_m };
	int i;

	while (1) {

		getcwd(curr, 200);
		printf("%s>  ", curr);

		gets(in);
		if (in[0] == '\0')
			continue;

		i = 0;
		res[i] = strtok(in, " ");
		while (res[i]) {
			i++;
			res[i] = strtok(NULL, " ");
		}

		if (strcmp(res[0], "exit") == 0)
			exit(0);

		for (i = 0; i < 6; i++) {
			if (!strcmp(res[0], inst[i])) {
				f[i](res);
				break;
			}
		}
	}

	return 0;
}
