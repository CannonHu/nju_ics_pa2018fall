#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM, REG, SYMB, NEG, DEREF, NEQ, AND, OR
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
	{"\\!=", NEQ},
	{"&&", AND},
	{"\\|\\|", OR},
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
	{DEREF, 2},
	{NEG, 2},
	{'*', 3},
	{'/', 3},
	{'%', 3},
	{'+', 4},
	{'-', 4},
	{EQ, 7},
	{NEQ, 7},
	{AND, 11},
	{OR, 12}
};

#define NR_OPT (sizeof(optable) / sizeof(optable[0]) )

bool isop(int id){
	if(id < 0){
		return false;
	}
	switch(tokens[id].type){
		case '+':
			return true;
		case '-':
			return true;
		case '*':
			return true;
		case '/':
			return true;
		case '%':
			return true;
		case EQ:
			return true;
		case NEQ:
			return true;
		case NEG:
			return true;
		case DEREF:
			return true;
	};
	return false;
}

int findop(int p, int q){
	int c = 0;
	int maxp = 1;
	int maxpnum = p;
	for(int i = p; i <= q; i++){
		if(tokens[i].type == NUM || tokens[i].type == REG || tokens[i].type == SYMB){
			continue;
		}

		if(tokens[i].type == '('){
			c++;
			continue;
		}
		if(tokens[i].type == ')'){
			c--;
			continue;
		}
		if(c == 0){
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
		else{
			continue;
		}
	}
	return maxpnum;
}

uint32_t look_up_symtab(char *sym, bool *success);



int tokentoval(int id){
	int num = 0;

	if(tokens[id].type == NUM){
		num = atoi(tokens[id].str);
	}
	else if(tokens[id].type == REG){
		if(tokens[id].str[2] == 'a'){
			num = cpu.eax;
		}
		else if(tokens[id].str[2] == 'c'){
			num = cpu.ecx;
		}
		else if(tokens[id].str[2] == 'd' && tokens[id].str[3] == 'x'){
			num = cpu.edx;
		}
		else if(tokens[id].str[2] == 'b' && tokens[id].str[3] == 'x'){
			num = cpu.ebx;
		}
		else if(tokens[id].str[2] == 's' && tokens[id].str[3] == 'p'){
			num = cpu.esp;
		}
		else if(tokens[id].str[2] == 'b' && tokens[id].str[3] == 'p'){
			num = cpu.ebp;
		}
		else if(tokens[id].str[2] == 's' && tokens[id].str[3] == 'i'){
			num = cpu.esi;
		}
		else if(tokens[id].str[2] == 'd' && tokens[id].str[3] == 'i'){
			num = cpu.edi;
		}


	}
	else if(tokens[id].type == SYMB){
		vaddr_t addr;
		bool success;
		addr = look_up_symtab(tokens[id].str, &success);
		if(!success){
			printf("SYMB '%s' not found\n",tokens[id].str);
		}
		else{
			num = addr;
		}
	}
	return num;
}

int eval(int p, int q){
	uint32_t vaddr;
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
		switch(tokens[opnum].type){
			case '+':return eval(p, opnum - 1) + eval(opnum + 1, q);
			case '-':return eval(p, opnum - 1) - eval(opnum + 1, q);
			case '*':return	eval(p, opnum - 1) * eval(opnum + 1, q);
			case '/':return eval(p, opnum - 1) / eval(opnum + 1, q);
			case '%':return eval(p, opnum - 1) % eval(opnum + 1, q);
			case EQ:return eval(p, opnum - 1) == eval(opnum + 1, q);
			case NEQ:return eval(p, opnum - 1) != eval(opnum + 1, q);
			case NEG:return 0 - eval(opnum + 1,q);
			case DEREF: 
				 vaddr = eval(opnum + 1,q);
				 return vaddr_read(vaddr, SREG_CS, 4);
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

				//printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
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
	
	for(int i = 0; i < nr_token; i++){
		if(tokens[i].type == '-' && (i == 0 || isop(i-1))){
			tokens[i].type = NEG;
		}
		if(tokens[i].type == '*' && (i == 0 || isop(i-1))){
			tokens[i].type = DEREF;
		}
		
	}

	int tmp = eval(0, nr_token-1);
	printf("res: %d\n", tmp);
	for(int i = 0; i < nr_token; i++)
		for(int j = 0; j < 32; j++){
			tokens[i].str[j] = '\0';
		}
	return tmp;
}

