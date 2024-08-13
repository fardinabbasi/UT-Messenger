#pragma once
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include <stdio.h>
#include <Windows.h>//For collor
# define true 1
struct user{
	char *phone;
	char *username;
	char *pass;
	struct user*next;
	time_t signup;
};
struct message
{
	int flag;
	char *pm;
	char *from;
	char *to;
	time_t sent;
	struct message*next;
};
char*givecommend();
int signup(struct user *head_u, struct message *head_m, char **logged_in);
int login(struct user *head_u, struct message *head_m,char **logged_in);
void saveinf(struct user *head_u, struct message *head_m);
void readinf(struct user *head_u, struct message *head_m);
int getusername(struct user *head_u, char *temp_username);
int getpass(struct user *head_u, char *temp_pass);
int getphone(struct user *head_u, char *temp_phone);
void adduser(struct user *head_u, char *temp_username, char *temp_pass, char *temp_phone);
int menue(struct user *head_u, struct message *head_m, char **logged_in);
void sendpm(struct user *head_u, struct message *head_m, char **logged_in);
void info(struct user *head_u);
void edit_username(struct user *head_u, char **logged_in);
void edit_pass(struct user *head_u, char**logged_in);
void about(struct user *head_u);
void addpm(struct message *head_m, char **logged_in, char *contact);
int existcheck(struct user *head_u, char * username);
void show_sent(struct user *head_u, struct message *head_m, char **logged_in);
void show_received(struct user *head_u, struct message *head_m, char **logged_in);
void show_unread_ms(struct message *head_m, char **logged_in);
void addmsfromfile(struct message *head_m, char*from, char*to, char*pm, int flag, int time_m);
void adduserfromfile(struct user *head_u, char *username, char *pass, char *phone, int time);
void destroy(struct user **head_u, struct message **head_m);