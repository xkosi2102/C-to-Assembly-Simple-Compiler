#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_ELEMENT 256
#define MAX_NO_LINES 100
#define MAX_NO_ELEMENTS 1000

int start_while;
int start_for;
char for_var[32];

int isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int isalpha(int c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Funcție pentru eliminarea spațiilor de la început și sfârșit
char *elim(char *str)
{
	if (!str || *str == '\0')
		return str;

	char *start = str;
	char *end = str + strlen(str) - 1;

	while (*start == ' ' || *start == '\n' || *start == '\t')
		start++;

	if (*start == '\0')
		return str;

	while (*end == ' ' || *end == '\n' || *start == '\t')
		end--;

	end[1] = '\0';

	if (start != str) {
		int i;
		for (i = 0; start[i] != '\0'; i++)
			str[i] = start[i];
		str[i] = '\0';
	}

	return str;
}

// Mapare variabile -> registre
const char *get_register(const char *var)
{
	if (strcmp(var, "a") == 0)
		return "eax";

	else if (strcmp(var, "b") == 0)
		return "ebx";

	else if (strcmp(var, "c") == 0)
		return "ecx";

	else if (strcmp(var, "d") == 0)
		return "edx";

	return var;
}

// Funcția de traducere a unei linii de cod C în instrucțiuni ASM
void transpiler(const char *line)
{
	char buffer[MAX_LINE];
	strcpy(buffer, line);

	char *text = elim(buffer);
	// Eliminăm punctul și virgula final (dacă există)
	int len = strlen(text);
	if (text[len - 1] == ';') {
		text[len - 1] = '\0';
		text = elim(text);
	}
	if (strstr(text, "if")) {
		char condition[128];
		if (sscanf(text, "if (%127[^)] )", condition) == 1) {
			char *cond = elim(condition);
			if (strstr(cond, "==")) {
				char op1[32], op2[32];
				if (sscanf(cond, "%31s == %31s", op1, op2) == 2) {
					printf("CMP %s, %s\nJNE end_label\n",
					       get_register(op1),
					       get_register(op2));
					return;
				}
			}
			if (strstr(cond, "!=")) {
				char op1[32], op2[32];
				if (sscanf(cond, "%31s != %31s", op1, op2) == 2) {
					printf("CMP %s, %s\nJE end_label\n",
					       get_register(op1),
					       get_register(op2));
					return;
				}
			}
			if (strstr(cond, "<=")) {
				char op1[32], op2[32];
				if (sscanf(cond, "%31s <= %31s", op1, op2) == 2) {
					printf("CMP %s, %s\nJG end_label\n",
					       get_register(op1),
					       get_register(op2));
					return;
				}
			}
			if (strstr(cond, ">=")) {
				char op1[32], op2[32];
				if (sscanf(cond, "%31s >= %31s", op1, op2) == 2) {
					printf("CMP %s, %s\nJL end_label\n",
					       get_register(op1),
					       get_register(op2));
					return;
				}
			}
			if (strstr(cond, " < ")) {
				char op1[32], op2[32];
				if (sscanf(cond, "%31s < %31s", op1, op2) == 2) {
					printf("CMP %s, %s\nJGE end_label\n",
					       get_register(op1),
					       get_register(op2));
					return;
				}
			}
			if (strstr(cond, " > ")) {
				char op1[32], op2[32];
				if (sscanf(cond, "%31s > %31s", op1, op2) == 2) {
					printf("CMP %s, %s\nJLE end_label\n",
					       get_register(op1),
					       get_register(op2));
					return;
				}
			}
			return;
		}
	}
	if (strstr(text, "while")) {
		char condition[128];
		start_while = 1;
		printf("start_loop:\n");
		sscanf(text, "while (%127[^)] )", condition);
		char *cond = elim(condition);
		if (strstr(cond, "==")) {
			char op1[32], op2[32];
			if (sscanf(cond, "%31s == %31s", op1, op2) == 2) {
				printf("CMP %s, %s\nJNE end_label\n",
				       get_register(op1), get_register(op2));
				return;
			}
		}
		if (strstr(cond, "!=")) {
			char op1[32], op2[32];
			if (sscanf(cond, "%31s != %31s", op1, op2) == 2) {
				printf("CMP %s, %s\nJE end_label\n",
				       get_register(op1), get_register(op2));
				return;
			}
		}
		if (strstr(cond, "<=")) {
			char op1[32], op2[32];
			if (sscanf(cond, "%31s <= %31s", op1, op2) == 2) {
				printf("CMP %s, %s\nJG end_label\n",
				       get_register(op1), get_register(op2));
				return;
			}
		}
		if (strstr(cond, ">=")) {
			char op1[32], op2[32];
			if (sscanf(cond, "%31s >= %31s", op1, op2) == 2) {
				printf("CMP %s, %s\nJL end_label\n",
				       get_register(op1), get_register(op2));
				return;
			}
		}
		if (strstr(cond, " < ")) {
			char op1[32], op2[32];
			if (sscanf(cond, "%31s < %31s", op1, op2) == 2) {
				printf("CMP %s, %s\nJGE end_label\n",
				       get_register(op1), get_register(op2));
				return;
			}
		}
		if (strstr(cond, " > ")) {
			char op1[32], op2[32];
			if (sscanf(cond, "%31s > %31s", op1, op2) == 2) {
				printf("CMP %s, %s\nJLE end_label\n",
				       get_register(op1), get_register(op2));
				return;
			}
		}
	}
	if (strstr(text, "for")) {
		char init[64], cond[64], incr[64];
		start_for = 1;
		if (sscanf(text, "for (%[^;]; %[^;]; %[^)])", init, cond, incr)
		    == 3) {
			char var[32], val[32];
			if (sscanf(init, "%31s = %31s", var, val) == 2) {
				printf("MOV %s, %s\n", get_register(var),
				       isdigit(val[0]) ? val :
				       get_register(val));
			}
			strcpy(for_var, var);
			printf("start_loop:\n");
			char cond_var[32], cond_op[3], cond_val[32];
			if (sscanf
			    (cond, "%31s %2s %31s", cond_var, cond_op,
			     cond_val) == 3) {
				printf("CMP %s, %s\n", get_register(cond_var),
				       isdigit(cond_val[0])
				       ? cond_val : get_register(cond_val));
				if (strcmp(cond_op, "<") == 0)
					printf("JGE end_label\n");

				else if (strcmp(cond_op, ">") == 0)
					printf("JLE end_label\n");

				else if (strcmp(cond_op, "<=") == 0)
					printf("JG end_label\n");

				else if (strcmp(cond_op, ">=") == 0)
					printf("JL end_label\n");

				else if (strcmp(cond_op, "==") == 0)
					printf("JNE end_label\n");

				else if (strcmp(cond_op, "!=") == 0)
					printf("JE end_label\n");
			}
			return;
		}
	}
	// Tratare expresii de asignare
	char *eq = strchr(text, '=');
	if (eq) {
		char left[32], right[128];

		int index = eq - text;
		int i;
		for (i = 0; i < index; i++)
			left[i] = text[i];
		left[i] = '\0';

		int j = 0;
		for (i = index + 1; text[i] != '\0'; i++) {
			right[j] = text[i];
			j++;
		}
		right[j] = '\0';

		char *var = elim(left);
		char *expr = elim(right);

		const char *reg = get_register(var);

		// Dacă expresia este un de tip numeric
		int is_number = 1;
		for (int i = 0; expr[i] != '\0'; i++) {
			if (!isdigit(expr[i])) {
				is_number = 0;
				break;
			}
		}
		if (is_number) {
			printf("MOV %s, %s\n", reg, expr);
			return;
		}
		// Dacă expresia este o variabilă
		if (strlen(expr) == 1 && isalpha(expr[0])) {
			printf("MOV %s, %s\n", reg, get_register(expr));
			return;
		}
		// Operație de adunare
		if (strstr(expr, "+")) {
			char op1[32], op2[32];
			if (sscanf(expr, "%31s + %31s", op1, op2) == 2) {
				if (strcmp(op1, var) == 0) {
					printf("ADD %s, %s\n", reg,
					       isdigit(op2[0]) ? op2 :
					       get_register(op2));
				} else if (strcmp(op2, var) == 0) {
					printf("ADD %s, %s\n", reg,
					       isdigit(op1[0]) ? op1 :
					       get_register(op1));
				} else {
					// Dacă niciunul nu este variabila destinație
					printf("MOV %s, %s\nADD %s, %s\n", reg,
					       isdigit(op1[0])
					       ? op1 : get_register(op1), reg,
					       isdigit(op2[0]) ? op2 :
					       get_register(op2));
				}
			}
		}
		// Operație de scădere
		if (strstr(expr, "-")) {
			char op1[32], op2[32];
			if (sscanf(expr, "%31s - %31s", op1, op2) == 2) {
				if (strcmp(op1, var) == 0) {
					printf("SUB %s, %s\n", reg,
					       isdigit(op2[0]) ? op2 :
					       get_register(op2));
				} else {
					printf("Error %s\n", text);
				}
			}
		}
		// Operație de înmulțire
		if (strstr(expr, "*")) {
			char op1[32], op2[32];
			if (sscanf(expr, "%31s * %31s", op1, op2) == 2) {
				// Dacă variabila destinație apare ca primul operand
				if (strcmp(op1, var) == 0) {
					if (strcmp(var, "a") == 0) {
						printf("MUL %s\n",
						       isdigit(op2[0]) ? op2 :
						       get_register(op2));
					} else {
						// Pentru cazuri cand b nu e mapat pe eax
						printf("MOV eax, %s\n",
						       get_register(var));
						printf("MUL %s\n",
						       isdigit(op2[0]) ? op2 :
						       get_register(op2));
						printf("MOV %s, eax\n",
						       get_register(var));
					}
				}
				// Dacă variabila destinație apare ca al doilea operand
				else if (strcmp(op2, var) == 0) {
					if (strcmp(var, "a") == 0) {
						printf("MUL %s\n",
						       isdigit(op1[0]) ? op1 :
						       get_register(op1));
					} else {
						printf("MOV eax, %s\n",
						       get_register(var));
						printf("MUL %s\n",
						       isdigit(op1[0]) ? op1 :
						       get_register(op1));
						printf("MOV %s, eax\n",
						       get_register(var));
					}
				} else {
					printf("Error: %s\n", text);
				}
			}
		}
		// Operație de împărțire
		if (strstr(expr, "/")) {
			char op1[32], op2[32];
			if (sscanf(expr, "%31s / %31s", op1, op2) == 2) {
				if (strcmp(op1, var) == 0 && *op2 != '0') {
					printf
					    ("MOV eax, %s\nDIV %s\nMOV %s, eax\n",
					     reg,
					     isdigit(op2[0]) ? op2 :
					     get_register(op2), reg);
				} else {
					printf("Error\n");
				}
			}
		}
		// Operații logice: AND, OR, XOR
		if (strstr(expr, "&")) {
			char op1[32], op2[32];
			if (sscanf(expr, "%31s & %31s", op1, op2) == 2) {
				if (strcmp(op1, var) == 0 || strcmp(op2, var) == 0) {
					char *other =
					    (strcmp(op1, var) == 0) ? op2 : op1;
					printf("AND %s, %s\n", reg,
					       (isdigit(other[0]) || (other[0] == '0' && other[1] ==
						    'x')) ? other :
					       get_register(other));
				} else {
					printf("Error %s\n", text);
				}
			}
		}
		if (strstr(expr, "|")) {
			char op1[32], op2[32];
			if (sscanf(expr, "%31s | %31s", op1, op2) == 2) {
				if (strcmp(op1, var) == 0 || strcmp(op2, var) == 0) {
					char *other =
					    (strcmp(op1, var) == 0) ? op2 : op1;
					printf("OR %s, %s\n", reg,
					       (isdigit(other[0]) || (other[0] == '0' && other[1] ==
						    'x')) ? other :
					       get_register(other));
				} else {
					printf("Error %s\n", text);
				}
			}
		}
		if (strstr(expr, "^")) {
			char op1[32], op2[32];
			if (sscanf(expr, "%31s ^ %31s", op1, op2) == 2) {
				if (strcmp(op1, var) == 0 || strcmp(op2, var) == 0) {
					char *other =
					    (strcmp(op1, var) == 0) ? op2 : op1;
					printf("XOR %s, %s\n", reg,
					       (isdigit(other[0]) || (other[0] == '0' && other[1] ==
						    'x')) ? other :
					       get_register(other));
				} else {
					printf("Error %s\n", text);
				}
			}
		}
		// Operații de shift: << și >>
		if (strstr(expr, "<<")) {
			int bits;
			if (sscanf(expr, "%*s << %d", &bits) == 1) {
				printf("SHL %s, %d\n", reg, bits);
				return;
			}
		}
		if (strstr(expr, ">>")) {
			int bits;
			if (sscanf(expr, "%*s >> %d", &bits) == 1) {
				printf("SHR %s, %d\n", reg, bits);
				return;
			}
		}
		return;
	}
	if (strstr(text, "}")) {
		if (start_while == 1)
			printf("JMP start_loop\n");
		if (start_for == 1) {
			printf("ADD %s, 1\n", get_register(for_var));
			printf("JMP start_loop\n");
			printf("end_loop:\n");
			start_for = 0;
			strcpy(for_var, "");
			return;
		}
		printf("end_label:\n");
		start_while = 0;
		return;
	}
}

int main(void)
{
	char element[MAX_ELEMENT];
	char **elements = (char **)malloc(MAX_NO_ELEMENTS * sizeof(char *));
	for (int i = 0; i < MAX_NO_ELEMENTS; i++)
		elements[i] = (char *)malloc(MAX_ELEMENT * sizeof(char));
	int index = 0;

	while (fgets(element, MAX_ELEMENT, stdin)) {
		// Elimină caracterul de newline dacă există
		element[strcspn(element, "\n")] = 0;

		// Verifică dacă linia este goală
		if (strlen(element) == 0)
			continue;
		// Stochează elementul în array
		strncpy(elements[index], element, MAX_ELEMENT - 1);
		elements[index][MAX_ELEMENT - 1] = '\0';

		// Apelează funcția de traducere pentru elementul curent
		transpiler(elements[index]);

		index++;
	}
	// Eliberare memorie
	for (int i = 0; i < MAX_NO_ELEMENTS; i++)
		free(elements[i]);
	free(elements);

	return 0;
}
