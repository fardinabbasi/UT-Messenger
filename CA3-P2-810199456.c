#include "Header.h"
void main() {
	char *logged_in = NULL;//which user is in system
	struct user *head_u = (struct user *)malloc(sizeof(struct user));
	struct message *head_m = (struct message *)malloc(sizeof(struct message));
	if (head_m == NULL || head_u == NULL) {
		printf("maloc failed");
		return;
	}
	head_u->next = NULL;
	head_m->next = NULL;//heads are dummy
	readinf(head_u, head_m);
	char *command;
	while (true) {
		printf("Welcom to UT MESSENGER \t Enter command\n");
		printf(">> ");
		command = givecommend();
		if (strcmp(command, "signup") == 0) {
			if (signup(head_u, head_m, &logged_in) == -1) {
				break;
			}
		}
		else if (strcmp(command, "login") == 0) {
			if (login(head_u, head_m, &logged_in) == -1) {
				break;
			}
		}
		else if (strcmp(command, "close\n") == 0) {
			saveinf(head_u, head_m);
			break;
		}
		else if (strcmp(command, "info\n") == 0) {
			info(head_u);
		}
		else {
			printf("Wrong command!,try again\n");
			if (command[strlen(command) - 1] != '\n') {//check buffer:if it is empty do not clear it
				while (getchar() != '\n');
			}
		}
	}
}
char* takingInputFromFile(FILE*myFlie) {
	int k = 0, j;
	char c;
	char*tmp = (char*)malloc(sizeof(char));
	c = fgetc(myFlie);
	while (c != ' ' && c != '\n') {
		tmp[k] = c;
		c = fgetc(myFlie);
		k++;
		tmp = realloc(tmp, (k + 1) * sizeof(char));
	}
	tmp[k] = '\0';
	return tmp;
}
void saveinf(struct user *head_u, struct message *head_m) {
	if (head_u->next == NULL)
		return;
	FILE *inf_u = fopen("usersinformation.txt", "w");
	if (inf_u == NULL) {
		printf("Creating file failed!\n");
		return;
	}
	struct user *ptr1 = head_u->next;
	while (ptr1 != NULL) {
		fprintf(inf_u, "%s %s %s %d\n", ptr1->username, ptr1->pass, ptr1->phone, ptr1->signup);
		ptr1 = ptr1->next;
	}
	if (head_m->next == NULL)
		return;
	FILE *inf_m = fopen("usersms.txt", "w");
	struct message *ptr2 = head_m->next;
	if (inf_m == NULL) {
		printf("Creating file failed!\n");
		return;
	}
	while (ptr2 != NULL) {
		fprintf(inf_m, "%s %s %s %d %d\n", ptr2->from, ptr2->to, ptr2->pm, ptr2->flag, ptr2->sent);
		ptr2 = ptr2->next;
	}
	fclose(inf_u);
	fclose(inf_m);
	destroy(&head_u, &head_m);
}
void destroy(struct user **head_u, struct message **head_m)
{
	struct user *ptr1= *head_u;
	struct message *ptr2 = *head_m;
	struct user *next_u;
	struct message *next_m;
	while (ptr1!= NULL)
	{
		next_u = ptr1->next;
		free(ptr1);
		ptr1 = next_u;
	}
	*head_u = NULL;
	while (ptr2 != NULL)
	{
		next_m = ptr2->next;
		free(ptr2);
		ptr2 = next_m;
	}
	*head_m = NULL;
}
void readinf(struct user *head_u, struct message *head_m) {
	FILE *inf_u = fopen("usersinformation.txt", "r");
	FILE *inf_m = fopen("usersms.txt", "r");
	if (inf_u == NULL) {
		printf("No file to read\n");
		return;
	}
	char *username, *pass, *phone;
	int time_u;
	while (!feof(inf_u)) {
		username = takingInputFromFile(inf_u);
		pass = takingInputFromFile(inf_u);
		phone = takingInputFromFile(inf_u);
		fscanf(inf_u, "%d\n", &time_u);
		adduserfromfile(head_u, username, pass, phone, time_u);
	}
	fclose(inf_u);
	char *from, *to, *pm;
	int flag, time_m;
	if (inf_m == NULL) {
		return;
	}
	while (!feof(inf_m)) {
		from = takingInputFromFile(inf_m);
		to = takingInputFromFile(inf_m);
		pm = takingInputFromFile(inf_m);
		fscanf(inf_m, "%d %d\n", &flag, &time_m);
		addmsfromfile(head_m, from, to, pm, flag, time_m);
	}
	fclose(inf_m);
}
void addmsfromfile(struct message *head_m, char*from, char*to, char*pm, int flag, int time_m) {
	struct message *new_m = (struct message *)malloc(sizeof(struct message));
	struct message *ptr = head_m->next;
	if (new_m == NULL) {
		printf("malloc failed");
		return;
	}
	new_m->from = from;
	new_m->to = to;
	new_m->pm = pm;
	new_m->flag = flag;
	new_m->sent = (time_t)time_m;
	new_m->next = NULL;
	if (head_m->next == NULL) {
		head_m->next = new_m;

	}
	else {
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new_m;
	}
}
void adduserfromfile(struct user *head_u, char *username, char *pass, char *phone, int time) {
	struct user *new_u = (struct user *)malloc(sizeof(struct user));
	struct user *ptr = head_u->next;
	if (new_u == NULL) {
		printf("malloc failed");
		return;
	}
	new_u->username = username;
	new_u->pass = pass;
	new_u->phone = phone;
	new_u->signup = (time_t)time;
	new_u->next = NULL;
	if (head_u->next == NULL) {
		head_u->next = new_u;

	}
	else {
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new_u;
	}
}
int login(struct user *head_u, struct message *head_m, char **logged_in) {
	int flag = 0;
	char * user = givecommend();
	struct user *ptr = head_u->next;
	if (user[strlen(user) - 1] == '\n') {
		printf("Wrong command.\n");
		return 0;
	}
	char *pass = givecommend();
	if (pass[strlen(pass) - 1] != '\n') {
		while (getchar() != '\n');
		printf("Wrong command.\n");
		return 0;
	}
	pass[strlen(pass) - 1] = '\0';
	while (ptr != NULL) {
		if (strcmp(ptr->username, user) == 0 && strcmp(ptr->pass, pass) == 0) {
			flag = 1;
			break;
		}
		else
			ptr = ptr->next;
	}
	if (flag == 0) {
		printf("Username or password is wrong\n");
		return 0;
	}
	else {
		*logged_in = user;
		printf("Successfully Logedin\n");
		show_unread_ms(head_m, logged_in);
		if (menue(head_u, head_m, logged_in) == -1) {
			return -1;
		}
	}
}
void show_unread_ms(struct message *head_m, char **logged_in) {
	struct message *ptr = head_m->next;
	printf("Unread messages:\n");
	while (ptr != NULL) {
		if (ptr->flag == 0 && strcmp(ptr->to, *logged_in) == 0) {
			printf("message from:%s at %s", ptr->from, ctime(&ptr->sent));
		}
		ptr = ptr->next;
	}
}
int signup(struct user *head_u, struct message *head_m, char **logged_in) {
	char *temp_username;
	char*temp_pass;
	char *temp_phone;
	if (getusername(head_u, temp_username = givecommend()) == 0) {
		printf("You did not Enter username!or wrong arguments!\tplease Enter your command again\n");
		if (temp_username[strlen(temp_username) - 1] != '\n') {
			while (getchar() != '\n');
		}
	}
	else if (getpass(head_u, temp_pass = givecommend()) == 0) {
		printf("At least 6 char for pass!or wrong arguments!\tplease Enter your command again\n");
		if (temp_pass[strlen(temp_pass) - 1] != '\n') {
			while (getchar() != '\n');
		}
	}
	else if (getphone(head_u, temp_phone = givecommend()) == 0) {
		printf("Wrong phone format or wrong arguments or used!,try again\n");
		if (temp_phone[strlen(temp_phone) - 1] != '\n') {
			while (getchar() != '\n');
		}
	}
	else {
		adduser(head_u, temp_username, temp_pass, temp_phone);
		*logged_in = temp_username;
		printf("succesfully signedup\n");
		if (menue(head_u, head_m, logged_in) == -1)
			return -1;
	}
}
int menue(struct user *head_u, struct message *head_m, char **logged_in) {
	char *cmd;
	while (true) {
		printf(">> ");
		cmd = givecommend();
		if (strcmp(cmd, "send") == 0) {
			sendpm(head_u, head_m, logged_in);
		}
		else if (strcmp(cmd, "show_sent") == 0) {
			show_sent(head_u, head_m, logged_in);
		}
		else if (strcmp(cmd, "show_received") == 0) {
			show_received(head_u, head_m, logged_in);
		}
		else if (strcmp(cmd, "about") == 0) {
			about(head_u);
		}
		else if (strcmp(cmd, "info\n") == 0) {
			info(head_u);
		}
		else if (strcmp(cmd, "edit_username") == 0) {
			edit_username(head_u, logged_in);
		}
		else if (strcmp(cmd, "edit_pass") == 0) {
			edit_pass(head_u, logged_in);
		}
		else if (strcmp(cmd, "logout\n") == 0) {
			break;
		}
		else if (strcmp(cmd, "close\n") == 0) {
			saveinf(head_u, head_m);
			return -1;
		}
		else {
			printf("Wrong command!,try again\n");
			if (cmd[strlen(cmd) - 1] != '\n') {
				while (getchar() != '\n');
			}
		}
	}
}
void show_received(struct user *head_u, struct message *head_m, char **logged_in) {
	char *sender = givecommend();
	int flag = 0;
	struct message  *ptr = head_m->next;
	if (sender[strlen(sender) - 1] != '\n') {
		while (getchar() != '\n');
		return;
	}
	sender[strlen(sender) - 1] = '\0';
	if (existcheck(head_u, sender) == 0) {
		printf("User not found!\n");
		return;
	}
	while (ptr != NULL) {
		if (strcmp(ptr->from, sender) == 0 && strcmp(ptr->to, *logged_in) == 0) {
			printf("%s to %s:%s\t\t/*at %s", ptr->from, ptr->to, ptr->pm, ctime(&ptr->sent));
			flag = 1;
			ptr->flag = 1;
			break;
		}
		ptr = ptr->next;
	}
	if (flag == 0) {
		printf("No messages to show!\n");
		return;
	}
}
void show_sent(struct user *head_u, struct message *head_m, char **logged_in) {
	char * contact = givecommend();
	int flag = 0;
	struct message  *ptr = head_m->next;
	if (contact[strlen(contact) - 1] != '\n') {
		while (getchar() != '\n');
		return;
	}
	contact[strlen(contact) - 1] = '\0';
	if (existcheck(head_u, contact) == 0) {
		printf("User not found!\n");
		return;
	}
	while (ptr != NULL) {
		if (strcmp(ptr->from, *logged_in) == 0 && strcmp(ptr->to, contact) == 0) {
			if (ptr->flag == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				printf("%s to %s:%s\t\t/*at %s", ptr->from, ptr->to, ptr->pm, ctime(&ptr->sent));
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			else
				printf("%s to %s:%s\t\t/*at %s", ptr->from, ptr->to, ptr->pm, ctime(&ptr->sent));
			flag = 1;
			break;
		}
		ptr = ptr->next;
	}
	if (flag == 0) {
		printf("No messages to show!\n");
		return;
	}
}
int existcheck(struct user *head_u, char * username) {
	struct user *ptr = head_u->next;
	while (ptr != NULL) {
		if (strcmp(username, ptr->username) == 0)
			return 1;
		ptr = ptr->next;
	}
	return 0;
}
void sendpm(struct user *head_u, struct message *head_m, char **logged_in) {
	char * contact;
	int flag = 0;
	struct user *ptr = head_u->next;
	contact = givecommend();
	if (contact[strlen(contact) - 1] == '\n') {
		printf("Wrong arguments!\t,try again\n");
		return;
	}
	else {
		if (existcheck(head_u, contact) == 0) {
			printf("User not found!\n");
			return;
		}
		else {
			addpm(head_m, logged_in, contact);
			printf("Sent\n");
		}
	}
}
void addpm(struct message *head_m, char **logged_in, char *contact) {
	char * content = givecommend();
	struct message *new_m = (struct message*)malloc(sizeof(struct message));
	struct message *ptr = head_m->next;
	if (new_m == NULL) {
		printf("Malloc failed!\n");
		return;
	}
	if (content[strlen(content) - 1] != '\n') {
		printf("Wrong argument\n");
		while (getchar() != '\n');
		return;
	}
	content[strlen(content) - 1] = '\0';
	new_m->flag = 0;
	new_m->from = *logged_in;
	new_m->to = contact;
	new_m->pm = content;
	time(&(new_m->sent));
	new_m->next = NULL;
	if (head_m->next == NULL) {
		head_m->next = new_m;
	}
	else {
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new_m;
	}
}
void about(struct user *head_u) {
	char*username = givecommend();
	struct user *ptr = head_u->next;
	if (username[strlen(username) - 1] != '\n') {
		printf("Wrong command");
		while (getchar() != '\n');
	}
	else {
		username[strlen(username) - 1] = '\0';
		if (existcheck(head_u, username) == 0) {
			printf("User not found!");
			return;
		}
		while (ptr != NULL) {
			if ((strcmp(ptr->username, username) == 0)) {
				printf("%s\t\t%s\n", username, ptr->phone);
				return;
			}
			ptr = ptr->next;
		}

	}
}
void edit_pass(struct user *head_u, char **logged_in) {
	char*new_pass = givecommend();
	struct user *ptr = head_u->next;
	if (new_pass[strlen(new_pass) - 1] != '\n') {
		printf("Wrong command");
		while (getchar() != '\n');
		return;
	}
	else {
		new_pass[strlen(new_pass) - 1] = '\0';
		while (ptr != NULL) {
			if ((strcmp(ptr->username, *logged_in) == 0)) {
				*logged_in = new_pass;
				ptr->pass = new_pass;
				return;
			}
			ptr = ptr->next;
		}
	}
}
void edit_username(struct user *head_u, char **logged_in) {
	char*new_username = givecommend();
	struct user *ptr = head_u->next;
	if (new_username[strlen(new_username) - 1] != '\n') {
		while (getchar() != '\n');
		return;
	}
	else {
		new_username[strlen(new_username) - 1] = '\0';
		if (existcheck(head_u, new_username) == 1) {
			printf("This username is used!\n");
			return;
		}
		while (ptr != NULL) {
			if ((strcmp(ptr->username, *logged_in) == 0)) {
				*logged_in = new_username;
				ptr->username = new_username;
				return;
			}
			ptr = ptr->next;
		}
	}
}
void info(struct user *head_u) {
	struct user*ptr = head_u->next;
	printf("current users are:\n");
	int i = 1;
	while (ptr != NULL)
	{
		printf("%d.%s \n", i, ptr->username);
		ptr = ptr->next;
	}
	return;
}
int getusername(struct user *head_u, char *temp_username) {
	struct user*ptr = head_u->next;
	if (existcheck(head_u, temp_username) == 1) {
		printf("This username is used! or");
		return 0;
	}
	if (strlen(temp_username) == 0 || temp_username[strlen(temp_username) - 1] == '\n') {
		return 0;
	}
	else
		return 1;
}
int getpass(struct user *head_u, char *temp_pass) {
	if (strlen(temp_pass) < 6 || temp_pass[strlen(temp_pass) - 1] == '\n')
		return 0;
	return 1;
}
int getphone(struct user *head_u, char *temp_phone) {
	struct user *ptr = head_u->next;
	if (temp_phone[0] != '0' || strlen(temp_phone) != 12 || temp_phone[strlen(temp_phone) - 1] != '\n') {//one more length for \n
		return 0;
	}
	for (int i = 0; i < 11; i++) {
		if (!(temp_phone[i] >= '0' && temp_phone[i] <= '9'))
			return 0;
	}
	temp_phone[strlen(temp_phone) - 1] = '\0';
	while (ptr != NULL) {
		if (strcmp(temp_phone, ptr->phone) == 0)
			return 0;
		ptr = ptr->next;
	}
	return 1;
}
void adduser(struct user *head_u, char *temp_username, char *temp_pass, char *temp_phone) {
	struct user *new_u = (struct user *)malloc(sizeof(struct user));
	struct user *ptr = head_u->next;
	if (new_u == NULL) {
		printf("malloc failed");
		return;
	}
	new_u->username = temp_username;
	new_u->pass = temp_pass;
	new_u->phone = temp_phone;
	time(&(new_u->signup));
	new_u->next = NULL;
	if (head_u->next == NULL) {
		head_u->next = new_u;

	}
	else {
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new_u;
	}
}
char* givecommend() {
	/*How this works it is a little complicated:
	Imagine user enter "signup fardin 123456 09334997176"
	with this function I pickup "signup" from buffer and other data remains in buffer then when I go to signup I pick up other data part by part!
	& I save where user enter '\n' to check the arguments are correct or not and buffer is empty or not, forexample phone number must have '\n' at the end of it,
	when I become sure that arguments are correct,I manually replace '\n' with '\0'.*/
	int j = 0;
	char x;
	char*commend = (char*)malloc(sizeof(char));
	if (commend == NULL) {
		printf("malloc failed");
	}
	else {
		while ((x = getchar()) != ' ') {
			commend[j] = x;
			if (x == '\n') {
				j++;
				break;
			}
			j++;
			commend = (char*)realloc(commend, (j + 1)*(sizeof(char)));
			if (commend == NULL) {
				printf("realloc failed");
				break;
			}
		}
		commend[j] = '\0';
		return commend;
	}
}