#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM, REG, SYMB, NEG
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"\\+", '+'},
	{"\\-", '-'},
	{"\\*",'*'},
	{"/",'/'},
	{"%", '%'},
	{"\\(",'('},
	{"\\)",')'},
	{"==", EQ},
	{"[0-9]+", NUM},
	{"\\$e[a,c,d,b]x", REG},
	{"\\$esp", REG},
	{"\\$esi", REG},
	{"[a-zA-Z]+[a-zA-Z0-9]*", SYMB},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

bool check_parentheses(int p,int q){
	int c = 0;
	if(tokens[p].type == '(' && tokens[q].type == ')'){
		for(int i = p + 1; i < q; i++){
			if(tokens[i].type == '('){c++;}
			else if(tokens[i].type == ')'){c--;}
			if(c < 0){
				printf("BAD\n");
				assert(0);
			}
		}
		if(c == 0){
			return true;
		}
		if(c != 0){
			printf("BAD\n");
			assert(0);
		}

	}
	else{
		return false;
	}
	return false;
}

static struct op{
	int type; int priority;
}optable[] = {
	{'(', 1},
	{')', 1},
	{NEG, 2},
	{'*', 3},
	{'/', 3},
	{'%', 3},
	{'+', 4},
	{'-', 4},
	{EQ, 7}
};

#define NR_OPT (sizeof(optable) / sizeof(optable[0]) )


int findop(int p, int q){
	int c = 0;
	int maxp = 1;
	int maxpnum = p;
	for(int i = p; i <= q; i++){
		if(tokens[i].type == '('){
			c++;
			continue;
		}
		else if(tokens[i].type == ')'){
			c--;
			continue;
		}
		else if(c != 0){
			continue;
		}
		else{
			for(int j = 0; j < NR_OPT; j++){
				if(tokens[i].type == optable[j].type){
					if(optable[j].priority > maxp){
						maxp = optable[j].priority;
						maxpnum = i;
						break;
					}
				}
			}
		}
	}
	return maxpnum;
}

int tokentoval(int id){
	int num = 0;
	if(tokens[id].type == NUM){
		num = atoi(tokens[p].str);
		return num;
	}

}

int eval(int p, int q){
	if(p > q){
		printf("BAD\n");
		return 0;
	}
	if(p == q){
		return tokentoval(p);
	}
	else if(check_parentheses(p,q)){
		return eval(p + 1, q - 1);
	}
	else{
		int opnum = findop(p, q);
		int val1 = eval(p, opnum - 1);
		int val2 = eval(opnum + 1, q);
		switch(tokens[opnum].type){
			case '+':return val1 + val2;
			case '-':return val1 - val2;
			case '*':return val1 * val2;
			case '/':return val1 / val2;
			case '%':return val1 % val2;
			default:assert(0);
		}
	}
	return 0;
}

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				if(rules[i].token_type == 256){
					continue;
				}	
				
				for(int j = 0; j < substr_len; j++){
					tokens[nr_token].str[j] = substr_start[j];
				}

				switch(rules[i].token_type) {
					default: tokens[nr_token].type = rules[i].token_type;
							 nr_token ++;
				}

				break;
			}
		}

		if(0 == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	int tmp = eval(0, nr_token-1);
	printf("res: %d\n", tmp);
	return tmp;
}

